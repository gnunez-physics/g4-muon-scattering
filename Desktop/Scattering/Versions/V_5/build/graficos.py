import numpy as np
up = __import__('uproot')
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def gaussian(x, amp, mean, stddev):
    return amp * np.exp(-0.5 * ((x - mean) / stddev) ** 2)

def generar_dashboard_telescopio(root_file_path="muon_mcs_results.root"):
    # 1. Leer datos del TTree de Geant4 (4 planos)
    with up.open(root_file_path) as file:
        tree = file["MuonData"]
        data = tree.arrays(library="np")
    
    # Coordenadas de los 4 planos
    x_in1, y_in1, z_in1 = data["x_in1"], data["y_in1"], data["z_in1"]
    x_in2, y_in2, z_in2 = data["x_in2"], data["y_in2"], data["z_in2"]
    
    x_out1, y_out1, z_out1 = data["x_out1"], data["y_out1"], data["z_out1"]
    x_out2, y_out2, z_out2 = data["x_out2"], data["y_out2"], data["z_out2"]

    # 2. Reconstrucción de vectores directores y pendientes con protección contra divisiones por cero
    dz_in = z_in2 - z_in1
    dz_out = z_out2 - z_out1
    
    dz_in[dz_in == 0] = np.nan
    dz_out[dz_out == 0] = np.nan

    mx_in = (x_in2 - x_in1) / dz_in
    my_in = (y_in2 - y_in1) / dz_in
    
    mx_out = (x_out2 - x_out1) / dz_out
    my_out = (y_out2 - y_out1) / dz_out

    # 3. Ángulos de desviación proyectados (en milirradianes)
    theta_x_mrad = (np.arctan(mx_out) - np.arctan(mx_in)) * 1000.0
    theta_y_mrad = (np.arctan(my_out) - np.arctan(my_in)) * 1000.0
    
    # Filtrar únicamente valores finitos (eliminar NaNs e Infs)
    valid_mask = np.isfinite(theta_x_mrad) & np.isfinite(theta_y_mrad)
    theta_x_mrad = theta_x_mrad[valid_mask]
    theta_y_mrad = theta_y_mrad[valid_mask]
    
    theta_3d_mrad = np.sqrt(theta_x_mrad**2 + theta_y_mrad**2)
    n_events = len(theta_x_mrad)
    
    print(f"[+] Eventos válidos procesados para el dashboard: {n_events}")

    # 4. Configuración del Dashboard (10 x 8)
    fig, axs = plt.subplots(2, 2, figsize=(11, 9))
    fig.suptitle(f"Dashboard de Telescopio MCS — Oro 20 cm, 4 GeV (N = {n_events} eventos)", fontsize=13, fontweight='bold')

    # --- PANEL 1 (Arriba-Izquierda): Ángulo Proyectado Theta_X ---
    counts_x, edges_x = np.histogram(theta_x_mrad, bins=80, density=True)
    centers_x = (edges_x[:-1] + edges_x[1:]) / 2
    popt_x, _ = curve_fit(gaussian, centers_x, counts_x, p0=[max(counts_x), 0, np.std(theta_x_mrad)])
    sigma_x = abs(popt_x[2])
    
    # Rango acotado a +/- 5 sigmas del fit
    xlim_x = 5.0 * sigma_x
    axs[0, 0].set_xlim(-xlim_x, xlim_x)

    axs[0, 0].hist(theta_x_mrad, bins=100, range=(-xlim_x, xlim_x), density=True, alpha=0.6, color='forestgreen', edgecolor='darkgreen', label='Simulación $\Delta\theta_x$')
    x_fit = np.linspace(-xlim_x, xlim_x, 200)
    axs[0, 0].plot(x_fit, gaussian(x_fit, *popt_x), 'r-', lw=2, label=f'Fit ($\sigma$ = {sigma_x:.2f} mrad)')
    axs[0, 0].set_title("Desviación Proyectada en X ($\Delta\theta_x$)", fontsize=10, fontweight='bold')
    axs[0, 0].set_xlabel(r"$\Delta\theta_x$ [mrad]", fontsize=9)
    axs[0, 0].set_ylabel("Densidad de Probabilidad", fontsize=9)
    axs[0, 0].grid(True, linestyle='--', alpha=0.5)
    axs[0, 0].legend(loc='upper right', fontsize=8)

    # --- PANEL 2 (Arriba-Derecha): Ángulo Proyectado Theta_Y ---
    counts_y, edges_y = np.histogram(theta_y_mrad, bins=80, density=True)
    centers_y = (edges_y[:-1] + edges_y[1:]) / 2
    popt_y, _ = curve_fit(gaussian, centers_y, counts_y, p0=[max(counts_y), 0, np.std(theta_y_mrad)])
    sigma_y = abs(popt_y[2])
    
    xlim_y = 5.0 * sigma_y
    axs[0, 1].set_xlim(-xlim_y, xlim_y)

    axs[0, 1].hist(theta_y_mrad, bins=100, range=(-xlim_y, xlim_y), density=True, alpha=0.6, color='royalblue', edgecolor='midnightblue', label='Simulación $\Delta\theta_y$')
    axs[0, 1].plot(x_fit, gaussian(x_fit, *popt_y), 'r-', lw=2, label=f'Fit ($\sigma$ = {sigma_y:.2f} mrad)')
    axs[0, 1].set_title("Desviación Proyectada en Y ($\Delta\theta_y$)", fontsize=10, fontweight='bold')
    axs[0, 1].set_xlabel(r"$\Delta\theta_y$ [mrad]", fontsize=9)
    axs[0, 1].set_ylabel("Densidad de Probabilidad", fontsize=9)
    axs[0, 1].grid(True, linestyle='--', alpha=0.5)
    axs[0, 1].legend(loc='upper right', fontsize=8)

    # --- PANEL 3 (Abajo-Izquierda): Ángulo Espacial Tridimensional ---
    # Acotamos también el 3D para enfocar la distribución principal (ej. hasta el percentil 98)
    max_3d = np.percentile(theta_3d_mrad, 98) * 1.1
    axs[1, 0].set_xlim(0, max_3d)

    axs[1, 0].hist(theta_3d_mrad, bins=80, range=(0, max_3d), color='darkorange', edgecolor='saddlebrown', alpha=0.8)
    axs[1, 0].set_title(f"Ángulo Espacial Tridimensional ($\theta_{{3D}}$)\nMedia = {np.mean(theta_3d_mrad):.2f} mrad", fontsize=10, fontweight='bold')
    axs[1, 0].set_xlabel(r"$\theta_{3D}$ [mrad]", fontsize=9)
    axs[1, 0].set_ylabel("Cuentas", fontsize=9)
    axs[1, 0].grid(True, linestyle='--', alpha=0.5)

    # --- PANEL 4 (Abajo-Derecha): Correlación 2D de Ángulos ---
    # Limitamos el mapa de calor 2D al mismo rango que los proyectados para apreciar la simetría circular
    hb = axs[1, 1].hist2d(theta_x_mrad, theta_y_mrad, bins=80, range=[[-xlim_x, xlim_x], [-xlim_y, xlim_y]], cmap='plasma')
    axs[1, 1].set_title("Correlación Angular Transversal ($\Delta\theta_x$ vs $\Delta\theta_y$)", fontsize=10, fontweight='bold')
    axs[1, 1].set_xlabel(r"$\Delta\theta_x$ [mrad]", fontsize=9)
    axs[1, 1].set_ylabel(r"$\Delta\theta_y$ [mrad]", fontsize=9)
    axs[1, 1] .grid(True, linestyle='--', alpha=0.3)
    
    cb = fig.colorbar(hb[3], ax=axs[1, 1])
    cb.set_label("Eventos", fontsize=9)

    # Ajustes finales
    plt.tight_layout()
    output_filename = "mcs_dashboard_telescopio.png"
    plt.savefig(output_filename, dpi=300)
    print(f"¡Dashboard avanzado guardado exitosamente como '{output_filename}'!")
    plt.show()

if __name__ == "__main__":
    generar_dashboard_telescopio()