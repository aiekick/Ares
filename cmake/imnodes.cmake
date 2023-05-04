set(IMNODES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/ImNodes)
file(GLOB IMNODES_SOURCES ${IMNODES_INCLUDE_DIR}/*.cpp)
file(GLOB IMNODES_HEADERS ${IMNODES_INCLUDE_DIR}/*.h)
                 
add_library(ImNodes STATIC ${IMNODES_SOURCES} ${IMNODES_HEADERS})

include_directories(${IMNODES_INCLUDE_DIR})
    
set_target_properties(ImNodes PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ImNodes PROPERTIES FOLDER 3rdparty)

set(IMNODES_LIBRARIES ImNodes)
