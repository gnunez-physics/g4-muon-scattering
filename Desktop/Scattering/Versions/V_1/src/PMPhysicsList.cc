// Llamamos a nuestro propio archivo de encabezado ("PMPhysicsList.hh") para que el programa 
// reconozca la clase y las funciones que vamos a programar a continuación.
#include "PMPhysicsList.hh"

// Aquí programamos el constructor: se ejecuta al crear la lista de física.
PMPhysicsList::PMPhysicsList()
{
	// Activamos los procesos físicos estándar para las partículas (interacciones electromagnéticas).
	RegisterPhysics(new G4EmStandardPhysics());

	RegisterPhysics(new G4DecayPhysics());
 
}

// Aquí programamos el destructor: se ejecuta al borrar la lista. Por ahora está vacío porque no necesitamos liberar memoria manualmente.
PMPhysicsList::~PMPhysicsList()
{
}
