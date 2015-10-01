find_package(Threads)

add_definitions(-Wall -g -pedantic -ggdb -lpthread -lcppdb_sqlite3)

add_executable(Hotspot main.c)

target_link_libraries(Hotspot mongoose)

target_link_libraries(Hotspot sqlite3)

target_link_libraries (Hotspot ${CMAKE_THREAD_LIBS_INIT})

