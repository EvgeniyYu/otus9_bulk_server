cmake_minimum_required(VERSION 3.8)

set(SRC ./src/server_async.cpp ./src/processor.cpp ./src/command_queue.cpp ./src/controller.cpp ./src/logger_view.cpp)
set(PATHS include/)
set(NAME_EXECUTABLE "bulk_server")
set(NAME_TEST "test_bulk_server")
set(NAME_LIBRARY "async")
set(PROJECT "bulk_server")

if ($ENV{GITHUB_RUN_NUMBER})
	project(${PROJECT} VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
	project(${PROJECT} VERSION 0.0.1)
endif()


find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_library(${NAME_LIBRARY} SHARED ./src/async.cpp ./include/async.h)

add_executable(${NAME_EXECUTABLE} ${SRC} ./src/main.cpp)

add_executable(${NAME_TEST} ${SRC} ./src/test.cpp)

set_target_properties(${NAME_EXECUTABLE} ${NAME_TEST} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

set_target_properties(${NAME_TEST} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(${NAME_EXECUTABLE} 
    PRIVATE "${PATHS}"
)

target_include_directories(${NAME_TEST} 
    PRIVATE "${PATHS}"
)


target_include_directories(${NAME_TEST} 
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(${NAME_LIBRARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${NAME_LIBRARY} PRIVATE "${PATHS}")

target_link_libraries(${NAME_EXECUTABLE} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE ${NAME_LIBRARY})

target_link_libraries(${NAME_TEST} PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${NAME_TEST} PRIVATE ${NAME_LIBRARY})

target_link_libraries(${NAME_EXECUTABLE} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${NAME_EXECUTABLE} PRIVATE pthread)
target_link_libraries(${NAME_LIBRARY} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${NAME_LIBRARY} PRIVATE pthread)
target_link_libraries(${NAME_TEST} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${NAME_TEST} PRIVATE pthread)


if (MSVC)
    target_compile_options(${NAME_EXECUTABLE} PRIVATE
        /W4
    )
    target_compile_options(${NAME_TEST} PRIVATE
        /W4
    )
else ()
    target_compile_options(${NAME_EXECUTABLE} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(${NAME_TEST} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()



install(TARGETS ${NAME_EXECUTABLE} RUNTIME DESTINATION bin)
install(TARGETS ${NAME_LIBRARY} LIBRARY DESTINATION lib)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT evgeniy.yurevich@gmail.com)

include(CPack)

enable_testing()

add_test(${NAME_TEST} ${NAME_TEST})

