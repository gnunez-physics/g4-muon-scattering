// Archivo de encabezado para construir el detector y el entorno de la simulación.
// Las siguientes líneas evitan que este archivo se cargue dos veces por error.
#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH

// Llamamos a este archivo para usar la clase base de Geant4 que nos permite 
// crear y definir la geometría del detector.
#include "G4VUserDetectorConstruction.hh"

// Llamamos a este archivo para crear formas de caja (como el universo o "World") 
// usando cubos o rectángulos.
#include "G4Box.hh"

// Llamamos a este archivo para poder crear formas de tubo o cilindro.
#include "G4Tubs.hh"

#include "G4Box.hh"

// Llamamos a este archivo para usar el "Volumen Lógico", que define las propiedades 
// de una figura (como su material y tamaño, sin importar dónde esté colocada).
#include "G4LogicalVolume.hh"

// Llamamos a este archivo para usar el "Volumen Físico", que sirve para colocar 
// y orientar nuestros objetos dentro del espacio tridimensional de la simulación.
#include "G4VPhysicalVolume.hh"

// Llamamos a este archivo para usar la función que coloca físicamente los volúmenes 
// dentro de otros (por ejemplo, poner el detector dentro del universo).
#include "G4PVPlacement.hh"

// Llamamos a este archivo para definir de qué material están hechos los objetos 
// (agua, aire, plomo, etc.).
#include "G4Material.hh"

// Llamamos a este archivo para usar el administrador NIST, una base de datos oficial 
// que ya tiene listos los elementos químicos, compuestos y materiales más comunes.
#include "G4NistManager.hh"

// Llamamos a este archivo para usar el sistema de unidades de Geant4 
// (para medir en milímetros, nanosegundos, MeV, etc.).
#include "G4SystemOfUnits.hh"

// Llamamos a este archivo para usar la tabla de unidades y mostrar resultados 
// con sus medidas correctas en pantalla.
#include "G4UnitsTable.hh"

#include "PMMySensitiveDetector.hh"

// Creamos nuestra clase para construir el detector. Una clase funciona como un plano 
// o un molde detallado para fabricar algo en el programa.
class PMDetectorConstruction : public G4VUserDetectorConstruction
{
// Definimos nuestro constructor como público para que otras partes del proyecto 
// puedan leerlo y usarlo.
public:
	PMDetectorConstruction();
	
	// Declaramos el destructor. Debe ser virtual porque viene heredado y adaptado 
	// de la clase principal de Geant4.
	virtual ~PMDetectorConstruction();
	
	// Esta es la función principal encargada de construir toda la geometría. 
	// Es virtual porque debemos sobrescribirla con nuestro propio diseño.
	virtual G4VPhysicalVolume *Construct();
	
private:
    G4LogicalVolume *logicDetectorEntry;
    G4LogicalVolume *logicDetectorExit;
    virtual void ConstructSDandField();

};

#endif
