# ==============================================================================
# ARCHIVO DE CONFIGURACIÓN DE CMAKE PARA EL PROYECTO CHERENKOV
# ==============================================================================
# Este archivo es el "plano de construcción" que le indica al compilador 
# cómo debe unir todos los archivos de código para crear el programa final.

# 1. Versión mínima requerida de CMake para que el sistema no falle.
cmake_minimum_required(VERSION 4.4.3 FATAL_ERROR)

# 2. Le damos un nombre oficial a nuestro proyecto de simulación.
project(Cherenkov)

# 3. Buscamos e integramos Geant4 en nuestro proyecto. 
# La palabra "REQUIRED" es obligatoria: si Geant4 no está instalado o no se encuentra, 
# la compilación se detiene de inmediato.
# En la segunda versión añadimos ui_all y vis_all para la visualización del usuario
find_package(Geant4 REQUIRED)

# 4. Configuramos el entorno de Geant4 para que nuestro compilador 
# entienda todos sus comandos y herramientas internas.
include(${Geant4_USE_FILE})

# 5. Le indicamos a CMake dónde buscar nuestros archivos de encabezado (.hh).
# Los organizaremos dentro de una carpeta llamada "include".
include_directories(${PROJECT_SOURCE_DIR}/include)



# Automatizamos la copia de los archivos de macros (.mac) desde la carpeta 
# de código fuente hacia la carpeta de compilación (${CMAKE_BINARY_DIR}). 
# Esto es indispensable para que, al ejecutar el programa compilado, 
# Geant4 pueda encontrar y leer archivos como "vis.mac" sin problemas.
file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)



# 6. Automatizamos la búsqueda de archivos fuente (.cc).
# La orden "GLOB" busca automáticamente todos los archivos terminados en .cc 
# que estén dentro de una carpeta llamada "src", para no tener que escribirlos uno por uno.
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# 7. Creamos el archivo ejecutable de nuestro programa.
# Llamaremos al programa "muon" y le entregamos el archivo principal (muon.cc) 
# junto con todos los archivos de código fuente que encontró el paso anterior (${sources}).
add_executable(muon muon.cc ${sources})

# 8. Conectamos (enlazamos) nuestro programa ejecutable con todas las 
# librerías matemáticas y físicas oficiales de Geant4 para que pueda funcionar.
target_link_libraries(muon ${Geant4_LIBRARIES})

# 9. Creamos un acceso directo o tarea personalizada para que el proyecto completo 
# Así, el proyecto completo se compilará simplemente escribiendo "Cherenkov".
add_custom_target(Cherenkov DEPENDS muon)
