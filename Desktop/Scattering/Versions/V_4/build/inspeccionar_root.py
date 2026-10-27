import uproot

with uproot.open("muon_mcs_results.root") as file:
    print("Claves en el archivo ROOT:", file.keys())
    tree = file["MuonData"]
    tree.show()  # Muestra las columnas, tipos de datos y número de entradas