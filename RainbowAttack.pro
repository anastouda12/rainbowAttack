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
        src/sha256.cpp \
        src/utils.cpp

DESTDIR = $$PWD/build

HEADERS += \
    src/headers/passwd-utils.hpp \
    src/headers/rainbowAttack.hpp \
    src/headers/rainbowTableGen.hpp \
    src/headers/random.hpp \
    src/headers/reduction.hpp \
    src/headers/sha256.h \
    src/headers/utils.hpp
