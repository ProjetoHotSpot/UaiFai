find_package(Threads)

add_definitions(-Wall -g -pedantic -ggdb -std=c99 -lpthread -lcppdb_sqlite3)

add_executable(UaiFai main.c)

add_library(routes routes.c)

add_library(resources resources.c)

target_link_libraries(UaiFai mongoose)

target_link_libraries(UaiFai sqlite3)

target_link_libraries(UaiFai ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(UaiFai routes)

target_link_libraries(UaiFai resources)

set(CMAKE_C_FLAGS "-std=c99")

