#
# Find the native ZLIB includes and library
#

IF(NOT HAS_NATIVE_ZLIB)
  FIND_PATH(NATIVE_ZLIB_INCLUDE_PATH zlib.h
    /usr/local/include
    /usr/include
  )
  
  FIND_LIBRARY(NATIVE_ZLIB_LIBRARY z
    /usr/lib
    /usr/local/lib
  )
  
  IF(NATIVE_ZLIB_INCLUDE_PATH)
  IF(NATIVE_ZLIB_LIBRARY)
  
    SET( HAS_NATIVE_ZLIB "YES" )
  
    INCLUDE_DIRECTORIES( ${NATIVE_ZLIB_INCLUDE_PATH} )
    LINK_LIBRARIES( ${NATIVE_ZLIB_LIBRARY} )
  
  ENDIF(NATIVE_ZLIB_LIBRARY)
  ENDIF(NATIVE_ZLIB_INCLUDE_PATH)
ENDIF(NOT HAS_NATIVE_ZLIB)
