QT += core
QT += multimedia

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

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
    INCLUDEPATH += /usr/local/include
    QMAKE_CXXFLAGS -= -mmacosx-version-min=10.6
    QMAKE_LFLAGS -= -mmacosx-version-min=10.6
    # If c++14 is not recorgnised, change two lines to -std=c++11
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++14 -stdlib=libc++
    QMAKE_LFLAGS += -mmacosx-version-min=10.7 -std=c++14 -stdlib=libc++
    QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra -pedantic -Wno-unused-parameter
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
    ../Datastore/TileStore.cpp \
    AsyncTerrainLoader.cpp

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
    util/Array2d.h \
    AsyncTerrainLoader.h

unix {
    system(mkdir -p build)
}

RESOURCES += \
    res.qrc
