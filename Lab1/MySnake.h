#pragma once

//#ifndef MY_SNAKE
//#define MY_SNAKE

#include <SDL.h>
#include <vector>
#include <string>
#include <imgui.h>

namespace MySnake {
    void UpKeyReaction();
    void DownKeyReaction();
    void LeftKeyReaction();
    void RightKeyReaction();
    bool moveSnake(bool addNewElem);
    bool checkCollision();
    void HandleKeyDownEvent(const SDL_Event* e);
    void renderGameGUI();
    void renderPauseMenu();
    void renderGameLostGUI();

    struct Matrix {
    private:

    public:
        std::vector<Uint32> arr;
        int rows;
        int columns;

        void Init(int rows, int columns) {
            this->rows = rows;
            this->columns = columns;
            arr = std::vector<Uint32>(rows * columns);
        }

        Uint32& GetElement(int row, int col) {
            row--; col--;
            /*
            1 2 3
            4 5 6
            7 8 9

            [2,3] = 6 -- returns arr[1*3 + 2], aka 5th el., aka number 6 -- corr.
            [3,2] = 8 -- returns arr[2*3 + 1], aka 7th el., aka number 8 -- corr.
            */
            return arr[row * columns + col];
        }
    };

    struct SnakeTail {
        int x = 1;
        int y = 1;
        int prevX;
        int prevY;
        int dirX;
        int dirY;
    };

    struct Snake {
        std::vector<SnakeTail> tails = std::vector<SnakeTail>(1);
        /*int x = 1;
        int y = 1;
        int prevX;
        int prevY;
        int dirX;
        int dirY;*/
    };

    struct Item {
        int x;
        int y;

        void GenerateNewPosition(Matrix matrix, Snake mSnake) {
            x = (rand() % matrix.columns) + 1;
            y = (rand() % matrix.rows) + 1;
            bool leave = false;
            do {
                leave = false;
                for (int i = 0; i < mSnake.tails.size(); i++) {
                    if (x == mSnake.tails[i].x && y == mSnake.tails[i].y)
                    {
                        x = (rand() % matrix.columns) + 1;
                        y = (rand() % matrix.rows) + 1;
                        leave = false;
                        i = 0;
                    }
                    else
                    {
                        leave = true;
                    }
                }
            } while (!leave);
        }
    };

    int WindowWidth = 800;
    int WindowHeight = 600;
    int Score = 0;
    bool mIsPause = true;
    bool mIsGameLost = false;
    bool addNewTail = false;
    Snake mSnake;
    Matrix matrix;
    Item mItem;
}

//#endif // MY_SNAKE