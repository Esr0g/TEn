cmake_minimum_required(VERSION 3.16)
project(TEn LANGUAGES CXX)

# C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optimisation
add_compile_options(-O2)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
    *.cpp
    *.hpp
)
list(REMOVE_ITEM SOURCES "${CMAKE_BINARY_DIR}/CMakeFiles/3.25.1/CompilerIdCXX/CMakeCXXCompilerId.cpp")


# Création de l'exécutable
add_executable(TEn ${SOURCES})

# Libs à l'édition de lien
target_link_libraries(TEn
    glfw
    vulkan
    dl
    pthread
    X11
    Xxf86vm
    Xrandr
    Xi
)

# Inclure les headers de tous les dossiers (si tu ne veux pas tout en "src/")
# Ajoute tous les dossiers contenant des headers (récursivement)
file(GLOB_RECURSE HEADER_DIRS CONFIGURE_DEPENDS src/*.hpp)
foreach(header ${HEADER_DIRS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND ALL_INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)
target_include_directories(TEn PRIVATE ${ALL_INCLUDE_DIRS})

#################################### SHADERS ###########################################

# Chercher tous les fichiers .spv dans shaders/
file(GLOB_RECURSE SPV_SHADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/shaders/*.spv")

# Répertoire de destination dans le dossier de build
set(SPV_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/shaders")

# Cible pour copier les shaders
add_custom_target(CopyShaders ALL
    COMMENT "Copying .spv shader files to build directory"
)

# Créer le dossier de destination
add_custom_command(
    TARGET CopyShaders PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SPV_DESTINATION}
    COMMENT "Creating shader output directory"
)

# Copier chaque shader, en écrasant les fichiers existants
foreach(SHADER_FILE ${SPV_SHADERS})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    
    add_custom_command(
        TARGET CopyShaders PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SHADER_FILE}
        ${SPV_DESTINATION}/${FILE_NAME}
        COMMENT "Copying ${FILE_NAME}"
    )
endforeach()

# Lier la copie des shaders à ton exécutable
add_dependencies(TEn CopyShaders)
