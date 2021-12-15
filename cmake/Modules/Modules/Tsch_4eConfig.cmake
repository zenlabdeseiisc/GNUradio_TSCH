INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_TSCH_4E Tsch_4e)

FIND_PATH(
    TSCH_4E_INCLUDE_DIRS
    NAMES Tsch_4e/api.h
    HINTS $ENV{TSCH_4E_DIR}/include
        ${PC_TSCH_4E_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    TSCH_4E_LIBRARIES
    NAMES gnuradio-Tsch_4e
    HINTS $ENV{TSCH_4E_DIR}/lib
        ${PC_TSCH_4E_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TSCH_4E DEFAULT_MSG TSCH_4E_LIBRARIES TSCH_4E_INCLUDE_DIRS)
MARK_AS_ADVANCED(TSCH_4E_LIBRARIES TSCH_4E_INCLUDE_DIRS)

