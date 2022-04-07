include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)


# include source directories
include_directories(
        "${CMAKE_SOURCE_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(CV_TEST "cv_test")
add_executable(${CV_TEST}
        "src/util_tests/cv_test.cpp"
)
target_include_directories(${CV_TEST} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${CV_TEST} ${CONAN_LIBS})
add_test(NAME ${CV_TEST} COMMAND $<TARGET_FILE:${CV_TEST}>)

set(AL_TEST "latch_test")
add_executable(${AL_TEST}
        "src/util_tests/latch_test.cpp"
)
target_include_directories(${AL_TEST} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${AL_TEST} ${CONAN_LIBS})
add_test(NAME ${AL_TEST} COMMAND $<TARGET_FILE:${AL_TEST}>)