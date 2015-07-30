SET( ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../ )
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${ROOT_PATH}
)

ADD_EXECUTABLE(test
    test_main.cpp
    ${ROOT_PATH}/common/system/cctime.cpp
)

TARGET_LINK_LIBRARIES(test
    ev
    ncurses
    curl
    leveldb
    boost_system
    boost_filesystem
    boost_program_options
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable -Wno-unused-function -O2 -std=c++11" )
