TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lm -ldl -lpthread 
LIBS += -L/opt/local/lib -L/usr/local/lib -lpqxx -lpq -lsnappy

INCLUDEPATH += /usr/local/include

SOURCES += datastore.cpp TileStore.cpp tileddatastore_test.cpp

HEADERS += datastore.h TileStore.h
