TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++17
CONFIG += object_parallel_to_source
TARGET = RainbowAttack


QMAKE_CXXFLAGS += -std=c++17
SOURCES += \
        src/main.cpp \
        src/rainbowAttack.cpp \
        src/rainbowTableGen.cpp \
        src/sha256.cpp

DESTDIR = $$PWD/build

HEADERS += \
    src/passwd-utils.hpp \
    src/rainbowAttack.hpp \
    src/rainbowTableGen.hpp \
    src/random.hpp \
    src/reduction.hpp \
    src/sha256.h \
    src/threadpool.hpp \
    src/threadroutine.hpp \
    src/utils.hpp
