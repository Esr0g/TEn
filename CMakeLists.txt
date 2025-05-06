cmake_minimum_required(VERSION 3.29)
project(TEn LANGUAGES CXX)

# C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build types and flags only if not already defined (useful for MSVC)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
    *.cpp
    *.hpp
)

# Remove CMake generated files
list(FILTER SOURCES EXCLUDE REGEX ".*CMakeCXXCompilerId.cpp$")


# Création de l'exécutable
add_executable(TEn ${SOURCES})

# Inclure les headers de tous les dossiers (si tu ne veux pas tout en "src/")
# Ajoute tous les dossiers contenant des headers (récursivement)
file(GLOB_RECURSE HEADER_DIRS CONFIGURE_DEPENDS src/*.hpp)
foreach(header ${HEADER_DIRS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND ALL_INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)
target_include_directories(TEn PRIVATE ${ALL_INCLUDE_DIRS})

# Détection de l'OS
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Spécifie les chemins vers Vulkan sur Windows
    set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.4.309.0/Include")
    set(Vulkan_LIBRARY "C:/VulkanSDK/1.4.309.0/Lib/vulkan-1.lib")

    include_directories(${Vulkan_INCLUDE_DIR})
    target_link_libraries(TEn ${Vulkan_LIBRARY})

    # Ajoute GLFW (présumé en sous-dossier libs)
    add_subdirectory(libs/glfw-3.4)
    target_link_libraries(TEn glfw)

    # Ajoute GLM si utilisé
    add_subdirectory(libs/glm)
    target_link_libraries(TEn glm)
    
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Sur Linux, on utilise les librairies du système
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
endif()


#################################### SHADERS ###########################################

# Chercher tous les fichiers .spv dans shaders/
file(GLOB_RECURSE SPV_SHADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/shaders/*.spv")

# Répertoire de destination dans le dossier de build
set(SPV_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/shaders")

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
