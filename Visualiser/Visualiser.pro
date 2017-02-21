TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

!macx {
    LIBS += -lm -ldl -lGL -lGLU -lpthread -lXext -lX11
    LIBS += -ljpeg -losg -losgGA -losgDB -losgViewer -losgSim -losgUtil -losgShadow
}

macx {
    LIBS += -lm -ldl -lpthread -framework OpenGL
    LIBS += -L/opt/local/lib -L/usr/local/lib -losg -losgGA -losgDB -losgViewer -losgSim -losgUtil -losgShadow -lOpenThreads
}

#This dependency is only needed for the perlin noise tests
!macx {
    INCLUDEPATH += $$PWD/../../glm
}

!macx {
    INCLUDEPATH += ..
}

DESTDIR = build
OBJECTS_DIR = build

SOURCES += main.cpp \
    util/log.cpp \
    TiledScene.cpp \
    TerrainTile.cpp \
    HeightMap.cpp \
    AsyncTerrainUpdater.cpp

HEADERS += \
    util/AtomicQueue.h \
    util/log.h \
    TiledScene.h \
    TerrainTile.h \
    HeightMap.h \
    AsyncTerrainUpdater.h

unix {
    system(mkdir -p build)
}
