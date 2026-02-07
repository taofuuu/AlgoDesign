#include <iostream>
#include <vector>

using namespace std;

struct Command {
    int type, x, y;
};

vector<Command> commands;

// type 0  type 1  type 2  type 3
//    o      o       oo      oo
//   oo      oo       o      o
//
// (x, y) - top-left coords of the grid
void tile(int size, int x, int y, int holeX, int holeY) {
    if (size == 1) return;

    int half = size / 2;

    // center position for calculating (bottom-right)
    // 'fake hole' position for tile needed for filling
    int centerX = x + half;
    int centerY = y + half;

    // top-left coords of center tile
    int tileX = centerX - 1;
    int tileY = centerY - 1;

    // determine hole relative position
    // give hole position to quadrant with hole (check relative to center position)
    // give 'fake hole' position for other quadrant with no hole
    // fill 'fake hole' position with command
    if (holeY < centerY) {
        if (holeX < centerX) {
            // center tile w/ hole top-left
            commands.push_back({0, tileX, tileY}); 

            tile(half, x, y, holeX, holeY); // Q1 - w/ hole
            tile(half, centerX, y, centerX, centerY - 1);
            tile(half, x, centerY, centerX - 1, centerY);
            tile(half, centerX, centerY, centerX, centerY);
        } else {
            // center tile w/ hole top-right
            commands.push_back({1, tileX, tileY});

            tile(half, x, y, centerX - 1, centerY - 1);
            tile(half, centerX, y, holeX, holeY); // Q2 - w/ hole
            tile(half, x, centerY, centerX - 1, centerY);
            tile(half, centerX, centerY, centerX, centerY);
        }
    } else {
        if (holeX < centerX) {
            // center tile w/ hole bottom-left
            commands.push_back({2, tileX, tileY}); 

            tile(half, x, y, centerX - 1, centerY - 1);
            tile(half, centerX, y, centerX, centerY - 1); 
            tile(half, x, centerY, holeX, holeY); // Q3 - w/ hole
            tile(half, centerX, centerY, centerX, centerY);
        } else {
            // center tile w/ hole bottom-right
            commands.push_back({3, tileX, tileY});

            tile(half, x, y, centerX - 1, centerY - 1);
            tile(half, centerX, y, centerX, centerY - 1);
            tile(half, x, centerY, centerX - 1, centerY);
            tile(half, centerX, centerY, holeX, holeY); // Q4 - w/ hole
        }
    }
}


int main() {
    int l, x, y;
    cin >> l >> x >> y;
    
    tile(l, 0, 0, x, y);

    cout << commands.size() << endl;
    for (Command c : commands) cout << c.type << " " << c.x << " " << c.y << endl;
}