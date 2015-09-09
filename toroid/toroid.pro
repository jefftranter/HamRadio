TARGET = toroid
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp ToroidWindow.cpp Toroid.cpp
HEADERS += ToroidWindow.h Toroid.h
FORMS += toroid.ui
RESOURCES += toroid.qrc

unix: {
    target.path = /usr/local/bin
    INSTALLS += target
}
