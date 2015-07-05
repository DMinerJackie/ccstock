INCLUDE_DIRECTORIES(/usr/include) 
INCLUDE_DIRECTORIES(/usr/include/curl) 
INCLUDE_DIRECTORIES(/usr/include/boost) 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET( CMAKE_BUILD_TYPE Debug )

ADD_DEFINITIONS (
	-std=c++11
)

ADD_EXECUTABLE(ccstock ccstock.cpp)

TARGET_LINK_LIBRARIES(ccstock
    ev
    curl
    boost_system
    boost_filesystem
    boost_program_options)
