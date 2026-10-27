#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();

	// 1. UNIVERSO (MUNDO)
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
	G4double xWorld = 6. * m;
	G4double yWorld = 6. * m;
	G4double zWorld = 6. * m;

	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(
		0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true
	);

	// =====================================================================
	// 2. SELECTOR DE MATERIALES PARA LA CAJA CENTRAL
	// =====================================================================
	// Descomenta el material que desees probar:
	// G4String selectedMaterial = "G4_WATER";          // Agua (X0 ≈ 361.0 mm)
	// G4String selectedMaterial = "G4_Al";              // Aluminio (X0 ≈ 88.9 mm)
	// G4String selectedMaterial = "G4_Fe";              // Hierro (X0 ≈ 17.57 mm)
	// G4String selectedMaterial = "G4_Cu";              // Cobre (X0 ≈ 14.36 mm)
	// G4String selectedMaterial = "G4_Pb";              // Plomo (X0 ≈ 5.61 mm)
	G4String selectedMaterial = "G4_Au";              // Oro (X0 ≈ 3.34 mm)

	G4Material *boxMaterial = nist->FindOrBuildMaterial(selectedMaterial);

	// 3. CAJA CENTRAL (Espesor ancho: 1.0 m total -> semilongitud Z = 0.5 m)
	G4double halfLenZ = 0.05 * m; 
	G4Box *solidBox = new G4Box("solidBox", 1. * m, 1. * m, halfLenZ);
	G4LogicalVolume *logicBox = new G4LogicalVolume(solidBox, boxMaterial, "logicBox");

	G4VPhysicalVolume *physBox = new G4PVPlacement(
		0, G4ThreeVector(0., 0., 0.), logicBox, "physBox", logicWorld, false, 0, true
	);

	// =====================================================================
	// 4. MATRICES DE DETECTORES (4 PLANOS EN TOTAL: 2 DE ENTRADA, 2 DE SALIDA)
	// =====================================================================
	G4int gridRows = 50; 
	G4int gridCols = 50;
	G4double totalGridSizeX = 2. * m; 
	G4double totalGridSizeY = 2. * m; 
	G4double sizeX = totalGridSizeX / gridRows;
	G4double sizeY = totalGridSizeY / gridCols;
	G4double thicknessZ = 0.001 * m; 

	G4Box *solidDetector = new G4Box("solidDetector", 0.5 * sizeX, 0.5 * sizeY, 0.5 * thicknessZ);

	// Volúmenes lógicos para los 4 planos
	logicDetectorEntry1 = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorEntry1"); // Cerca del bloque
	logicDetectorEntry2 = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorEntry2"); // Más lejos (10 cm más arriba)
	
	logicDetectorExit1  = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorExit1");   // Cerca del bloque
	logicDetectorExit2  = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorExit2");   // Más lejos (10 cm más abajo)

	// Distancias de separación adicionales respecto a las caras del cubo
	G4double gap1 = 0.10 * m; // 10 cm más separado que antes
	G4double gap2 = 0.20 * m; // 20 cm más separado (10 cm más arriba del plano 1)

	// ----------------------------------------------------
	// A1. REJILLA DE ENTRADA 1 (Z = +halfLenZ + 10 cm)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, +halfLenZ + gap1 + 0.5 * thicknessZ),
				logicDetectorEntry1, "physDetectorEntry1", logicWorld, false, j + i * gridCols, true
			);
		}
	}

	// ----------------------------------------------------
	// A2. REJILLA DE ENTRADA 2 (Z = +halfLenZ + 20 cm)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, +halfLenZ + gap2 + 0.5 * thicknessZ),
				logicDetectorEntry2, "physDetectorEntry2", logicWorld, false, 2000 + (j + i * gridCols), true
			);
		}
	}

	// ----------------------------------------------------
	// B1. REJILLA DE SALIDA 1 (Z = -halfLenZ - 10 cm)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, -halfLenZ - gap1 - 0.5 * thicknessZ),
				logicDetectorExit1, "physDetectorExit1", logicWorld, false, 1000 + (j + i * gridCols), true
			);
		}
	}

	// ----------------------------------------------------
	// B2. REJILLA DE SALIDA 2 (Z = -halfLenZ - 20 cm)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, -halfLenZ - gap2 - 0.5 * thicknessZ),
				logicDetectorExit2, "physDetectorExit2", logicWorld, false, 3000 + (j + i * gridCols), true
			);
		}
	}

	return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDetEntry1 = new MySensitiveDetector("EntryDetector1");
    MySensitiveDetector *sensDetEntry2 = new MySensitiveDetector("EntryDetector2");
    MySensitiveDetector *sensDetExit1  = new MySensitiveDetector("ExitDetector1");
    MySensitiveDetector *sensDetExit2  = new MySensitiveDetector("ExitDetector2");
    
    logicDetectorEntry1->SetSensitiveDetector(sensDetEntry1);
    logicDetectorEntry2->SetSensitiveDetector(sensDetEntry2);
    logicDetectorExit1->SetSensitiveDetector(sensDetExit1);
    logicDetectorExit2->SetSensitiveDetector(sensDetExit2);
}