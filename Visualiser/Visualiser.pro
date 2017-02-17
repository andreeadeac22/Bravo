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
macx {
    INCLUDEPATH += /usr/local/include
}

DESTDIR = build
OBJECTS_DIR = build

INCLUDEPATH += ./src

SOURCES += src/main.cpp \
    util/log.cpp \
    src/TiledScene.cpp \
    src/TerrainTile.cpp \
    src/HeightMap.cpp

HEADERS += \
    util/AtomicQueue.h \
    util/log.h \
    src/TiledScene.h \
    src/TerrainTile.h \
    src/HeightMap.h

unix {
    system(mkdir -p build)
}
