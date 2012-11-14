#-------------------------------------------------
#
# Project created by QtCreator 2012-10-06T08:34:48
#
#-------------------------------------------------
include( ../../../tests.pri )
include( $$LIMEPIPES_SOURCE_TREE/src/views/treeview/treeview.pri )
include( $$LIMEPIPES_SOURCE_TREE/src/model/model.pri )
include( $$LIMEPIPES_SOURCE_TREE/src/model/datatypes/datatypes.pri )

QT       += testlib

TARGET = bin/tst_pitchdelegatetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_pitchdelegatetest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS =  -L$${TEST_PLUGINS_FOLDER} -llp_musicmodeltestplugin
