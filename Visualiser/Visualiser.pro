TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpqxx -lsnappy -lm -ldl -lpthread -losg -losgGA -losgDB -losgViewer -losgSim -losgUtil -losgShadow -lOpenThreads

QMAKE_CXXFLAGS += -g

!macx {
    LIBS += -lGL -lGLU -lXext -lX11
    LIBS += -ljpeg
}

macx {
    LIBS += -lm -ldl -lpthread -framework OpenGL
    LIBS += -L/opt/local/lib -L/usr/local/lib
    QMAKE_CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic
    INCLUDEPATH += /usr/local/include
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
    AsyncTerrainUpdater.cpp \
    KeyboardController.cpp \
    ../Datastore/datastore.cpp \
    ../Datastore/TileStore.cpp

HEADERS += \
    util/AtomicQueue.h \
    util/log.h \
    TiledScene.h \
    TerrainTile.h \
    HeightMap.h \
    AsyncTerrainUpdater.h \
    KeyboardController.h \
    ../Datastore/datastore.h \
    ../Datastore/TileStore.h \
    util/AsyncJobHandler.h \
    util/Array2d.h

unix {
    system(mkdir -p build)
}
