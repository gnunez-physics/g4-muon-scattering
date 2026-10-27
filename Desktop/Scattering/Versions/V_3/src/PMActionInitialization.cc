#include "PMActionInitialization.hh"
#include "PMPrimaryGenerator.hh"

// =========================================================================
// 1. CONSTRUCTOR Y DESTRUCTOR
// =========================================================================
// El constructor se ejecuta al iniciar la clase. Al estar vacío, indica que 
// esta clase actúa puramente como una "fábrica" y no requiere variables previas.
PMActionInitialization::PMActionInitialization()
{}

// El destructor libera los recursos al cerrar, gestionado de forma estándar por C++.
PMActionInitialization::~PMActionInitialization()
{}


// =========================================================================
// 2. CONFIGURACIÓN MULTIHILO (MASTER)
// =========================================================================
// BuildForMaster(): Se ejecuta EXCLUSIVAMENTE en el hilo principal ("Master") 
// cuando ejecutas simulaciones en paralelo. Al ser un generador sencillo, se deja vacío.
void PMActionInitialization::BuildForMaster() const
{}


// =========================================================================
// 3. LA FUNCIÓN CLAVE: BUILD (EL PUENTE DE ACCIÓN CON GEANT4)
// =========================================================================
// Build(): Se ejecuta en cada hilo de trabajo (o en el hilo único si corres 
// en modo secuencial). Es aquí donde conectamos nuestra lógica con el motor Geant4.
void PMActionInitialization::Build() const
{
    // PASO A: Creamos físicamente en la memoria RAM una nueva instancia de tu 
    // cañón de partículas personalizado que programamos previamente.
    PMPrimaryGenerator *generator = new PMPrimaryGenerator();
    
    // PASO B: Conectamos el generador con Geant4. 
    // La función "SetUserAction(generator)" es la orden formal que le dice al motor:
    // "Toma este objeto 'generator' y utilízalo oficialmente; cada vez que 
    // comience un nuevo evento en la simulación, ejecuta sus reglas de disparo".
    SetUserAction(generator);
}
