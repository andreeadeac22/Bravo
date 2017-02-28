TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lm -ldl -lpthread -framework OpenGL -lsnappy
LIBS += -L/opt/local/lib -L/usr/local/lib -losg -losgGA -losgDB -losgViewer -losgSim -losgUtil -losgShadow -lOpenThreads -lpqxx -lpq

INCLUDEPATH += /usr/local/include

SOURCES += icestore_test.cpp datastore.cpp IceStore.cpp TileStore.cpp

HEADERS += datastore.h IceStore.h TileStore.h