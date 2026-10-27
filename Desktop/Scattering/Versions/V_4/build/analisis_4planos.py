import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import uproot

def gaussian(x, amp, mean, stddev):
    return amp * np.exp(-0.5 * ((x - mean) / stddev) ** 2)

def analizar_telescopio_con_teoria(filename="muon_mcs_results.root"):
    # 1. Cargar datos del TTree de Geant4 (4 planos)
    with uproot.open(filename) as file:
        tree = file["MuonData"]
        data = tree.arrays(library="np")
    
    x_in1, z_in1 = data["x_in1"], data["z_in1"]
    x_in2, z_in2 = data["x_in2"], data["z_in2"]
    
    x_out1, z_out1 = data["x_out1"], data["z_out1"]
    x_out2, z_out2 = data["x_out2"], data["z_out2"]
    
    # 2. Reconstrucción de pendientes y ángulo de desviación proyectado
    m_x_in = (x_in2 - x_in1) / (z_in2 - z_in1)
    m_x_out = (x_out2 - x_out1) / (z_out2 - z_out1)
    
    theta_x_rad = np.arctan(m_x_out) - np.arctan(m_x_in)
    theta_x_mrad = theta_x_rad * 1000.0
    
    # 3. Ajuste Gaussiano del núcleo (aislando colas)
    p_low, p_high = np.percentile(theta_x_mrad, [2, 98])
    core_mask = (theta_x_mrad >= p_low) & (theta_x_mrad <= p_high)
    theta_core = theta_x_mrad[core_mask]
    
    hist, bin_edges = np.histogram(theta_core, bins=60, density=True)
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
    
    p0 = [max(hist), np.mean(theta_core), np.std(theta_core)]
    try:
        popt, _ = curve_fit(gaussian, bin_centers, hist, p0=p0)
        sigma_gaussian = abs(popt[2])
    except RuntimeError:
        sigma_gaussian = np.std(theta_x_mrad)

    sigma_bruto = np.std(theta_x_mrad)

    # 4. Parámetros Teóricos (Bloque de Oro de 10 cm, muones a 3 GeV)
    material_nombre = "Oro (G4_Au)"
    X_0 = 3.34           # Longitud de radiación oficial para G4_Au en mm
    thickness_mm = 100.0 # Espesor real del bloque en Z (10 cm = 100 mm)
    p_momentum = 3000.0  # Momento del muón en MeV/c (3 GeV)
    beta = 1.0           
    
    t_ratio = thickness_mm / X_0
    
    # Fórmula de Highland (PDG)
    theta_theo = (13.6 / (beta * p_momentum)) * np.sqrt(t_ratio) * (1.0 + 0.038 * np.log(t_ratio)) * 1000.0
    
    # 5. Reporte por consola
    print("=" * 65)
    print("   ANÁLISIS DE TELESCOPIO 4 PLANOS + TEORÍA DE HIGHLAND")
    print("=" * 65)
    print(f" Eventos analizados            : {len(theta_x_mrad)}")
    print(f" Material configurado          : {material_nombre}")
    print(f" Espesor del bloque ($x$)      : {thickness_mm:.1f} mm")
    print(f" Momento de los muones         : {p_momentum/1000.0:.1f} GeV/c")
    print("-" * 65)
    print(f" Teoría de Highland (Analítica): {theta_theo:.2f} mrad")
    print(f" Sigma del Núcleo (Fit Gauss)  : {sigma_gaussian:.2f} mrad")
    print(f" RMS Global (con colas)        : {sigma_bruto:.2f} mrad")
    print("=" * 65)

    # 6. Generación de la gráfica con la línea teórica
    plt.figure(figsize=(9, 6))
    plt.hist(theta_x_mrad, bins=80, density=True, alpha=0.5, color='forestgreen', edgecolor='darkgreen', label='Simulación Telescopio ($\Delta\theta_x$)')
    
    x_plot = np.linspace(min(theta_x_mrad), max(theta_x_mrad), 300)
    plt.plot(x_plot, gaussian(x_plot, *popt), 'r-', lw=2.5, label=f'Ajuste Núcleo ($\sigma$ = {sigma_gaussian:.2f} mrad)')
    
    # Línea vertical de la Teoría de Highland
    plt.axvline(theta_theo, color='darkorange', linestyle='--', linewidth=2.5, label=f'Teoría Highland ({theta_theo:.2f} mrad)')
    
    plt.xlabel(r"Ángulo de Desviación Proyectado $\Delta\theta_x$ [mrad]", fontsize=11)
    plt.ylabel("Densidad de Probabilidad", fontsize=11)
    plt.title(f"Dispersión Múltiple de Coulomb — {material_nombre} (10 cm, 3 GeV)", fontsize=13, fontweight='bold')
    plt.legend(loc='upper right', fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.tight_layout()
    
    output_filename = "mcs_4planos_con_teoria.png"
    plt.savefig(output_filename, dpi=300)
    print(f"[+] Gráfica guardada exitosamente como '{output_filename}'")
    plt.show()

if __name__ == "__main__":
    analizar_telescopio_con_teoria()