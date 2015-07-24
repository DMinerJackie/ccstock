INCLUDE_DIRECTORIES(/usr/include) 
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)

SET( CMAKE_BUILD_TYPE Debug )

ADD_DEFINITIONS (
    -std=c++11
)

ADD_EXECUTABLE(dc 
    dc.cpp
    dc_service.cpp
)

TARGET_LINK_LIBRARIES(dc
    ev
    curl
    boost_system
    boost_filesystem
    boost_program_options
)
