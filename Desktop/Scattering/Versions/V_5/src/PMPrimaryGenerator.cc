#include "PMPrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh" // ¡Esencial para G4UniformRand()!

PMPrimaryGenerator::PMPrimaryGenerator()
{
    // Creamos el cañón para 1 partícula por evento
    fParticleGun = new G4ParticleGun(1);

    // Definimos la partícula fija (Muón negativo) una sola vez
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("mu-");
    fParticleGun->SetParticleDefinition(particle);
    
    // Energía fija para todos los muones (ej. 4 GeV o la que prefieras)
    fParticleGun->SetParticleEnergy(4. * GeV); 
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

// Esta función se ejecuta AUTOMÁTICAMENTE en cada uno de los 10,000 disparos
void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // ==========================================
    // 1. POSICIÓN INICIAL ALEATORIA EN X e Y
    // ==========================================
    G4double maxX = 1.0 * m; 
    G4double maxY = 1.0 * m;

    // G4UniformRand() da un valor entre 0 y 1; lo centramos en [-1m, +1m]
    G4double randomX = (G4UniformRand() - 0.5) * 2.0 * maxX; 
    G4double randomY = (G4UniformRand() - 0.5) * 2.0 * maxY; 
    
    // Posición Z fija arriba del telescopio
    G4double startZ = 2.5 * m; 

    fParticleGun->SetParticlePosition(G4ThreeVector(randomX, randomY, startZ));

    // ==========================================
    // 2. DIRECCIÓN INICIAL ALEATORIA E INCLINADA
    // ==========================================
    // Definimos un ángulo máximo de divergencia (ej. 0.2 rad ≈ 11.5°)
    G4double maxAngle = 0.2; 
    
    G4double dirX = (G4UniformRand() - 0.5) * maxAngle;
    G4double dirY = (G4UniformRand() - 0.5) * maxAngle;
    
    // Dirección principal hacia abajo (-Z)
    G4double dirZ = -1.0; 

    // Creamos el vector de dirección y lo normalizamos
    G4ThreeVector direction(dirX, dirY, dirZ);
    fParticleGun->SetParticleMomentumDirection(direction.unit());

    // Disparar la partícula en el evento actual
    fParticleGun->GeneratePrimaryVertex(anEvent);
}