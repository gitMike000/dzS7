TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH +=/usr/include/google/protobuf
LIBS += /usr/lib/libprotobuf.so -lprotobuf

SOURCES += main.cpp \
        Task2.pb.cc

HEADERS += Task2.pb.h

OTHER_FILES += \
    Task2.proto
