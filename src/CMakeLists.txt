set(SNAKE_SOURCES
        snake.cpp
        snake.h
)

add_library(snake_lib STATIC ${SNAKE_SOURCES})
target_include_directories(snake_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(snake_lib PUBLIC raylib)

add_executable(snake main.cpp)
target_link_libraries(snake PRIVATE snake_lib)