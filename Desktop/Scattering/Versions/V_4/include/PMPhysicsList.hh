// Este archivo define la lista de física que usará la simulación.
// Las siguientes líneas evitan que este archivo se cargue dos veces por error.
#ifndef PMPHYSICSLISTS_HH
#define PMPHYSICSLISTS_HH

// Archivo base de Geant4 para crear listas de física modulares.
#include "G4VModularPhysicsList.hh"
// Archivo que añade la física estándar para partículas con carga (como electrones y fotones).
#include "G4EmStandardPhysics.hh"

#include "G4DecayPhysics.hh"

// Creamos nuestra propia clase de física basada en la de Geant4.
class PMPhysicsList : public G4VModularPhysicsList
{
// Declaramos el constructor (para crear la física) y el destructor (para borrarla al terminar).
public:
	PMPhysicsList();
	~PMPhysicsList();
};
#endif
