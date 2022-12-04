#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_impl_sdl.h"

#include "MySnake.h"

using namespace MySnake;

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
    mSnake.tails[0].x = 1;
    mSnake.tails[0].y = 1;
    mSnake.tails[0].prevX = 1;
    mSnake.tails[0].prevY = 1;
    mSnake.tails[0].dirX = 1;
    mSnake.tails[0].dirY = 0;
    mItem.GenerateNewPosition(matrix, mSnake);

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
            mIsGameLost = !moveSnake(addNewTail);
            addNewTail = false;
        }

        if (mSnake.tails[0].x == mItem.x && mSnake.tails[0].y == mItem.y) {
            addNewTail = true;
            Score++;
            mItem.GenerateNewPosition(matrix, mSnake);
        }

        // UpdateImage
        {
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            if (!mIsPause) {
                //matrix.GetElement(mSnake.tails[0].prevY, mSnake.tails[0].prevX) = 0xaa000000;
                matrix.GetElement(mSnake.tails[mSnake.tails.size() - 1].prevY, mSnake.tails[mSnake.tails.size() - 1].prevX) = 0xaa000000;
                matrix.GetElement(mSnake.tails[0].y, mSnake.tails[0].x) = 0x00aa0000;
                matrix.GetElement(mItem.y, mItem.x) = 0xf423ff00;
            }

            SDL_UpdateTexture(texture, nullptr, matrix.arr.data(), textureWidth * sizeof(matrix.arr[0]));

            
            // Render Gui
            {
                if (mIsGameLost) {
                    renderGameLostGUI();
                } 
                else if (!mIsPause) {
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

//namespace MySnake {
//    void HandleKeyDownEvent(const SDL_Event* e)
//    {
//        switch (e->key.keysym.sym)
//        {
//        case SDL_KeyCode::SDLK_UP:
//            UpKeyReaction();
//            break;
//        case SDL_KeyCode::SDLK_DOWN:
//            DownKeyReaction();
//            break;
//        case SDL_KeyCode::SDLK_LEFT:
//            LeftKeyReaction();
//            break;
//        case SDL_KeyCode::SDLK_RIGHT:
//            RightKeyReaction();
//            break;
//        case SDL_KeyCode::SDLK_ESCAPE:
//            mIsPause = !mIsPause;
//            break;
//        default:
//            break;
//        }
//    }
//
//    void UpKeyReaction() {
//        mSnake.tails[0].dirX = 0;
//        mSnake.tails[0].dirY = -1;
//    }
//    void DownKeyReaction() {
//        mSnake.tails[0].dirX = 0;
//        mSnake.tails[0].dirY = 1;
//    }
//    void LeftKeyReaction() {
//        mSnake.tails[0].dirX = -1;
//        mSnake.tails[0].dirY = 0;
//    }
//    void RightKeyReaction() {
//        mSnake.tails[0].dirX = 1;
//        mSnake.tails[0].dirY = 0;
//    }
//    bool moveSnake(bool addNewElem) {
//        if (checkCollision()) {
//            return false;
//        }
//        
//        if (addNewElem) {
//            SnakeTail tail;
//            tail.x = mSnake.tails[mSnake.tails.size() - 1].x;
//            tail.y = mSnake.tails[mSnake.tails.size() - 1].y;
//            tail.prevX = mSnake.tails[mSnake.tails.size() - 1].prevX;
//            tail.prevY = mSnake.tails[mSnake.tails.size() - 1].prevY;
//            mSnake.tails.push_back(tail);
//            for (int i = 0; i < mSnake.tails.size(); i++) {
//                if (i != mSnake.tails.size() - 1) {
//                    mSnake.tails[i].prevX = mSnake.tails[i].x;
//                    mSnake.tails[i].prevY = mSnake.tails[i].y;
//                    if (i > 0) {
//                        mSnake.tails[i].x = mSnake.tails[i - 1].prevX;
//                        mSnake.tails[i].y = mSnake.tails[i - 1].prevY;
//                    }
//                }
//            }
//        }
//        else {
//            for (int i = 0; i < mSnake.tails.size(); i++) {
//                mSnake.tails[i].prevX = mSnake.tails[i].x;
//                mSnake.tails[i].prevY = mSnake.tails[i].y;
//                if (i > 0) {
//                    mSnake.tails[i].x = mSnake.tails[i - 1].prevX;
//                    mSnake.tails[i].y = mSnake.tails[i - 1].prevY;
//                }
//                else {
//                    mSnake.tails[i].x += mSnake.tails[0].dirX;
//                    mSnake.tails[i].y += mSnake.tails[0].dirY;
//                }
//            }
//        }
//    }
//    bool checkCollision() {
//        // check if out of playfield
//        if (mSnake.tails[0].x + mSnake.tails[0].dirX > matrix.columns ||
//            mSnake.tails[0].x + mSnake.tails[0].dirX < 1)
//            return true;
//        if (mSnake.tails[0].y + mSnake.tails[0].dirY > matrix.rows ||
//            mSnake.tails[0].y + mSnake.tails[0].dirY < 1)
//            return true;
//        // check if collide with itself
//        for (int i = 0; i < mSnake.tails.size(); i++) {
//            if (mSnake.tails[0].x + mSnake.tails[0].dirX == mSnake.tails[i].x &&
//                mSnake.tails[0].y + mSnake.tails[0].dirY == mSnake.tails[i].y)
//                return true;
//        }
//        return false;
//    }
//    void renderGameGUI() {
//        ImVec2 size;
//        ImVec2 pos;
//        size.x = WindowWidth;
//        size.y = WindowHeight / 6.0;
//        pos.x = 0;
//        pos.y = 0;
//        ImGui::SetNextWindowSize(size);
//        ImGui::SetNextWindowPos(pos);
//
//        ImGui::Begin("Score window", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
//            ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
//            ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground |
//            ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);
//        ImGui::Text("Your score: %i", Score);
//        ImGui::Text("x pos: %i", mSnake.tails[0].x);
//        ImGui::Text("y pos: %i", mSnake.tails[0].y);
//        ImGui::End();
//    }
//    void renderPauseMenu() {
//        ImVec2 size;
//        ImVec2 pos;
//        size.x = 2 * WindowWidth / 3.0;
//        size.y = 2 * WindowHeight / 3.0;
//        pos.x = WindowWidth / 2 - size.x / 2;
//        pos.y = WindowHeight / 2 - size.y / 2;
//        ImGui::SetNextWindowSize(size);
//        ImGui::SetNextWindowPos(pos);
//
//        ImGui::Begin("MENU", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
//            ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
//        ImGui::Text("MENU");
//        ImGui::Text("tails %i", mSnake.tails.size());
//        ImGui::End();
//    }
//    void renderGameLostGUI() {
//        ImVec2 size;
//        ImVec2 pos;
//        size.x = 2 * WindowWidth / 3.0;
//        size.y = 2 * WindowHeight / 3.0;
//        pos.x = WindowWidth / 2 - size.x / 2;
//        pos.y = WindowHeight / 2 - size.y / 2;
//        ImGui::SetNextWindowSize(size);
//        ImGui::SetNextWindowPos(pos);
//
//        ImGui::Begin("GAME LOST", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
//            ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
//        ImGui::TextColored(ImVec4(2, 2, 2, 2), "GAME LOST");
//        ImGui::Text("GAME LOST");
//        ImGui::End();
//    }
//}