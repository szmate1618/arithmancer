#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class Map
{
public:
    enum class TileType { WALL, FLOOR, START, GOAL, EMPTY, PLAYER, ENEMY };

    struct Player {
        int x, y;
        TileType standingOn;
    };

    Map(size_t width, size_t height, size_t roomCount);
    void PrintDungeon();
    bool IsWalkable(int x, int y) const;
    void PlacePlayer(int x, int y);

private:
    size_t WIDTH, HEIGHT;
    std::vector<std::vector<TileType>> grid;
    static constexpr char tileChars[] = { '#', '.', 'S', 'G', ' ', '@', '*' };
    static constexpr bool walkable[] = { false, true, true, true, true, false, false };
    Player player;

    struct Room {
        int x, y, w, h;
    };

    void GenerateDungeon(size_t roomCount);
};

Map::Map(size_t width, size_t height, size_t roomCount) : WIDTH(width), HEIGHT(height), grid(height, std::vector<TileType>(width, TileType::WALL))
{
    GenerateDungeon(roomCount);
}

// Print the dungeon
void Map::PrintDungeon() {
    for (const auto& row : grid) {
        for (TileType cell : row)
            std::cout << tileChars[static_cast<int>(cell)];
        std::cout << '\n';
    }
}

// Check if a position is walkable
bool Map::IsWalkable(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return false;
    return walkable[static_cast<int>(grid[y][x])];
}

// Place the player on a walkable tile
void Map::PlacePlayer(int x, int y) {
    if (IsWalkable(x, y)) {
        player.x = x;
        player.y = y;
        player.standingOn = grid[y][x];
        grid[y][x] = TileType::PLAYER;
    }
}

// Dungeon generation function
void Map::GenerateDungeon(size_t roomCount) {
    std::vector<Room> rooms;
    srand(time(0));

    // Place rooms
    for (int i = 0; i < roomCount; ++i) {
        int rw = 5 + rand() % 10; // Room width: 5-14
        int rh = 5 + rand() % 10; // Room height: 5-14
        int rx = rand() % (WIDTH - rw - 1) + 1;
        int ry = rand() % (HEIGHT - rh - 1) + 1;

        // Create room
        Room room = { rx, ry, rw, rh };
        rooms.push_back(room);

        // Carve out the room
        for (int y = ry; y < ry + rh; ++y)
            for (int x = rx; x < rx + rw; ++x)
                grid[y][x] = TileType::FLOOR;
    }

    // Connect rooms with corridors
    for (size_t i = 1; i < rooms.size(); ++i) {
        Room a = rooms[i - 1];
        Room b = rooms[i];

        int x1 = a.x + a.w / 2;
        int y1 = a.y + a.h / 2;
        int x2 = b.x + b.w / 2;
        int y2 = b.y + b.h / 2;

        // Horizontal tunnel
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
            grid[y1][x] = TileType::FLOOR;

        // Vertical tunnel
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
            grid[y][x2] = TileType::FLOOR;
    }

    // Pass 1: Remove isolated walls (turn WALL into EMPTY if not adjacent to FLOOR)
    std::vector<std::vector<TileType>> tempGrid = grid;
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            if (grid[y][x] == TileType::WALL) {
                bool hasNeighbor = false;
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                        if (grid[y + dy][x + dx] == TileType::FLOOR)
                            hasNeighbor = true;

                if (!hasNeighbor)
                    tempGrid[y][x] = TileType::EMPTY; // Remove isolated walls
            }
        }
    }
    grid = tempGrid;

    // Pass 2: Convert all remaining FLOOR to EMPTY
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            if (grid[y][x] == TileType::FLOOR)
                grid[y][x] = TileType::EMPTY;

    // Set START and GOAL positions
    if (!rooms.empty()) {
        int sx = rooms.front().x + rooms.front().w / 2;
        int sy = rooms.front().y + rooms.front().h / 2;
        grid[sy][sx] = TileType::START;
        PlacePlayer(sx, sy);

        int gx = rooms.back().x + rooms.back().w / 2;
        int gy = rooms.back().y + rooms.back().h / 2;
        grid[gy][gx] = TileType::GOAL;
    }
}
