import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import uproot

def gaussian_norm(x, mean, stddev):
    """Gaussiana normalizada a densidad de probabilidad (área = 1)."""
    return (1.0 / (stddev * np.sqrt(2.0 * np.pi))) * np.exp(-0.5 * ((x - mean) / stddev) ** 2)

def analizar_telescopio_con_teoria(filename="muon_mcs_results.root"):
    # 1. Cargar datos del TTree de Geant4 (4 planos)
    try:
        with uproot.open(filename) as file:
            tree = file["MuonData"]
            data = tree.arrays(library="np")
    except Exception as e:
        print(f"[!] Error al abrir el archivo ROOT: {e}")
        return
        
    x_in1, z_in1 = data["x_in1"], data["z_in1"]
    x_in2, z_in2 = data["x_in2"], data["z_in2"]
    x_out1, z_out1 = data["x_out1"], data["z_out1"]
    x_out2, z_out2 = data["x_out2"], data["z_out2"]
    
    # 2. Filtrado de eventos válidos para evitar división por cero (dz != 0)
    valid_mask = (z_in2 != z_in1) & (z_out2 != z_out1)
    
    x_in1, z_in1 = x_in1[valid_mask], z_in1[valid_mask]
    x_in2, z_in2 = x_in2[valid_mask], z_in2[valid_mask]
    x_out1, z_out1 = x_out1[valid_mask], z_out1[valid_mask]
    x_out2, z_out2 = x_out2[valid_mask], z_out2[valid_mask]

    # Reconstrucción de pendientes (dx/dz)
    m_x_in = (x_in2 - x_in1) / (z_in2 - z_in1)
    m_x_out = (x_out2 - x_out1) / (z_out2 - z_out1)
    
    theta_x_rad = np.arctan(m_x_out) - np.arctan(m_x_in)
    theta_x_mrad = theta_x_rad * 1000.0

    # Limpieza estricta: conservar únicamente valores finitos
    finite_mask = np.isfinite(theta_x_mrad)
    theta_x_mrad = theta_x_mrad[finite_mask]

    if len(theta_x_mrad) == 0:
        print("[!] Error: No hay eventos válidos en theta_x_mrad después de la limpieza.")
        return

    # 3. Ajuste Gaussiano del núcleo (aislando colas no gaussianas)
    p_low, p_high = np.percentile(theta_x_mrad, [5, 95])
    core_mask = (theta_x_mrad >= p_low) & (theta_x_mrad <= p_high)
    theta_core = theta_x_mrad[core_mask]
    
    hist, bin_edges = np.histogram(theta_core, bins=60, density=True)
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
    
    p0 = [np.mean(theta_core), np.std(theta_core)]
    fit_exitoso = False
    
    try:
        popt, _ = curve_fit(gaussian_norm, bin_centers, hist, p0=p0)
        mean_gaussian = popt[0]
        sigma_gaussian = abs(popt[1])
        fit_exitoso = True
    except (RuntimeError, ValueError):
        mean_gaussian = np.mean(theta_core)
        sigma_gaussian = np.std(theta_core)

    sigma_bruto = np.std(theta_x_mrad)

    # 4. Parámetros Físicos y Teóricos (¡Verifica aquí el espesor real de tu bloque!)
    material_nombre = "Uranio (G4_U)"
    X_0 = 3.16            # Longitud de radiación para G4_U en mm (~3.16 mm)[cite: 8]
    
    # ⚠️ ATENCIÓN: Si tu bloque no es de 3 metros (3000 mm), cambia este valor al real (ej. 30.0 mm para 3 cm)
    thickness_mm = 30.0   # <-- Cambiar si el espesor en Geant4 es diferente de 3000 mm
    
    p_momentum = 4000.0   # Momento inicial del muón en MeV/c (4 GeV/c)[cite: 8]
    beta = 1.0           
    
    t_ratio = thickness_mm / X_0
    
    # Validación de validez de Highland (advertir si el espesor supera los límites razonables)
    if t_ratio > 20:
        print(f"[!] Advertencia: t/X0 = {t_ratio:.1f} es muy alto. La fórmula de Highland estándar pierde precisión por pérdida de energía.")

    # Fórmula de Highland (PDG) -> SIGMA teórica en mrad
    theta_theo_sigma = (13.6 / (beta * p_momentum)) * np.sqrt(t_ratio) * (1.0 + 0.038 * np.log(t_ratio)) * 1000.0
    
    # 5. Reporte por consola
    print("=" * 65)
    print("   ANÁLISIS DE TELESCOPIO 4 PLANOS + TEORÍA DE HIGHLAND")
    print("=" * 65)
    print(f" Eventos válidos analizados    : {len(theta_x_mrad)}")
    print(f" Material configurado          : {material_nombre}")
    print(f" Espesor del bloque (Z)        : {thickness_mm:.1f} mm")
    print(f" Momento de los muones         : {p_momentum/1000.0:.1f} GeV/c")
    print("-" * 65)
    print(f" Teoría de Highland (Sigma)   : {theta_theo_sigma:.2f} mrad")
    print(f" Sigma del Núcleo (Fit Gauss)  : {sigma_gaussian:.2f} mrad")
    print(f" RMS Global (con colas)        : {sigma_bruto:.2f} mrad")
    print("=" * 65)

    # 6. Generación de la gráfica con RANGO VISUAL ACOTADO
    plt.figure(figsize=(9, 6))

    ancho_vista = 5.0 * max(theta_theo_sigma, sigma_gaussian)  
    x_min_limit = mean_gaussian - ancho_vista
    x_max_limit = mean_gaussian + ancho_vista

    plt.xlim(x_min_limit, x_max_limit)

    plt.hist(theta_x_mrad, bins=120, range=(x_min_limit, x_max_limit), density=True, 
             alpha=0.5, color='forestgreen', edgecolor='darkgreen', 
             label='Simulación Geant4 ($\Delta\\theta_x$)')
    
    x_plot = np.linspace(x_min_limit, x_max_limit, 400)
    
    y_highland = gaussian_norm(x_plot, mean_gaussian, theta_theo_sigma)
    plt.plot(x_plot, y_highland, 'k--', lw=2.5, label=f'Teoría Highland ($\sigma$ = {theta_theo_sigma:.2f} mrad)')

    if fit_exitoso:
        y_fit = gaussian_norm(x_plot, mean_gaussian, sigma_gaussian)
        plt.plot(x_plot, y_fit, 'r-', lw=2.0, label=f'Ajuste Núcleo Geant4 ($\sigma$ = {sigma_gaussian:.2f} mrad)')
    
    plt.xlabel(r"Ángulo de Desviación Proyectado $\Delta\theta_x$ [mrad]", fontsize=11)
    plt.ylabel("Densidad de Probabilidad", fontsize=11)
    plt.title(f"Dispersión Múltiple de Coulomb — {material_nombre} ({thickness_mm} mm, {p_momentum/1000:.0f} GeV)", fontsize=13, fontweight='bold')
    plt.legend(loc='upper right', fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.tight_layout()
    
    output_filename = "mcs_4planos_con_teoria.png"
    plt.savefig(output_filename, dpi=300)
    print(f"[+] Gráfica guardada exitosamente como '{output_filename}'")
    plt.show()

if __name__ == "__main__":
    analizar_telescopio_con_teoria()