TEMPLATE = app
CONFIG += console c++14
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
    INCLUDEPATH += /usr/local/include
    QMAKE_CXXFLAGS -= -mmacosx-version-min=10.6
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++14 -stdlib=libc++
    QMAKE_LFLAGS -= -mmacosx-version-min=10.6
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
    CrackSegment.cpp \
    AxisAlignedBoundingBox.cpp \
    SurfaceVertexSet.cpp \
    LineSegment.cpp \
    util/Mat2x2.cpp

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
    CrackSegment.h \
    AxisAlignedBoundingBox.h \
    SurfaceVertexSet.h \
    LineSegment.h \
    util/Mat2x2.h

unix {
    system(mkdir -p build)
}
