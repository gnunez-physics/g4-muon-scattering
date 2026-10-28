import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import uproot

def analizar_p_scan(nombre_archivo="muon_mcs_results.root", tree_name="MuonData"):
    # Listar carpetas con nombres numéricos (que representan el momento p en GeV)
    directorios = [d for d in os.listdir('.') if os.path.isdir(d) and d.isdigit()]
    momentos_p = sorted([float(d) for d in directorios])
    
    if not momentos_p:
        print("No se encontraron carpetas con nombres numéricos (ej. 1, 3, 5, 10) en el directorio actual.")
        return

    sigmas = []
    inverso_p = []

    print(f"Carpetas/Momentos encontrados: {momentos_p} GeV\n")

    for p in momentos_p:
        carpeta = f"{int(p)}" if p.is_integer() else f"{p}"
        ruta_archivo = os.path.join(carpeta, nombre_archivo)
        
        if not os.path.exists(ruta_archivo):
            print(f"[Advertencia] No se encontró el archivo '{nombre_archivo}' en la carpeta '{carpeta}/'. Se omite.")
            continue
            
        try:
            with uproot.open(ruta_archivo) as archivo:
                if tree_name in archivo:
                    tree = archivo[tree_name]
                    
                    # Extraer coordenadas de entrada y salida
                    x_in1 = tree['x_in1'].array(library="np")
                    x_in2 = tree['x_in2'].array(library="np")
                    z_in1 = tree['z_in1'].array(library="np")
                    z_in2 = tree['z_in2'].array(library="np")
                    
                    x_out1 = tree['x_out1'].array(library="np")
                    x_out2 = tree['x_out2'].array(library="np")
                    z_out1 = tree['z_out1'].array(library="np")
                    z_out2 = tree['z_out2'].array(library="np")
                    
                    # Calcular pendientes (direcciones) en el plano XZ
                    dx_in = x_in2 - x_in1
                    dz_in = z_in2 - z_in1
                    slope_in = dx_in / dz_in
                    
                    dx_out = x_out2 - x_out1
                    dz_out = z_out2 - z_out1
                    slope_out = dx_out / dz_out
                    
                    # Ángulo de desviación proyectado en el plano XZ (en radianes)
                    theta_in = np.arctan(slope_in)
                    theta_out = np.arctan(slope_out)
                    delta_theta = theta_out - theta_in
                    
                    delta_theta = delta_theta[np.isfinite(delta_theta)]
                    
                    sigma_theta = np.std(delta_theta) * 1000.0  # en milirradianes
                    
                    sigmas.append(sigma_theta)
                    inverso_p.append(1.0 / p)
                    print(f" -> p = {p} GeV | 1/p = {1/p:.4f} | sigma_theta = {sigma_theta:.4f} mrad")
                else:
                    print(f"[Error] El TTree '{tree_name}' no se encuentra en {ruta_archivo}.")
        except Exception as e:
            print(f"[Error] Al procesar el archivo {ruta_archivo}: {e}")

    if not sigmas:
        print("No se pudieron extraer datos válidos para realizar el gráfico.")
        return

    inverso_p = np.array(inverso_p)
    sigmas = np.array(sigmas)

    def func_lineal(x, m, c):
        return m * x + c

    popt, pcov = curve_fit(func_lineal, inverso_p, sigmas)
    m_opt, c_opt = popt
    
    x_fit = np.linspace(min(inverso_p), max(inverso_p), 100)
    y_fit = func_lineal(x_fit, m_opt, c_opt)

    residuos = sigmas - func_lineal(inverso_p, m_opt, c_opt)
    ss_res = np.sum(residuos**2)
    ss_tot = np.sum((sigmas - np.mean(sigmas))**2)
    r_squared = 1 - (ss_res / ss_tot)
    print(f"\nCoeficiente de determinación R^2: {r_squared:.4f}")

    plt.figure(figsize=(8, 6))
    plt.scatter(inverso_p, sigmas, color='blue', label='Datos Simulados Uranio 3m ($\sigma_{\\theta_x}$)', zorder=5)
    plt.plot(x_fit, y_fit, color='red', linestyle='--', label=f'Ajuste Lineal: $\\theta_0 \\propto 1/p$\n$y = {m_opt:.3f}(1/p) + {c_opt:.3f}$ ($R^2={r_squared:.3f}$)')
    
    plt.title('Estudio de la Dependencia con el Momento ($p$-scan) - Uranio (3 m)', fontsize=13, fontweight='bold')
    plt.xlabel('$1 / p$ $(\mathrm{GeV}^{-1})$', fontsize=12)
    plt.ylabel('Anchura $\\sigma_{\\theta}$ (mrad)', fontsize=12)
    plt.grid(True, linestyle=':', alpha=0.7)
    plt.legend(fontsize=11)
    
    output_filename = 'dependencia_momento_highland_corregido.png'
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')
    print(f"¡Gráfico guardado exitosamente como '{output_filename}'!")
    plt.show()

if __name__ == "__main__":
    analizar_p_scan()