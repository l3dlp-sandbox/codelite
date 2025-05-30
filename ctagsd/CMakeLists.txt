set(CL_SRC_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CL_SRC_ROOT}/cmake/Modules/")
set(BUILD_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIRECTORY}/lib")

# Our project is called 'codelite' this is how it will be called in visual studio, and in our makefiles.
project(ctagsd)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.

include("${wxWidgets_USE_FILE}")

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if(NOT CL_WX_CONFIG)
    set(CL_WX_CONFIG "wx-config")
endif()

# we need wxWidgets flags to be set only for the c++ files, so we do it like this by setting the CMAKE_CXX_FLAGS if(NOT
# MINGW) execute_process(COMMAND ${CL_WX_CONFIG} --cxxflags OUTPUT_VARIABLE WX_CXX_FLAGS
# OUTPUT_STRIP_TRAILING_WHITESPACE) else() execute_process(COMMAND sh ${CL_WX_CONFIG} --cxxflags OUTPUT_VARIABLE
# WX_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE) endif() set (CMAKE_CXX_FLAGS
# "${CMAKE_CXX_FLAGS} ${WX_CXX_FLAGS}") message("${WX_CXX_FLAGS}")

# and finally prepare list of includes directories
include_directories("${CL_SRC_ROOT}/ctagsd/lib")

set(UTIL_LIB "")
if(UNIX)
    set(UTIL_LIB "-lutil")
endif(UNIX)

file(GLOB LIBSRC "lib/*.cpp")
add_library(ctagdslib STATIC "${LIBSRC}")
target_link_libraries(ctagdslib PRIVATE libcodelite plugin)

target_include_directories(ctagdslib PUBLIC "${CL_SRC_ROOT}/CxxParser")

add_executable(ctagsd "main.cpp")
target_link_libraries(
    ctagsd
    ctagdslib
    ${LINKER_OPTIONS}
    -L"${CL_LIBPATH}"
    libcodelite
    plugin
    ${UTIL_LIB})

cl_install_executable(ctagsd)

include(CTest)
if(BUILD_TESTING)
    add_executable(ctagsd-tests "tests/main.cpp" "tests/tester.cpp")
    target_link_libraries(
        ctagsd-tests
        ctagdslib
        ${LINKER_OPTIONS}
        -L"${CL_LIBPATH}"
        libcodelite
        plugin
        wxsqlite3
        ${UTIL_LIB})

    add_test(NAME "ctagsd-tests" COMMAND ctagsd-tests)

    cl_install_executable(ctagsd-tests)
endif(BUILD_TESTING)
