#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"
#include "Lab1.h"

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

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL init falure\n";
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    SDL_Window* window = SDL_CreateWindow("Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cout << "Couldn't create window\n";
        return -1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Couldn't create renderer\n";
        return -1;
    }

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    size_t textureWidth = 11;
    size_t textureHeight = 12;
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING | SDL_TEXTUREACCESS_TARGET,
        textureWidth,
        textureHeight);
    std::vector<Uint32> bufferForUpdate(textureWidth * textureHeight);
    matrix.Init(textureHeight, textureWidth);
    Uint32 i = 0;
    for (auto& pixel : bufferForUpdate) {
        switch (i%3)
        {
        case 0:
            pixel = 0x00005aaa;
            break;
        case 1:
            pixel = 0x005a00aa;
            break;
        case 2:
            pixel = 0x5a0000aa;
            break;
        default:
            pixel = 0xaaaaaaaa;
            break;
        }
        i += 1;
    }
    for (size_t i = 1; i <= matrix.rows; i++)
    {
        for (size_t j = 1; j <= matrix.columns; j++)
        {
            matrix.GetElement(i, j) = 0xaa000000;
        }
    }
    mSnake.x = 1;
    mSnake.y = 1;
    mSnake.prevX = 1;
    mSnake.prevY = 1;
    mSnake.dirX = 1;
    mSnake.dirY = 0;
    mItem.GenerateNewPosition(matrix);

    SDL_UpdateTexture(texture, nullptr, bufferForUpdate.data(), textureWidth * sizeof(bufferForUpdate[0]));

    bool programWorks = true;
    int fps = 60;
    Uint64 trueFrameTime = 1000.f / fps;
    Uint32 frameInSecond = 0;
    while (programWorks)
    {
        SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);
        Uint64 frameTime = SDL_GetTicks();
        frameInSecond++;
        // HandleInputs
        {
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                ImGui_ImplSDL2_ProcessEvent(&e);
                if (e.type == SDL_QUIT)
                    programWorks = false;
                if (e.type == SDL_KEYDOWN)
                    HandleKeyDownEvent(&e);
            }
        }

        if (frameInSecond % 30 == 0 && !mIsPause) {
            if (!moveSnake()) {
                programWorks = false;
            }
        }

        if (mSnake.x == mItem.x && mSnake.y == mItem.y) {
            Score++;
            mItem.GenerateNewPosition(matrix);
        }

        // UpdateImage
        {
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            if (!mIsPause) {
                matrix.GetElement(mSnake.prevY, mSnake.prevX) = 0xaa000000;
                matrix.GetElement(mSnake.y, mSnake.x) = 0x00aa0000;
                matrix.GetElement(mItem.y, mItem.x) = 0xf423ff00;
            }

            SDL_UpdateTexture(texture, nullptr, matrix.arr.data(), textureWidth * sizeof(matrix.arr[0]));

            
            // Render Gui
            {
                if (!mIsPause) {
                    renderGameGUI();
                }
                else {
                    renderPauseMenu();
                }
            }

            ImGui::Render();

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);

            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer);
        }
        
        frameTime = SDL_GetTicks() - frameTime;
        if (frameTime < trueFrameTime)
            SDL_Delay(trueFrameTime - frameTime);
        if (frameInSecond == 60)
            frameInSecond = 0;
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

void HandleKeyDownEvent(const SDL_Event* e)
{
    switch (e->key.keysym.sym)
    {
    case SDL_KeyCode::SDLK_UP:
        UpKeyReaction();
        break;
    case SDL_KeyCode::SDLK_DOWN:
        DownKeyReaction();
        break;
    case SDL_KeyCode::SDLK_LEFT:
        LeftKeyReaction();
        break;
    case SDL_KeyCode::SDLK_RIGHT:
        RightKeyReaction();
        break;
    case SDL_KeyCode::SDLK_ESCAPE:
        mIsPause = !mIsPause;
        break;
    default:
        break;
    }
}

void UpKeyReaction() {
    mSnake.dirX = 0;
    mSnake.dirY = -1;
}
void DownKeyReaction() {
    mSnake.dirX = 0;
    mSnake.dirY = 1;
}
void LeftKeyReaction() {
    mSnake.dirX = -1;
    mSnake.dirY = 0;
}
void RightKeyReaction() {
    mSnake.dirX = 1;
    mSnake.dirY = 0;
}
bool moveSnake() {
    if (mSnake.x + mSnake.dirX > matrix.columns ||
        mSnake.x + mSnake.dirX < 1)
        return false;
    if (mSnake.y + mSnake.dirY > matrix.rows ||
        mSnake.y + mSnake.dirY < 1)
        return false;

    mSnake.prevX = mSnake.x;
    mSnake.prevY = mSnake.y;
    mSnake.x += mSnake.dirX;
    mSnake.y += mSnake.dirY;
}
void renderGameGUI() {
    ImVec2 size;
    ImVec2 pos;
    size.x = WindowWidth;
    size.y = WindowHeight / 6.0;
    pos.x = 0;
    pos.y = 0;
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("Score window", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize     |
                                           ImGuiWindowFlags_::ImGuiWindowFlags_NoMove       |
                                           ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground |
                                           ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);
    
    ImGui::Text("Your score: %i", Score);
    ImGui::End();
}
void renderPauseMenu() {
    ImVec2 size;
    ImVec2 pos;
    size.x = 2 * WindowWidth / 3.0;
    size.y = 2 * WindowHeight / 3.0;
    pos.x = WindowWidth / 2 - size.x / 2;
    pos.y = WindowHeight / 2 - size.y / 2;
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("MENU", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
    ImGui::Text("MENU");
    ImGui::End();
}