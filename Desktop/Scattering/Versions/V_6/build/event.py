import numpy as np
up = __import__('uproot')
import matplotlib.pyplot as plt

def plot_muon_trajectories(root_file_path="muon_mcs_results.root", n_muons=500):
    with up.open(root_file_path) as file:
        tree = file["MuonData"]
        data = tree.arrays(library="np")
    
    valid_indices = np.where(
        (data["x_in1"] > -900) & (data["x_out1"] > -900) &
        (data["x_in2"] > -900) & (data["x_out2"] > -900)
    )[0]
    
    total_events = len(valid_indices)
    if total_events == 0:
        print("[Error] No se encontraron eventos válidos en el archivo ROOT.")
        return

    muons_a_mostrar = min(n_muons, total_events)
    selected_idx = valid_indices[:muons_a_mostrar]
    
    print(f"[+] Eventos válidos totales: {total_events}")
    print(f"[+] Graficando las trayectorias de {muons_a_mostrar} partículas.")

    plt.figure(figsize=(11, 6), dpi=150)
    
    # 2. Dibujar el bloque central de Uranio de 3 m de espesor (-1500 mm a +1500 mm en Z)
    plt.axvspan(-1500, 1500, color='olive', alpha=0.3, label='Bloque de Uranio (3 m)')

    # 3. Graficar las trayectorias reales ordenadas en Z
    for count, i in enumerate(selected_idx):
        z_coords = [data["z_in2"][i], data["z_in1"][i], data["z_out1"][i], data["z_out2"][i]]
        x_coords = [data["x_in2"][i], data["x_in1"][i], data["x_out1"][i], data["x_out2"][i]]
        
        # Segmento 1: Entrada (z_in2 hasta z_in1)
        plt.plot([z_coords[0], z_coords[1]], [x_coords[0], x_coords[1]], 'b-', alpha=0.6, 
                 label='Haz Incidente (Entrada)' if count == 0 else "")
        
        # Segmento 2: Trayectoria dentro del bloque (de z_in1 a z_out1)
        plt.plot([z_coords[1], z_coords[2]], [x_coords[1], x_coords[2]], 'k--', alpha=0.5, 
                 label='Dispersión en el Bloque (U)' if count == 0 else "")
        
        # Segmento 3: Salida (z_out1 hasta z_out2)
        plt.plot([z_coords[2], z_coords[3]], [x_coords[2], x_coords[3]], 'r-', alpha=0.6, 
                 label='Haz Disperso (Salida)' if count == 0 else "")

    plt.xlabel("Eje Z (Trayectoria del haz) [mm]", fontsize=11)
    plt.ylabel("Posición Transversal X [mm]", fontsize=11)
    
    plt.title(f"Reconstrucción de Trayectorias de Muones — 4 GeV (Uranio 3 m)", fontsize=13, fontweight='bold')
    
    info_text = f"Energía: 4 GeV\nEspesor: 3 m (Uranio)\nPartículas graficadas: {muons_a_mostrar}\nTotal válidas: {total_events}"
    plt.gca().text(0.03, 0.92, info_text, transform=plt.gca().transAxes, fontsize=10,
                   verticalalignment='top', bbox=dict(boxstyle='round,pad=0.5', facecolor='white', alpha=0.8, edgecolor='gray'))

    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend(loc='upper right', fontsize=9)
    plt.tight_layout()
    
    output_filename = "trayectorias_muones.png"
    plt.savefig(output_filename, dpi=300)
    print(f"¡Gráfica guardada exitosamente como '{output_filename}'!")
    plt.show()

if __name__ == "__main__":
    plot_muon_trajectories()