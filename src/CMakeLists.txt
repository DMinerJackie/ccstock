INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

ADD_EXECUTABLE(ccstock 
    ccstock.cpp
    common/system/cctime.cpp
)

TARGET_LINK_LIBRARIES(ccstock
    ncursesw
    ev
    curl
    boost_system
    boost_filesystem
    boost_program_options)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable -Wno-unused-function -O2 -std=c++11" )
