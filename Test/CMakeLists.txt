file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    *.cpp
)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE WndDesign)
    target_link_options(${TEST_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
endforeach()
