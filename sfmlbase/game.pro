TEMPLATE = app
TARGET = bin/game

OBJECTS_DIR = obj

DESTDIR_TARGET
CONFIG += exceptions
CONFIG += debug
CONFIG += opengl

#INCLUDEPATH += C:\SFML-2.1\include\

CONFIG(release, debug|release) {
    message("in release")
    CONFIG += release
    QMAKE_CFLAGS += -O2
    QMAKE_CXXFLAGS += -O2
} else {
    message("in debug")
    CONFIG += debug
    #CONFIG += console
}

INCLUDEPATH += src/

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS_WARN_ON += -Wextra -Wno-switch -Wno-unused-parameter -Wno-reorder -Wno-unused-function

#LIBS += -L C:\\SFML-2.1\\lib
#LIBS += -lsfml-main
LIBS += -lsfml-system
LIBS += -lsfml-window
LIBS += -lsfml-graphics
LIBS += -lsfml-audio
#LIBS += -lOpenGL32
#LIBS += -lsfml-network

SOURCES += \
    $$PWD/src/*.cpp \
    $$PWD/src/helpers/*.cpp \
    $$PWD/src/math/*.cpp \

HEADERS += \
    $$PWD/src/*.h \
    $$PWD/src/helpers/*.h \
    $$PWD/src/math/*.h \

DISTFILES += \
    src/shaders/*
`

#Copy shaders to target directory
copy_distfiles.commands = $$QMAKE_COPY $$PWD/$$DISTFILES $$OUT_PWD/bin
first.depends = $(first) copy_distfiles
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copy_distfiles
