#ifndef PMPRIMARYGENERATOR_HH
#define PMPRIMARYGENERATOR_HH

// Archivos de encabezado necesarios de Geant4 para configurar la fuente de partículas.
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"     // Herramienta principal para disparar las partículas.
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"

// Definimos nuestra clase para el generador primario, heredando de la clase oficial de Geant4.
class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    // Constructor y destructor de la clase.
    PMPrimaryGenerator();
    ~PMPrimaryGenerator();
    
    // Sobrescribimos esta función obligatoria. Es la encargada de disparar 
    // las partículas al inicio de cada evento de la simulación.
    virtual void GeneratePrimaries(G4Event *);

private:
    // Puntero al cañón de partículas que utilizaremos para configurar 
    // qué tipo de partícula sale, con qué energía y hacia dónde apunta.
    G4ParticleGun *fParticleGun;
};

#endif
