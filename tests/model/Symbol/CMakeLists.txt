set( testname SymbolTest )
set( testmodules Test Gui )
set( testlibraries lp_model )

find_package( Qt5Gui   REQUIRED )
find_package( Qt5Test  REQUIRED )

set( Test_SOURCES
        tst_symboltest.cpp
        testsymbol.cpp
        )

add_executable( ${testname} ${Test_SOURCES} )
qt5_use_modules( ${testname} ${testmodules} )
target_link_libraries( ${testname} ${testlibraries} )

add_test( NAME ${testname} COMMAND ${testname} )
