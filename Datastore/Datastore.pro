TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lm -ldl -lpthread -framework OpenGL
LIBS += -L/opt/local/lib -L/usr/local/lib -losg -losgGA -losgDB -losgViewer -losgSim -losgUtil -losgShadow -lOpenThreads -lpqxx -lpq

#This dependency is only needed for the perlin noise tests
INCLUDEPATH += /usr/local/include

SOURCES += datastore_test.cpp

HEADERS += 