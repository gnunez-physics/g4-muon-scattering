import uproot

with uproot.open("1/muon_mcs_results.root") as archivo:
    tree = archivo["MuonData"]
    
    # Extraer las primeras 5 entradas de cada rama relevante
    x_in1 = tree['x_in1'].array(library="np")[:5]
    x_in2 = tree['x_in2'].array(library="np")[:5]
    z_in1 = tree['z_in1'].array(library="np")[:5]
    z_in2 = tree['z_in2'].array(library="np")[:5]
    
    print("x_in1:", x_in1)
    print("x_in2:", x_in2)
    print("z_in1:", z_in1)
    print("z_in2:", z_in2)