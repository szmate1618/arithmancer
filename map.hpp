#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const int WIDTH = 100;
const int HEIGHT = 100;

class Map
{
public:
    Map(size_t roomCount);
    void PrintDungeon();
private:

    std::vector<std::vector<char>> grid;

    struct Room {
        int x, y, w, h;
    };

    void GenerateDungeon(size_t roomCount);
};

Map::Map(size_t roomCount): grid(HEIGHT, std::vector<char>(WIDTH))
{
    GenerateDungeon(roomCount);
}

// Print the dungeon
void Map::PrintDungeon() {
    for (const auto& row : grid) {
        for (char cell : row)
            std::cout << cell;
        std::cout << '\n';
    }
}

// Dungeon generation function
void Map::GenerateDungeon(size_t roomCount) {
    std::vector<Room> rooms;
    srand(time(0));

    // Initialize grid with walls
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            grid[i][j] = '#';

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
                grid[y][x] = '.';
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
            grid[y1][x] = '.';

        // Vertical tunnel
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
            grid[y][x2] = '.';
    }

    // Pass 1: Remove isolated walls (turn '#' into ' ' if not adjacent to '.')
    std::vector<std::vector<char>> tempGrid = grid;
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            if (grid[y][x] == '#') {
                bool hasNeighbor = false;
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx)
                        if (grid[y + dy][x + dx] == '.')
                            hasNeighbor = true;

                if (!hasNeighbor)
                    tempGrid[y][x] = ' '; // Remove isolated walls
            }
        }
    }
    grid = tempGrid;

    // Pass 2: Convert all remaining '.' to spaces
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            if (grid[y][x] == '.')
                grid[y][x] = ' ';
}
