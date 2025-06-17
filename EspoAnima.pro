QT       += core gui widgets

CONFIG += c++17

# Desactiva APIs viejas si quieres forzarte a usar solo Qt 6
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    Canvas.cpp \
    FrameTimeLine.cpp \
    NewProjectDialog.cpp \
    main.cpp \
    mainwindow.cpp \
    menubarhelper.cpp

HEADERS += \
    Canvas.h \
    FrameTimeLine.h \
    NewProjectDialog.h \
    mainwindow.h \
    menubarhelper.h

# Si no estás usando el diseñador de interfaces (Design), puedes eliminar esto:
FORMS += \
    mainwindow.ui

TRANSLATIONS +=

CONFIG += lrelease
CONFIG += embed_translations

# Instalación (puedes dejarlo igual si no lo usas)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
