find_package(Threads)

add_definitions(-Wall -g -pedantic -ggdb -lpthread -lcppdb_sqlite3)

add_executable(UaiFai main.c)

target_link_libraries(UaiFai mongoose)

target_link_libraries(UaiFai sqlite3)

target_link_libraries (UaiFai ${CMAKE_THREAD_LIBS_INIT})

