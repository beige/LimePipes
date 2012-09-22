#-------------------------------------------------
#
# Project created by QtCreator 2012-09-20T17:43:40
#
#-------------------------------------------------
include( ../../../tests.pri )
include( InstrumentManager.pri )
include( $$LIMEPIPES_SOURCE_TREE/src/model/model.pri )
include( $$LIMEPIPES_SOURCE_TREE/src/model/datatypes/datatypes.pri )

QT       += testlib

QT       -= gui

TARGET = $$INSTRUMENT_MANAGER_BUILD_DIR/tst_instrumentmanagertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_instrumentmanagertest.cpp \
    $$LIMEPIPES_SOURCE_TREE/src/app/instrumentmanager.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS =  -L$${LIMEPIPES_SOURCE_TREE}/plugins_static -llp_greathighlandbagpipe

HEADERS += \
    $$LIMEPIPES_SOURCE_TREE/src/app/instrumentmanager.h
