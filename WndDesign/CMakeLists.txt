file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    *.h
    *.cpp
)

add_library(WndDesign STATIC ${LIB_SOURCES})
target_include_directories(WndDesign PUBLIC ${CMAKE_SOURCE_DIR})
