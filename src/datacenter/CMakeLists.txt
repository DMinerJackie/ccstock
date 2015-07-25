SET( ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../ )

message(${ROOT_PATH})
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ROOT_PATH}
)

ADD_EXECUTABLE(dc
    dc.cpp
    dc_service.cpp
    db/history_db.cpp
)

TARGET_LINK_LIBRARIES(dc
    ev
    curl
    boost_system
    boost_filesystem
    boost_program_options
    leveldb
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable -Wno-unused-function -O2 -std=c++11" )
