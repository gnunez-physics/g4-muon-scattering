import matplotlib.pyplot as plt
import numpy as np
import uproot

# Ruta del archivo ROOT de la simulación
file_path = "muon_mcs_results.root"
tree_name = "MuonData"

print(f"Abriendo archivo: {file_path}")

try:
    with uproot.open(file_path) as file:
        if tree_name in file:
            tree = file[tree_name]
            branches = tree.keys()
            print("Ramas disponibles:", branches)

            # Puedes cambiar aquí la rama según la capa que quieras analizar (ej. x_out1, x_in1, etc.)
            branch_x = "x_out1"
            branch_y = "y_out1"

            if branch_x in branches and branch_y in branches:
                x = tree[branch_x].array(library="np")
                y = tree[branch_y].array(library="np")

                plt.figure(figsize=(9, 8))

                # Usamos hist2d con vmax=5 para realzar los detalles de densidad local
                counts, xedges, yedges, im = plt.hist2d(
                    x, y, bins=200, cmap="inferno", cmin=1, vmax=5
                )

                # Barra de colores
                cbar = plt.colorbar(im)
                cbar.set_label("Número de muones por bin (frecuencia)")

                plt.title(
                    "Mapa de Densidad de Impactos (Silueta de la Caja de Oro)",
                    fontsize=12,
                )
                plt.xlabel("X (mm)", fontsize=11)
                plt.ylabel("Y (mm)", fontsize=11)
                plt.grid(True, linestyle="--", alpha=0.3)
                
                # --- LÍMITES FIJADOS EXACTAMENTE A +/- 5000 mm ---
                plt.xlim(-3500, 3500)
                plt.ylim(-3500, 3500)

                output_image = "silueta_caja_oro.png"
                plt.savefig(output_image, dpi=300, bbox_inches="tight")
                plt.show()
                print(
                    f"¡Listo! Gráfico guardado como '{output_image}'. Ejes configurados de -5000 a 5000 mm."
                )
            else:
                print(f"No se encontraron las ramas {branch_x} / {branch_y} en el árbol.")
        else:
            print(f"No se encontró el árbol '{tree_name}' en el archivo.")

except Exception as e:
    print(f"Error al procesar el archivo ROOT: {e}")