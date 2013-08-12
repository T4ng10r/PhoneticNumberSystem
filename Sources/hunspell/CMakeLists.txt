cmake_minimum_required(VERSION 2.8)
project ( hunspell )
SET(HUNSPELL_VERSION 1.3.2)


include_directories(${hunspell_PROJECT_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/hunspell)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/parsers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/tools)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/win_api)

if(WIN32)
    #add_definitions( -DWIN32 -DMINGW32 -c -O2 -frtti -fexceptions -mthreads -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc)
endif(WIN32)

# LIBS
SET(SRCS
    src/hunspell/affentry.cxx
    src/hunspell/affixmgr.cxx
    src/hunspell/csutil.cxx
    src/hunspell/dictmgr.cxx
    src/hunspell/hashmgr.cxx
    src/hunspell/suggestmgr.cxx
    src/hunspell/license.myspell
    src/hunspell/license.hunspell
    src/hunspell/phonet.cxx
    src/hunspell/filemgr.cxx
    src/hunspell/hunzip.cxx
    src/hunspell/hunspell.cxx
    src/hunspell/replist.cxx)

SET(HEADERS
    src/hunspell/affentry.hxx
    src/hunspell/htypes.hxx
    src/hunspell/affixmgr.hxx
    src/hunspell/csutil.hxx
    src/hunspell/hunspell.hxx
    src/hunspell/atypes.hxx
    src/hunspell/dictmgr.hxx
    src/hunspell/hunspell.h
    src/hunspell/suggestmgr.hxx
    src/hunspell/baseaffix.hxx
    src/hunspell/hashmgr.hxx
    src/hunspell/langnum.hxx
    src/hunspell/phonet.hxx
    src/hunspell/filemgr.hxx
    src/hunspell/hunzip.hxx
    src/hunspell/w_char.hxx
    src/hunspell/replist.hxx
    src/hunspell/hunvisapi.h)

ADD_LIBRARY( hunspell-static STATIC ${HEADERS} ${SRCS})
TARGET_LINK_LIBRARIES( hunspell-static )
ADD_DEPENDENCIES( hunspell-static ${HEADERS})
#INSTALL(TARGETS hunspell-static RUNTIME DESTINATION bin
#                                 LIBRARY DESTINATION lib
#                                 ARCHIVE DESTINATION lib)
SET_TARGET_PROPERTIES( hunspell-static PROPERTIES COMPILE_FLAGS "-DHUNSPELL_STATIC -D_CRT_SECURE_NO_WARNINGS")


ADD_LIBRARY( hunspell SHARED ${HEADERS} ${SRCS})
# set Windows DLL version information
IF(WIN32)
#      ADD_CUSTOM_COMMAND(TARGET hunspell PRE_BUILD COMMAND windres -i ${CMAKE_CURRENT_SOURCE_DIR}/src/win_api/Hunspell.rc -o Hunspell_res.o COMMENT "Windows DLL version")
#      SET_TARGET_PROPERTIES(hunspell PROPERTIES LINK_FLAGS "Hunspell_res.o")
ENDIF(WIN32)
SET_TARGET_PROPERTIES( hunspell PROPERTIES COMPILE_FLAGS "-DMAKE_DLL -DNDEBUG -D_USRDLL -DBUILDING_LIBHUNSPELL -D_CRT_SECURE_NO_DEPRECATE")
ADD_DEPENDENCIES( hunspell ${HEADERS})
TARGET_LINK_LIBRARIES( hunspell )

#SET_TARGET_PROPERTIES(
    #hunspell
    #hunspell-static
  #PROPERTIES
    #VERSION ${HUNSPELL_VERSION}
    #SOVERSION ${HUNSPELL_VERSION}
    #PREFIX "lib"
    #OUTPUT_NAME "hunspell")

#########
SET(PARSER_SRCS
    src/parsers/firstparser.cxx
    src/parsers/htmlparser.cxx
    src/parsers/latexparser.cxx
    src/parsers/manparser.cxx
    src/parsers/textparser.cxx)

SET(PARSER_HEADERS
    src/parsers/firstparser.hxx
    src/parsers/htmlparser.hxx
    src/parsers/manparser.hxx
    src/parsers/latexparser.hxx
    src/parsers/textparser.hxx)

#ADD_LIBRARY( parsers-static STATIC ${PARSER_HEADERS} ${PARSER_SRCS})
#SET_TARGET_PROPERTIES( hunspell-static PROPERTIES PREFIX "lib")
#TARGET_LINK_LIBRARIES( parsers-static hunspell-static )
#ADD_DEPENDENCIES( parsers-static ${PARSER_HEADERS})
#INSTALL(TARGETS parsers-static RUNTIME DESTINATION bin
#                                 LIBRARY DESTINATION lib
#                                 ARCHIVE DESTINATION lib)
                                 
#ADD_LIBRARY( parsers SHARED ${PARSER_HEADERS} ${PARSER_SRCS})
#SET_TARGET_PROPERTIES( parsers PROPERTIES COMPILE_FLAGS "-DMAKE_DLL")
#ET_TARGET_PROPERTIES( hunspell-static PROPERTIES PREFIX "lib")
#TARGET_LINK_LIBRARIES( parsers hunspell)
#ADD_DEPENDENCIES( parsers ${PARSER_HEADERS})

