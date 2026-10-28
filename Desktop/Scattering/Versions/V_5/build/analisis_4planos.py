import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import uproot

def gaussian_norm(x, mean, stddev):
    """Gaussiana normalizada a densidad de probabilidad (área = 1)."""
    return (1.0 / (stddev * np.sqrt(2.0 * np.pi))) * np.exp(-0.5 * ((x - mean) / stddev) ** 2)

def analizar_telescopio_con_teoria(filename="muon_mcs_results.root"):
    # 1. Cargar datos del TTree de Geant4 (4 planos)
    with uproot.open(filename) as file:
        tree = file["MuonData"]
        data = tree.arrays(library="np")
        
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

    # Reconstrucción de pendientes
    m_x_in = (x_in2 - x_in1) / (z_in2 - z_in1)
    m_x_out = (x_out2 - x_out1) / (z_out2 - z_out1)
    
    theta_x_rad = np.arctan(m_x_out) - np.arctan(m_x_in)
    theta_x_mrad = theta_x_rad * 1000.0

    # Limpieza estricta: conservar únicamente valores finitos (sin NaN ni Inf)
    finite_mask = np.isfinite(theta_x_mrad)
    theta_x_mrad = theta_x_mrad[finite_mask]

    if len(theta_x_mrad) == 0:
        print("[!] Error: No hay eventos válidos en theta_x_mrad después de la limpieza.")
        return

    # 3. Ajuste Gaussiano del núcleo (aislando colas)
    p_low, p_high = np.percentile(theta_x_mrad, [2, 98])
    core_mask = (theta_x_mrad >= p_low) & (theta_x_mrad <= p_high)
    theta_core = theta_x_mrad[core_mask]
    
    hist, bin_edges = np.histogram(theta_core, bins=60, density=True)
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
    
    # Valores iniciales para el fit [media, std]
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

    # 4. Parámetros Teóricos (Bloque de Oro de 20 cm, muones a 4 GeV)
    material_nombre = "Oro (G4_Au)"
    X_0 = 3.34            # Longitud de radiación oficial para G4_Au en mm
    thickness_mm = 200.0  # Espesor real del bloque en Z (200 mm = 20 cm)
    p_momentum = 4000.0   # Momento del muón en MeV/c (4 GeV/c)
    beta = 1.0           
    
    t_ratio = thickness_mm / X_0
    
    # Fórmula de Highland (PDG) -> nos da la SIGMA teórica en mrad
    theta_theo_sigma = (13.6 / (beta * p_momentum)) * np.sqrt(t_ratio) * (1.0 + 0.038 * np.log(t_ratio)) * 1000.0
    
    # 5. Reporte por consola
    print("=" * 65)
    print("   ANÁLISIS DE TELESCOPIO 4 PLANOS + TEORÍA DE HIGHLAND")
    print("=" * 65)
    print(f" Eventos válidos analizados    : {len(theta_x_mrad)}")
    print(f" Material configurado          : {material_nombre}")
    print(f" Espesor del bloque (x)        : {thickness_mm:.1f} mm")
    print(f" Momento de los muones         : {p_momentum/1000.0:.1f} GeV/c")
    print("-" * 65)
    print(f" Teoría de Highland (Sigma)   : {theta_theo_sigma:.2f} mrad")
    print(f" Sigma del Núcleo (Fit Gauss)  : {sigma_gaussian:.2f} mrad")
    print(f" RMS Global (con colas)        : {sigma_bruto:.2f} mrad")
    print("=" * 65)

    # 6. Generación de la gráfica con RANGO VISUAL ACOTADO
    plt.figure(figsize=(9, 6))

    # Límite visual automático centrado en la media
    ancho_vista = 5.0 * theta_theo_sigma  
    x_min_limit = mean_gaussian - ancho_vista
    x_max_limit = mean_gaussian + ancho_vista

    plt.xlim(x_min_limit, x_max_limit)

    plt.hist(theta_x_mrad, bins=100, range=(x_min_limit, x_max_limit), density=True, 
             alpha=0.5, color='forestgreen', edgecolor='darkgreen', 
             label='Simulación Telescopio ($\Delta\\theta_x$)')
    
    x_plot = np.linspace(x_min_limit, x_max_limit, 400)
    
    # Curva Gaussiana Teórica de Highland
    y_highland = gaussian_norm(x_plot, 0.0, theta_theo_sigma)
    plt.plot(x_plot, y_highland, 'k--', lw=2.5, label=f'Teoría Highland ($\sigma$ = {theta_theo_sigma:.2f} mrad)')

    # Curva del Fit a los Datos de Geant4
    if fit_exitoso:
        y_fit = gaussian_norm(x_plot, mean_gaussian, sigma_gaussian)
        plt.plot(x_plot, y_fit, 'r-', lw=2.0, label=f'Ajuste Núcleo ($\sigma$ = {sigma_gaussian:.2f} mrad)')
    
    plt.xlabel(r"Ángulo de Desviación Proyectado $\Delta\theta_x$ [mrad]", fontsize=11)
    plt.ylabel("Densidad de Probabilidad", fontsize=11)
    plt.title(f"Dispersión Múltiple de Coulomb — {material_nombre} ({thickness_mm/10:.0f} cm, {p_momentum/1000:.0f} GeV)", fontsize=13, fontweight='bold')
    plt.legend(loc='upper right', fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.tight_layout()
    
    output_filename = "mcs_4planos_con_teoria.png"
    plt.savefig(output_filename, dpi=300)
    print(f"[+] Gráfica guardada exitosamente como '{output_filename}'")
    plt.show()

if __name__ == "__main__":
    analizar_telescopio_con_teoria()