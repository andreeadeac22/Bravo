TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lm -ldl -lpthread -framework OpenGL
LIBS += -L/opt/local/lib -L/usr/local/lib -losg -losgGA -losgDB -losgViewer -losgSim -losgUtil -losgShadow -lOpenThreads

#This dependency is only needed for the perlin noise tests
INCLUDEPATH += /usr/local/include

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
