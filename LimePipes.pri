!isEmpty(LIMEPIPES_PRI_INCLUDED):error("LimePipes.pri already included")
LIMEPIPES_PRI_INCLUDED = 1

LIMEPIPES_SOURCE_TREE = $$PWD
LIMEPIPES_BUILD_FOLDER = $$LIMEPIPES_SOURCE_TREE/build
LIMEPIPES_STATIC_PLUGINS_FOLDER = $$LIMEPIPES_SOURCE_TREE/plugins_static

#Includes common configuration for all subdirectory .pro files.
INCLUDEPATH += \
    $$LIMEPIPES_SOURCE_TREE/src \
    $$LIMEPIPES_SOURCE_TREE/src/model \
    $$LIMEPIPES_SOURCE_TREE/src/views

CONFIG += depend_includepath

WARNINGS += -Wall

# The following keeps the generated files at least somewhat separate
# from the source files.
UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs
RCC_DIR = rccs

