cmake_minimum_required(VERSION 3.16)
project(EspoAnima LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar Qt6 y sus componentes
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Archivos fuente
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/app/*.cpp)

# Archivos de cabecera (pueden ser detectados por patrón si gustas)
file(GLOB HEADERS "src/app/headers/*.h")

# Recursos (qrc)
set(RESOURCES resources/resources.qrc)

# Generar MOC y recursos
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
qt6_add_resources(RESOURCE_SOURCES ${RESOURCES})

# Crear el ejecutable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${MOC_SOURCES}
    ${RESOURCE_SOURCES}
)

# Incluir rutas
target_include_directories(${PROJECT_NAME} PRIVATE
    src/app
    src/app/headers
)

# Enlazar con Qt6
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)


