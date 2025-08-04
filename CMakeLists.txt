cmake_minimum_required(VERSION 3.10)
project(ProjetSDL C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Détection des fichiers source ===
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

message(STATUS "Sources trouvés :")
foreach(source_file ${SOURCES})
    message(STATUS "  ${source_file}")
endforeach()

# === Détection dynamique des dossiers d'en-tête ===
file(GLOB_RECURSE HEADER_PATHS 
    "${CMAKE_SOURCE_DIR}/include/*.h" 
    "${CMAKE_SOURCE_DIR}/src/**/*.h"
)

set(INCLUDE_DIRS "")
foreach(header ${HEADER_PATHS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# === Ajout des includes automatiquement trouvés ===
include_directories(${INCLUDE_DIRS})

# === Dépendances SDL ===
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

# === Création de l'exécutable ===
add_executable(main ${SOURCES})

# === Link des bibliothèques ===
target_link_libraries(main ${SDL2_LIBRARIES} SDL2_ttf)

# === Force la recompilation si les headers changent ===
file(GLOB_RECURSE ALL_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/src/**/*.h")
set_source_files_properties(${ALL_HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)