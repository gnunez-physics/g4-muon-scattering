#include "PMDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "PMMySensitiveDetector.hh"

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
	
	G4double xWorld = 11. * m;
	G4double yWorld = 11. * m;
	G4double zWorld = 11. * m;

	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(
		0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true
	);

	// =====================================================================
	// 2. SELECTOR DE MATERIALES PARA LA CAJA CENTRAL
	// =====================================================================
	// G4String selectedMaterial = "G4_WATER";        // Agua (X0 ≈ 361.0 mm)
	// G4String selectedMaterial = "G4_Al";            // Aluminio (X0 ≈ 88.9 mm)
	// G4String selectedMaterial = "G4_Fe";            // Hierro (X0 ≈ 17.57 mm)
	// G4String selectedMaterial = "G4_Cu";            // Cobre (X0 ≈ 14.36 mm)
	// G4String selectedMaterial = "G4_Pb";            // Plomo (X0 ≈ 5.61 mm)
	G4String selectedMaterial = "G4_Au";               // Oro real (X0 ≈ 3.34 mm)

	G4Material *boxMaterial = nist->FindOrBuildMaterial(selectedMaterial);

	// 3. CAJA CENTRAL - Ahora más delgada (Espesor total en Z = 5 cm -> semiancho = 2.5 cm)
	G4double halfSizeX = 1.0 * m;  // Ancho transversal mantenido para cubrir el área
	G4double halfSizeY = 1.0 * m;  
	G4double halfLenZ  = 0.025 * m; // 2.5 cm de semiancho -> 50 mm de espesor total (t = 50 mm)

	G4Box *solidBox = new G4Box("solidBox", halfSizeX, halfSizeY, halfLenZ);
	G4LogicalVolume *logicBox = new G4LogicalVolume(solidBox, boxMaterial, "logicBox");

	// Colocado exactamente en el MEDIO (Z = 0.0)
	G4double zPosBox = 0.0 * mm; 

	G4VPhysicalVolume *physBox = new G4PVPlacement(
		0, G4ThreeVector(0., 0., zPosBox), logicBox, "physBox", logicWorld, false, 0, true
	);

	// =====================================================================
	// 4. MATRICES DE DETECTORES (4 PLANOS DISTRIBUIDOS SIMÉTRICAMENTE)
	// =====================================================================
	G4int gridRows = 50; 
	G4int gridCols = 50;
	G4double totalGridSizeX = 2.0 * m; 
	G4double totalGridSizeY = 2.0 * m; 
	G4double sizeX = totalGridSizeX / gridRows;
	G4double sizeY = totalGridSizeY / gridCols;
	G4double thicknessZ = 0.001 * m; 

	G4Box *solidDetector = new G4Box("solidDetector", 0.5 * sizeX, 0.5 * sizeY, 0.5 * thicknessZ);

	// Volúmenes lógicos para los 4 planos
	logicDetectorEntry1 = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorEntry1"); 
	logicDetectorEntry2 = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorEntry2"); 
	
	logicDetectorExit1  = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorExit1");   
	logicDetectorExit2  = new G4LogicalVolume(solidDetector, worldMat, "logicDetectorExit2");   

	// Posiciones Z ajustadas cercanamente al nuevo bloque delgado (que va de -25 mm a +25 mm)
	G4double zDetEntry1 = +0.20 * m; // 20 cm
	G4double zDetEntry2 = +0.15 * m; // 15 cm
	G4double zDetExit1  = -0.15 * m; // -15 cm
	G4double zDetExit2  = -0.20 * m; // -20 cm

	// ----------------------------------------------------
	// A1. REJILLA DE ENTRADA 1 (Arriba 1)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, zDetEntry1),
				logicDetectorEntry1, "physDetectorEntry1", logicWorld, false, j + i * gridCols, true
			);
		}
	}

	// ----------------------------------------------------
	// A2. REJILLA DE ENTRADA 2 (Arriba 2)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, zDetEntry2),
				logicDetectorEntry2, "physDetectorEntry2", logicWorld, false, 2000 + (j + i * gridCols), true
			);
		}
	}

	// ----------------------------------------------------
	// B1. REJILLA DE SALIDA 1 (Abajo 1)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, zDetExit1),
				logicDetectorExit1, "physDetectorExit1", logicWorld, false, 1000 + (j + i * gridCols), true
			);
		}
	}

	// ----------------------------------------------------
	// B2. REJILLA DE SALIDA 2 (Abajo 2)
	// ----------------------------------------------------
	for (G4int i = 0; i < gridRows; i++) {
		for (G4int j = 0; j < gridCols; j++) {
			G4double posX = -0.5 * totalGridSizeX + (i + 0.5) * sizeX;
			G4double posY = -0.5 * totalGridSizeY + (j + 0.5) * sizeY;

			new G4PVPlacement(
				0, G4ThreeVector(posX, posY, zDetExit2),
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