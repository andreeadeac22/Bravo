TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lm -ldl -lGL -lGLU -lpthread -lXext -lX11
LIBS += -ljpeg -losg -losgGA -losgDB -losgViewer -losgSim -losgUtil -losgShadow

#This dependency is only needed for the perlin noise tests
INCLUDEPATH += 

SOURCES += main.cpp \
    util/log.cpp \
    TiledScene.cpp \
    TerrainTile.cpp \
    HeightMap.cpp

HEADERS += \
    util/AtomicQueue.h \
    util/log.h \
    TiledScene.h \
    TerrainTile.h \
    HeightMap.h
