#include "PMPrimaryGenerator.hh"

// Programamos el constructor: aquí inicializamos el cañón de partículas y 
// configuramos sus valores por defecto (posición, dirección, tipo de partícula y energía).
PMPrimaryGenerator::PMPrimaryGenerator()
{
	// Creamos el objeto del cañón. El número (1) indica que se disparará 
	// exactamente 1 partícula por cada evento de la simulación.
    	fParticleGun = new G4ParticleGun(1);

    	// Definimos la posición inicial (coordenadas X, Y, Z) donde aparecerá la partícula.
    	G4double x = -0.5 * m;
    	G4double y = -0.5 * m;
    	G4double z = 3 * m;
    	G4ThreeVector pos(x, y, z);

    	// Definimos la dirección de movimiento (momentum). 
    	G4double px = 0.2;
    	G4double py = 0.2;
    	G4double pz = -1.; 
    	G4ThreeVector mom(px, py, pz);
	
	// Llamamos a la tabla oficial de Geant4 para buscar el tipo de partícula que usaremos.
    	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	
	// Seleccionamos específicamente la partícula para la simulación.
	G4ParticleDefinition *particle = particleTable->FindParticle("mu-");

	// Empaquetamos todas las configuraciones anteriores y se las entregamos al cañón:
	fParticleGun->SetParticlePosition(pos);
    	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleEnergy(.1 * GeV); 
	fParticleGun->SetParticleDefinition(particle);
}

// Programamos el destructor: se encarga de liberar y limpiar la memoria RAM
PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

// Esta función se ejecuta automáticamente en cada "evento" (cada disparo de la simulación).
void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}