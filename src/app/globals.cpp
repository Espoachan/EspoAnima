#include "headers/globals.h"

QString theme = R"(
    QWidget {
        background-color: #222;
        color: white;
    }

    QPushButton {
        background-color: #444;
        color: white;
        border: 1px solid #888;
        padding: 5px;
    }

    QPushButton:hover {
        background-color: #666;
    }
    
    QToolButton {
        background-color: #444;
        color: black;
        border: 1px solid #888;
        padding: 5px;
    }

    QToolButton:hover {
        background-color: #666;
    }
)";
// Tema por defecto
