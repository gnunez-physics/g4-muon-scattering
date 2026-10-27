// Creamos el archivo de encabezado (.hh) protegido con un Include Guard 
// para evitar errores si el sistema intenta leer este archivo varias veces.
#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

// Archivos de encabezado necesarios:
// 1. La clase base oficial de Geant4 para inicializar las acciones del usuario.
#include "G4VUserActionInitialization.hh"
// 2. Nuestro generador de partículas para que la clase sepa qué va a instanciar.
#include "PMPrimaryGenerator.hh"

// Definimos nuestra clase heredando de la clase oficial de Geant4.
class PMActionInitialization : public G4VUserActionInitialization
{
public:
    // Constructor y destructor de la clase.
    PMActionInitialization();
    ~PMActionInitialization();

    // BuildForMaster: Se ejecuta exclusivamente en el hilo principal ("Master") 
    // cuando configuras la simulación para correr en modo paralelo (multihilo).
    virtual void BuildForMaster() const;

    // Build: Se ejecuta en cada hilo de trabajo (o en el único hilo si es secuencial). 
    // Es la función clave donde registramos las acciones del usuario, 
    // como instanciar nuestro generador de partículas.
    virtual void Build() const;
};

#endif // Fin del Include Guard