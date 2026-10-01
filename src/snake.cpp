//
// Created by Charles Roush on 10/29/25.
//

#include "snake.h"
#include <algorithm>
#include <chrono>
#include <unordered_set>

SnakeGame::SnakeGame(int width, int height, std::string title)
    : m_foods(FOOD_AMOUNT, Vector2Int{0,0})
{
    init(width, height, title);
    for (auto& f: m_foods)
    {
        randomize_food_pos(f);
    }
}

void SnakeGame::run()
{
    auto last_move = std::chrono::high_resolution_clock::now();
    const float move_interval = 0.2f;

    while (!WindowShouldClose() && !m_game_over)
    {
        handle_input();
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - last_move;
        if (elapsed.count() >= move_interval)
        {
            update();
            last_move = now;
        }

        render();
    }

    shutdown();
}

void SnakeGame::init(int width, int height, std::string title)
{
    InitWindow(width, height, title.c_str());
}
void SnakeGame::handle_input()
{
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && last_m_dir.y == 0) m_dir = {0, -1};
    else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && last_m_dir.y == 0) m_dir = {0, 1};
    else if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && last_m_dir.x == 0) m_dir = {-1, 0};
    else if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && last_m_dir.x == 0) m_dir = {1, 0};
}
void SnakeGame::update_snake()
{
    Vector2Int old_head = m_snake.front();
    Vector2Int new_head = Vector2Int(old_head.x + m_dir.x, old_head.y + m_dir.y);

    for (int i = static_cast<int>(m_snake.size()) - 1; i > 0; --i) {
        m_snake[i] = m_snake[i - 1];
    }

    m_snake[0] = new_head;

    if (is_touching_food()) {
        m_snake.emplace_back( old_head );
    }

    bool out_of_bounds = new_head.x < 0 || new_head.y < 0 ||
                         new_head.x >= BOARD_SIZE || new_head.y >= BOARD_SIZE;

    bool touching_self = std::any_of(m_snake.begin() + 1, m_snake.end(),
        [&](const Vector2Int &seg) {
            return seg.x == new_head.x && seg.y == new_head.y;
        });

    m_game_over = out_of_bounds || touching_self;
}

void SnakeGame::randomize_food_pos(Vector2Int &food)
{
    const int total_cells = BOARD_SIZE * BOARD_SIZE;
    const int occupied_cells = static_cast<int>(m_snake.size() + m_foods.size());

    if (occupied_cells >= total_cells) {
        food = Vector2Int{-1, -1};
        m_game_over = true;
        return;
    }

    std::unordered_set<int> occupied_indices;
    occupied_indices.reserve(occupied_cells);

    for (const auto& seg : m_snake) {
        occupied_indices.insert(seg.y * BOARD_SIZE + seg.x);
    }
    for (const auto& f : m_foods) {
        occupied_indices.insert(f.y * BOARD_SIZE + f.x);
    }

    const int free_cells = total_cells - occupied_cells;
    std::uniform_int_distribution<int> dist(0, free_cells - 1);
    int target_index = dist(m_gen);

    for (int i = 0; i < total_cells; ++i) {
        if (occupied_indices.find(i) == occupied_indices.end()) {
            if (target_index == 0) {
                food.x = i % BOARD_SIZE;
                food.y = i / BOARD_SIZE;
                return;
            }
            --target_index;
        }
    }

    food = Vector2Int{0, 0};
}

bool SnakeGame::is_touching_food()
{
    auto &head = m_snake[0];
    for (auto& food : m_foods)
    {
        if (food.x == head.x && food.y == head.y)
        {
            randomize_food_pos(food);
            return true;
        }
    }
    return false;
}


void SnakeGame::update()
{
    last_m_dir = m_dir;
    update_snake();
}
void SnakeGame::render_squares(const std::vector<Vector2Int>& positions, Color color)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    int min = std::min(w, h);
    int square_px_size = min / BOARD_SIZE;

    for (auto &segement : positions)
    {
        int pixel_x = segement.x * square_px_size;
        int pixel_y = segement.y * square_px_size;
        DrawRectangle(pixel_x, pixel_y, square_px_size, square_px_size, color);
    }
}
void SnakeGame::render()
{
    BeginDrawing();
    ClearBackground(BLACK);

    render_squares(m_snake, WHITE);
    render_squares(m_foods, RED);

    EndDrawing();
}

void SnakeGame::shutdown()
{
    CloseWindow();
}