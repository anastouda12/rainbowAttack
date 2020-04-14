TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += object_parallel_to_source
TARGET = RainbowAttack


QMAKE_CXXFLAGS += -std=c++17
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        src/func-utils.cpp \
        src/main.cpp \
        src/rainbowAttack.cpp \
        src/rainbowTableGen.cpp \
        src/reduction.cpp \
        src/sha256.cpp

DESTDIR = $$PWD/build

HEADERS += \
    src/headers/func-utils.hpp \
    src/headers/passwd-utils.hpp \
    src/headers/rainbowAttack.hpp \
    src/headers/rainbowTableGen.hpp \
    src/headers/random.hpp \
    src/headers/reduction.hpp \
    src/headers/sha256.h
