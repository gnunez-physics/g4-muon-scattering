import uproot
import matplotlib.pyplot as plt
import numpy as np
import os

# Define las carpetas numéricas y su etiqueta de energía equivalente
# (Modifica o añade las carpetas según el valor real de energía que simulaste en cada una)
carpetas_energias = {
    "1 GeV": "1/muon_mcs_results.root",
    "2 GeV": "2/muon_mcs_results.root",
    "3 GeV": "3/muon_mcs_results.root",
    "4 GeV": "4/muon_mcs_results.root",
    "5 GeV": "5/muon_mcs_results.root",
    "6 GeV": "6/muon_mcs_results.root",
    "7 GeV": "7/muon_mcs_results.root"
}

plt.figure(figsize=(10, 6), dpi=150)
datos_cargados = False

for energia_str, filepath in carpetas_energias.items():
    
    if not os.path.exists(filepath):
        print(f"[Aviso] No se encontró el archivo en la ruta: {filepath}")
        continue
        
    try:
        with uproot.open(filepath) as f:
            if "MuonData" not in f:
                print(f"[Aviso] El TTree 'MuonData' no está en {filepath}")
                continue
                
            tree = f["MuonData"]
            data = tree.arrays(library="np")
            
            # Extraer coordenadas de los 4 planos del telescopio
            x_in1, z_in1 = data["x_in1"], data["z_in1"]
            x_in2, z_in2 = data["x_in2"], data["z_in2"]
            x_out1, z_out1 = data["x_out1"], data["z_out1"]
            x_out2, z_out2 = data["x_out2"], data["z_out2"]
            
            # Reconstrucción de pendientes y ángulo de desviación proyectado (en mrad)
            dz_in = z_in2 - z_in1
            dz_out = z_out2 - z_out1
            
            mx_in = (x_in2 - x_in1) / dz_in
            mx_out = (x_out2 - x_out1) / dz_out
            
            theta_x_mrad = (np.arctan(mx_out) - np.arctan(mx_in)) * 1000.0
            
            # Graficar histograma normalizado del ángulo de desviación para esta energía
            plt.hist(theta_x_mrad, bins=70, range=(-30, 30), alpha=0.4, density=True, label=f"Energía: {energia_str}")
            datos_cargados = True
            print(f"-> Procesado con éxito: {energia_str} desde {filepath}")
            
    except Exception as e:
        print(f"Error procesando {filepath}: {e}")

if datos_cargados:
    plt.xlabel(r"Ángulo de Desviación Proyectado $\Delta\theta_x$ [mrad]", fontsize=12)
    plt.ylabel("Densidad de Probabilidad Normalizada", fontsize=12)
    plt.title("Efecto de la Energía en el Ángulo de Dispersión (MCS)", fontsize=14)
    plt.legend()
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()
    plt.savefig("comparativa_scattering_energias.png")
    print("¡Gráfica comparativa guardada exitosamente como 'comparativa_scattering_energias.png'!")
    plt.show()
else:
    print("\n[Error] No se pudo graficar: verifica los nombres de las carpetas y los archivos .root en su interior.")