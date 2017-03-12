TEMPLATE = app
HEADERS += window.h \
           renderwindow.h \
           itemsview.h \
           pointsview.h \
           markersview.h \
           readthread.h \
           xmlparser.h
           
SOURCES += main.cpp \
           window.cpp \
           renderwindow.cpp \
           pointsview.cpp \
           markersview.cpp \
           readthread.cpp \
           xmlparser.cpp
           
TARGET = testapp
CONFIG += qt
QT += xml widgets
