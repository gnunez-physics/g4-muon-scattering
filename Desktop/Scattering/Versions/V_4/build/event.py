import numpy as np
up = __import__('uproot')
import matplotlib.pyplot as plt

def plot_muon_trajectories(root_file_path="muon_mcs_results.root", n_muons=100):
    with up.open(root_file_path) as file:
        tree = file["MuonData"]
        data = tree.arrays(library="np")
    
    # Obtener el número total de eventos reales disponibles
    total_events = len(data["x_in1"])
    muons_a_mostrar = min(n_muons, total_events)
    
    print(f"[+] Eventos totales en el archivo: {total_events}")
    print(f"[+] Graficando las trayectorias de {muons_a_mostrar} partículas.")

    # Extraer coordenadas limitadas al tamaño real disponible
    x_in1, z_in1 = data["x_in1"][:muons_a_mostrar], data["z_in1"][:muons_a_mostrar]
    x_in2, z_in2 = data["x_in2"][:muons_a_mostrar], data["z_in2"][:muons_a_mostrar]
    x_out1, z_out1 = data["x_out1"][:muons_a_mostrar], data["z_out1"][:muons_a_mostrar]
    x_out2, z_out2 = data["x_out2"][:muons_a_mostrar], data["z_out2"][:muons_a_mostrar]

    plt.figure(figsize=(10, 6))
    
    # Dibujar el bloque de oro central (de -50 mm a +50 mm en Z)
    plt.axvspan(-50, 50, color='gold', alpha=0.3, label='Bloque de Oro (10 cm)')

    # Graficar las trayectorias de los muones
    for i in range(muons_a_mostrar):
        # 1. Trayectoria de ENTRADA (Haz incidente en Azul)
        plt.plot([z_in2[i], z_in1[i]], [x_in2[i], x_in1[i]], 'b-', alpha=0.7, 
                 label='Entrada (Haz Incidente)' if i == 0 else "")
        
        # 2. Trayectoria de SALIDA (Haz disperso en Rojo)
        plt.plot([z_out1[i], z_out2[i]], [x_out1[i], x_out2[i]], 'r-', alpha=0.7, 
                 label='Salida (Dispersos)' if i == 0 else "")
        
        # 3. Conexión a través del bloque de oro
        plt.plot([z_in1[i], z_out1[i]], [x_in1[i], x_out1[i]], 'k--', alpha=0.3)

    plt.xlabel("Eje Z (Trayectoria del haz) [mm]")
    plt.ylabel("Posición X [mm]")
    
    # Título y cuadro de texto indicando el número de partículas
    plt.title(f"Reconstrucción de Trayectorias de Muones (Mostrando {muons_a_mostrar} de {total_events} partículas)", fontweight='bold')
    
    # Cuadro de texto flotante dentro del gráfico con la estadística
    info_text = f"Partículas graficadas: {muons_a_mostrar}\nTotal simuladas: {total_events}"
    plt.gca().text(0.03, 0.92, info_text, transform=plt.gca().transAxes, fontsize=10,
                   verticalalignment='top', bbox=dict(boxstyle='round,pad=0.5', facecolor='white', alpha=0.8, edgecolor='gray'))

    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend(loc='upper right')
    plt.tight_layout()
    
    output_filename = "trayectorias_muones.png"
    plt.savefig(output_filename, dpi=300)
    print(f"¡Gráfica guardada exitosamente como '{output_filename}'!")
    plt.show()

if __name__ == "__main__":
    plot_muon_trajectories()