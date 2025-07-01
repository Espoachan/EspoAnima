cmake_minimum_required(VERSION 3.16)
project(EspoAnima LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar Qt6 y sus componentes
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Archivos fuente
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/app/*.cpp)
file(GLOB_RECURSE COLORPICKERSOURCES CONFIGURE_DEPENDS src/app/colorpicker/*.cpp)
file(GLOB_RECURSE COLORPICKERHEADERS CONFIGURE_DEPENDS src/app/colorpicker/*.h)
file(GLOB HEADERS "src/app/headers/*.h")

# Recursos (qrc)
set(RESOURCES resources/resources.qrc)

# Generar MOC y recursos
qt6_wrap_cpp(MOC_SOURCES
    ${HEADERS}
    src/app/colorpicker/ColorPicker.h
)
qt6_add_resources(RESOURCE_SOURCES ${RESOURCES})

# Crear el ejecutable con todas las fuentes y cabeceras necesarias
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${MOC_SOURCES}
    ${RESOURCE_SOURCES}
    ${COLORPICKERSOURCES}
)

# Incluir rutas
target_include_directories(${PROJECT_NAME} PRIVATE
    src/app
    src/app/headers
)

# Enlazar con Qt6
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
