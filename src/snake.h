//
// Created by Charles Roush on 10/29/25.
//
#pragma once
#include <random>
#include <string>
#include <vector>


#include "raylib.h"

#define BOARD_SIZE 20
#define FOOD_AMOUNT 10

struct Vector2Int
{
    int x;
    int y;
};

class SnakeGame
{
    public:
    SnakeGame(int width, int height, std::string title);
    void run();

    private:
    void init(int width, int height, std::string title);
    void handle_input();
    void update_snake();

    void randomize_food_pos(Vector2Int& food);

    bool is_touching_food();

    void update();
    void render_squares(const std::vector<Vector2Int> &positions, Color color);
    void render();
    void shutdown();

    std::mt19937 m_gen{std::random_device{}()};
    std::vector<Vector2Int> m_snake{{10,10},{9,10},{8,10}};
    std::vector<Vector2Int> m_foods;
    Vector2Int m_dir{1,0};
    Vector2Int last_m_dir = m_dir;
    bool m_game_over = false;

};


