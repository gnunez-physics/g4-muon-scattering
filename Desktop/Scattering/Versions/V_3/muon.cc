// Incluimos la librería estándar de C++ para poder mostrar mensajes en la consola.
#include <iostream>

// Archivos de Geant4 necesarios para hacer funcionar la simulación:

// Llamamos a este archivo para usar el RunManager, que actúa como el corazón de Geant4: 
// gestiona la física, la geometría y las partículas.
#include "G4RunManager.hh"

// Llamamos a este archivo para usar una versión especial del administrador que aprovecha 
// varios núcleos de la computadora a la vez (multihilo) y hace que el programa corra más rápido.
#include "G4MTRunManager.hh"

// Llamamos a este archivo para usar el UImanager, que es el encargado de manejar 
// los menús y comandos de texto en Geant4.
#include "G4UImanager.hh"

// Llamamos a este archivo para usar el VisManager, el administrador general 
// que enciende el sistema de gráficos y dibujos.
#include "G4VisManager.hh"

// Llamamos a estos archivos (VisExecutive y UIExecutive) para obtener las herramientas 
// específicas que abren la ventana visual y la interfaz interactiva en tu computadora.
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// Llamamos a nuestro propio archivo de encabezado ("PMPhysicsList.hh") para que el programa 
// reconozca la lista de física personalizada que creamos para la segunda versión.
#include "PMPhysicsList.hh"

// Incluimos nuestro propio archivo de encabezado (.hh). 
// Esto es indispensable para que este archivo (.cc) conozca la estructura, 
// las clases y las reglas del detector que definimos previamente en los planos.
#include "PMDetectorConstruction.hh"


#include "PMActionInitialization.hh"


/**
 * @brief Función principal (punto de entrada del programa).
 * 
 * Recibe argumentos de la consola (argc y argv). Esto permite, por ejemplo, 
 * cargar un archivo "macro" (un bloc de notas con una lista de comandos ordenados 
 * que Geant4 lee y ejecuta automáticamente línea por línea).
 */
int main(int argc, char** argv)
{
	// 1. Creamos la interfaz gráfica para que se abra la ventana de usuario.
	G4UIExecutive *ui = new G4UIExecutive(argc, argv);

	// 2. Preparamos el RunManager. 
	// El sistema detecta automáticamente si tu computadora puede usar varios hilos (multihilo) 
	// y activa el administrador correspondiente.
	#ifdef G4MULTITHREADED
		G4MTRunManager *runManager = new G4MTRunManager;
	#else
		G4RunManager *runManager = new G4RunManager;
	#endif

	// Asignamos nuestra lista de física personalizada al administrador de la simulación.
	runManager->SetUserInitialization(new PMPhysicsList());
	
	// Le indicamos al gestor principal (RunManager) que inicialice y utilice 
	// nuestro diseño de geometría personalizado (PMDetectorConstruction) 
	// para que la simulación sepa qué objetos y escenarios debe dibujar.
	runManager->SetUserInitialization(new PMDetectorConstruction());
	
	// Le entregamos y registramos formalmente nuestra clase de inicialización de acciones 
	// (PMActionInitialization) en el administrador general (RunManager), para que Geant4 
	// sepa que debe activar el generador de partículas y las reglas de disparo en cada 		evento.
	runManager->SetUserInitialization(new PMActionInitialization());




	// 3. Encendemos el sistema de visualización de gráficos.
	// Esto prepara todo para poder dibujar las trayectorias de los muones, 
	// el tanque y los detectores en pantalla. La línea Initialize() lo pone en marcha.
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();

	// 4. Obtenemos el canal de comandos global.
	// Este sistema es el que procesa todas las órdenes basadas en texto dentro de Geant4.
	G4UImanager *UImanager = G4UImanager::GetUIpointer();



	// Ejecutamos el archivo de comandos "vis.mac" a través del gestor de la interfaz. 
	// Esto le ordena a Geant4 leer todas las instrucciones gráficas que preparamos 
	// (abrir la ventana OpenGL, ajustar la cámara, dibujar volúmenes, ejes y trayectorias) 
	// para desplegar la interfaz visual de la simulación en pantalla.
	UImanager->ApplyCommand("/control/execute vis.mac");
	
	



	// 5. Arrancamos la sesión interactiva.
	// En este punto se lanza la ventana gráfica y el usuario puede empezar a interactuar.
	ui->SessionStart();

	// Cuando cierras la ventana, el programa finaliza devolviendo 0 de forma limpia.
	return 0;
}






