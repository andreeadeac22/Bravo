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
    CONFIG += warn_on
    LIBS += -lm -ldl -lpthread -framework OpenGL
    LIBS += -L/opt/local/lib -L/usr/local/lib
    QMAKE_CXXFLAGS -= -mmacosx-version-min=10.6
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++14 -stdlib=libc++
    QMAKE_LFLAGS -= -mmacosx-version-min=10.6
    QMAKE_LFLAGS += -mmacosx-version-min=10.7 -std=c++14 -stdlib=libc++
    QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra -pedantic -Wno-unused-parameter
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
    TileLoader.cpp

HEADERS += \
    util/AtomicQueue.h \
    util/log.h \
    TiledScene.h \
    TerrainTile.h \
    HeightMap.h \
    AsyncTerrainUpdater.h \
    KeyboardController.h \
    TileLoader.h \
    util/AsyncJobHandler.h \
    util/Array2d.h

unix {
    system(mkdir -p build)
}
