TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lm -ldl -lpthread 
LIBS += -L/opt/local/lib -L/usr/local/lib -lpqxx -lpq -lsnappy

#This dependency is only needed for the perlin noise tests
INCLUDEPATH += /usr/local/include

SOURCES += tileddatastore_test.cpp
