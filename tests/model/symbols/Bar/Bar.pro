#-------------------------------------------------
#
# Project created by QtCreator 2012-09-07T11:41:52
#
#-------------------------------------------------
include( ../../../tests.pri )
include( $$LIMEPIPES_SOURCE_TREE/src/model/model.pri )
include( $$LIMEPIPES_SOURCE_TREE/src/model/symbols/symbols.pri )

QT       += testlib

QT       -= gui

TARGET = bin/tst_bartest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_bartest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"