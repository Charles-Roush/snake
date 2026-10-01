#include <iostream>
#include "snake.h"
int main()
{
    SnakeGame snake(1000, 1000, "Snake");
    snake.run();
    return 0;
}