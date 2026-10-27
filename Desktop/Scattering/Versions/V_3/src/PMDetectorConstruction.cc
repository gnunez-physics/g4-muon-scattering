#include "PMDetectorConstruction.hh"

// Programamos el constructor: se ejecuta cuando se crea la geometría del detector.
PMDetectorConstruction::PMDetectorConstruction()
{
}

// Programamos el destructor: se ejecuta al borrar la geometría. Por ahora está vacío.
PMDetectorConstruction::~PMDetectorConstruction()
{
}

// Esta es la función principal donde construimos todo el escenario del experimento.
G4VPhysicalVolume *PMDetectorConstruction::Construct()
{

	// Llamamos al gestor oficial de materiales (NIST) para poder usar los 
	// elementos y compuestos químicos ya creados en el sistema.
	G4NistManager *nist  = G4NistManager::Instance();

	//--------------------------------
	// 1. UNIVERSO (MUNDO)
	//--------------------------------

	// Buscamos y asignamos el material para el universo, en este caso, aire.
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
	// Definimos las dimensiones totales que queremos para nuestra caja del universo.
	G4double xWorld = 6. * m;
    G4double yWorld = 6. * m;
	G4double zWorld = 6. * m;

	// Creamos la forma geométrica básica del universo (una caja o cubo).
	G4Box *solidWorld = new G4Box(
		"solidWorld",
		0.5 * xWorld, 
		0.5 * yWorld, 
		0.5 * zWorld
	);

	// Creamos el "volumen lógico" asignándole la forma y el material de aire.
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");
	
	// Creamos el "volumen físico" para colocarlo en el centro exacto del espacio (0,0,0).
	G4VPhysicalVolume *physWorld = new G4PVPlacement(
    		0,                            // 1. Rotación
    		G4ThreeVector(0., 0., 0.),        // 2. Posición (X, Y, Z)
    		logicWorld,                       // 3. El volumen lógico que vamos a colocar
    		"physWorld",                      // 4. Nombre interno del volumen físico
    		0,                                // 5. El volumen "madre" (Padre)
    		false,                            // 6. Copias
    		0,                                // 7. Número de copia exacto
    		true                              // 8. Revisión de superposiciones
	);

	//-------------------------------------------------------------------------
	// 2. SELECTOR DE MATERIALES PARA LA CAJA CENTRAL
	//-------------------------------------------------------------------------
	// Opciones principales del NIST disponibles para usar:
	// - "G4_WATER" (Agua pura)
	// - "G4_Cu"    (Cobre)
	// - "G4_Pb"    (Plomo)
	// - "G4_Au"    (Oro)
	// - "G4_Ag"    (Plata)
	// - "G4_Al"    (Aluminio)
	// - "G4_Fe"    (Hierro)
	// - "G4_C"     (Grafito / Carbono)
	//-------------------------------------------------------------------------
	
	G4String selectedMaterial = "G4_WATER"; // <--- CAMBIA EL MATERIAL AQUÍ FÁCILMENTE

	// Buscamos el material seleccionado en la base de datos del NIST
	G4Material *boxMaterial = nist->FindOrBuildMaterial(selectedMaterial);


	//--------------------------------
	// 3. CREACIÓN DE LA CAJA CENTRAL
	//--------------------------------	
	
		// Creamos la geometría de la caja (G4Box) con sus semidimensiones (Half-lengths)
		G4Box *solidBox = new G4Box(
			"solidBox",
			1. * m,         // Mitad de la longitud en X (Ancho total = 2 m)
			1. * m,         // Mitad de la longitud en Y (Alto total = 2 m)
			0.5 * m         // Mitad de la longitud en Z (Profundidad total = 1 m)
		);

		// Creamos el volumen lógico asignándole la forma y el material dinámico
		G4LogicalVolume *logicBox = new G4LogicalVolume(solidBox, boxMaterial, "logicBox");

		// Creamos el volumen físico para colocarlo en el centro del mundo
		G4VPhysicalVolume *physBox = new G4PVPlacement(
			0,                        // 1. Sin rotación
			G4ThreeVector(0., 0., 0.), // 2. Posición central (X, Y, Z)
			logicBox,                 // 3. El volumen lógico
			"physBox",                // 4. Nombre interno del volumen físico
			logicWorld,               // 5. Volumen madre (dentro del aire del mundo)
			false,                    // 6. Copias
			0,                        // 7. Número de copia
			true                      // 8. Revisión de solapes
		);

	// =====================================================================
	// 2. MATRICES DE DETECTORES (REJILLAS DE ENTRADA Y SALIDA)
	// =====================================================================
	G4int gridRows = 50; 
	G4int gridCols = 50;
	
	G4double totalGridSizeX = 2. * m; 
	G4double totalGridSizeY = 2. * m; 
	
	G4double sizeX = totalGridSizeX / gridRows;
	G4double sizeY = totalGridSizeY / gridCols;
	G4double thicknessZ = 0.001 * m; 

	G4Box *solidDetector = new G4Box("solidDetector", 0.5 * sizeX, 0.5 * sizeY, 0.5 * thicknessZ);

	// Creamos DOS volúmenes lógicos separados para distinguirlos perfectamente
	logicDetectorEntry = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorEntry");
	logicDetectorExit  = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorExit");

	// ----------------------------------------------------
	// A. REJILLA DE ENTRADA (En la cara frontal, Z = +0.5 m)
	// ----------------------------------------------------
	G4VPhysicalVolume *physDetectorEntry = nullptr;
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) 
		{ 
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			physDetectorEntry = new G4PVPlacement(
				0, 
				G4ThreeVector(posX, posY, +0.5 * m + 0.5 * thicknessZ), 
				logicDetectorEntry, 
				"physDetectorEntry", 
				logicWorld, 
				false, 
				j + i * gridCols, 
				true
			);
		}
	}

	// ----------------------------------------------------
	// B. REJILLA DE SALIDA (En la cara trasera, Z = -0.5 m)
	// ----------------------------------------------------
	G4VPhysicalVolume *physDetectorExit = nullptr;
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) 
		{ 
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			physDetectorExit = new G4PVPlacement(
				0, 
				G4ThreeVector(posX, posY, -0.5 * m - 0.5 * thicknessZ), 
				logicDetectorExit, 
				"physDetectorExit", 
				logicWorld, 
				false, 
				1000 + (j + i * gridCols), 
				true
			);
		}
	}

	return physWorld;
}

// =====================================================================
// DEFINICIÓN DE LOS DETECTORES SENSIBLES
// =====================================================================
void PMDetectorConstruction::ConstructSDandField()
{
    // Creamos instancias independientes para la entrada y la salida
    MySensitiveDetector *sensDetEntry = new MySensitiveDetector("EntryDetector");
    MySensitiveDetector *sensDetExit  = new MySensitiveDetector("ExitDetector");
    
    // Se los asignamos formalmente a cada volumen lógico
    logicDetectorEntry->SetSensitiveDetector(sensDetEntry);
    logicDetectorExit->SetSensitiveDetector(sensDetExit);
}
