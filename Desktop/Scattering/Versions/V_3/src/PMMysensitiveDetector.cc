#include "PMMySensitiveDetector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
}

MySensitiveDetector::~MySensitiveDetector()
{
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // Aquí es donde escribiremos la lógica para extraer las coordenadas,
    // el número de copia de la rejilla y el momento del muón.

    return true; // ¡Obligatorio para que Geant4 no devuelva un error!
}