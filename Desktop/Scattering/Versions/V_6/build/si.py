import matplotlib.pyplot as plt
import numpy as np
import uproot

# Ruta del archivo ROOT generado por Geant4
file_path = "muon_mcs_results.root"
print(f"Abriendo archivo: {file_path}")

with uproot.open(file_path) as file:
  # Verificar las trees disponibles en el archivo
  print("Trees disponibles:", file.keys())
  tree = file["MuonData"]

  # Leer las variables de posición y IDs para las capas de entrada y salida
  # Capa 1
  x_in1 = tree["x_in1"].array(library="np")
  y_in1 = tree["y_in1"].array(library="np")
  id_in1 = tree["id_in1"].array(library="np")

  x_out1 = tree["x_out1"].array(library="np")
  y_out1 = tree["y_out1"].array(library="np")
  id_out1 = tree["id_out1"].array(library="np")

  # Capa 2 (Plano final)
  x_in2 = tree["x_in2"].array(library="np")
  y_in2 = tree["y_in2"].array(library="np")
  id_in2 = tree["id_in2"].array(library="np")

  x_out2 = tree["x_out2"].array(library="np")
  y_out2 = tree["y_out2"].array(library="np")
  id_out2 = tree["id_out2"].array(library="np")

  # Filtrar centinelas (-999 o ID < 0) para la capa final out2
  valid_mask = (id_out2 >= 0) & (x_out2 > -900) & (y_out2 > -900)
  x_val = x_out2[valid_mask]
  y_val = y_out2[valid_mask]

  print(f"Total de eventos en el árbol: {len(x_out2)}")
  print(f"Entradas válidas en la capa final (out2): {len(x_val)}")

  if len(x_val) > 0:
    # --- 1. Gráfico de Densidad Espacial (Plano XY - out2) ---
    plt.figure(figsize=(9, 8))
    counts, xedges, yedges, im = plt.hist2d(
        x_val, y_val, bins=150, cmap="turbo"
    )

    plt.colorbar(im, label="Número de impactos")
    plt.title(
        "Densidad Espacial en la Capa Final (PMDetectorConstruction - out2)",
        fontsize=14,
    )
    plt.xlabel("X_out2 (mm)", fontsize=12)
    plt.ylabel("Y_out2 (mm)", fontsize=12)
    plt.grid(True, linestyle="--", alpha=0.4)
    plt.axis("equal")

    output_map = "mapa_impactos_capa_final.png"
    plt.savefig(output_map, dpi=300, bbox_inches="tight")
    plt.show()
    print(f"Mapa de impactos guardado como '{output_map}'.")

    # --- 2. Perfiles Proyectados (1D histograms para X e Y) ---
    plt.figure(figsize=(12, 5))

    plt.subplot(1, 2, 1)
    plt.hist(x_val, bins=100, color="royalblue", edgecolor="black", alpha=0.7)
    plt.title("Proyección en X (Capa Final)", fontsize=12)
    plt.xlabel("X_out2 (mm)", fontsize=11)
    plt.ylabel("Eventos", fontsize=11)
    plt.grid(True, linestyle="--", alpha=0.4)

    plt.subplot(1, 2, 2)
    plt.hist(y_val, bins=100, color="darkorange", edgecolor="black", alpha=0.7)
    plt.title("Proyección en Y (Capa Final)", fontsize=12)
    plt.xlabel("Y_out2 (mm)", fontsize=11)
    plt.ylabel("Eventos", fontsize=11)
    plt.grid(True, linestyle="--", alpha=0.4)

    plt.tight_layout()
    output_projections = "perfiles_proyeccion.png"
    plt.savefig(output_projections, dpi=300, bbox_inches="tight")
    plt.show()
    print(f"Perfiles proyectados guardados como '{output_projections}'.")

  else:
    print(
        "[ALERTA]: No se encontraron eventos válidos en 'out2'. Comprueba la"
        " geometría del PMDetectorConstruction o los cortes de energía."
    )