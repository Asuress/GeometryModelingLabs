namespace MySnake {
    void UpKeyReaction();
    void DownKeyReaction();
    void LeftKeyReaction();
    void RightKeyReaction();
    bool moveSnake();
    void HandleKeyDownEvent(const SDL_Event* e);
    void renderGameGUI();
    void renderPauseMenu();
    int main(int argc, char** argv);

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

    struct Snake {
        int x = 1;
        int y = 1;
        int prevX;
        int prevY;
        int dirX;
        int dirY;
    };

    struct Item {
        int x;
        int y;

        void GenerateNewPosition(Matrix matrix) {
            x = (rand() % matrix.columns) + 1;
            y = (rand() % matrix.rows) + 1;
        }
    };

    int WindowWidth = 800;
    int WindowHeight = 600;
    int Score = 0;
    bool mIsPause = true;
    Snake mSnake;
    Matrix matrix;
    Item mItem;
}