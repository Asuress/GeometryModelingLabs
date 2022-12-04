#pragma once

#include "MySnake.h"

namespace MySnake {
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
        mSnake.tails[0].dirX = 0;
        mSnake.tails[0].dirY = -1;
    }
    void DownKeyReaction() {
        mSnake.tails[0].dirX = 0;
        mSnake.tails[0].dirY = 1;
    }
    void LeftKeyReaction() {
        mSnake.tails[0].dirX = -1;
        mSnake.tails[0].dirY = 0;
    }
    void RightKeyReaction() {
        mSnake.tails[0].dirX = 1;
        mSnake.tails[0].dirY = 0;
    }
    bool moveSnake(bool addNewElem) {
        if (checkCollision()) {
            return false;
        }
        
        if (addNewElem) {
            SnakeTail tail;
            tail.x = mSnake.tails[mSnake.tails.size() - 1].x;
            tail.y = mSnake.tails[mSnake.tails.size() - 1].y;
            tail.prevX = mSnake.tails[mSnake.tails.size() - 1].prevX;
            tail.prevY = mSnake.tails[mSnake.tails.size() - 1].prevY;
            mSnake.tails.push_back(tail);
            for (int i = 0; i < mSnake.tails.size(); i++) {
                if (i != mSnake.tails.size() - 1) {
                    mSnake.tails[i].prevX = mSnake.tails[i].x;
                    mSnake.tails[i].prevY = mSnake.tails[i].y;
                    if (i > 0) {
                        mSnake.tails[i].x = mSnake.tails[i - 1].prevX;
                        mSnake.tails[i].y = mSnake.tails[i - 1].prevY;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < mSnake.tails.size(); i++) {
                mSnake.tails[i].prevX = mSnake.tails[i].x;
                mSnake.tails[i].prevY = mSnake.tails[i].y;
                if (i > 0) {
                    mSnake.tails[i].x = mSnake.tails[i - 1].prevX;
                    mSnake.tails[i].y = mSnake.tails[i - 1].prevY;
                }
                else {
                    mSnake.tails[i].x += mSnake.tails[0].dirX;
                    mSnake.tails[i].y += mSnake.tails[0].dirY;
                }
            }
        }
    }
    bool checkCollision() {
        // check if out of playfield
        if (mSnake.tails[0].x + mSnake.tails[0].dirX > matrix.columns ||
            mSnake.tails[0].x + mSnake.tails[0].dirX < 1)
            return true;
        if (mSnake.tails[0].y + mSnake.tails[0].dirY > matrix.rows ||
            mSnake.tails[0].y + mSnake.tails[0].dirY < 1)
            return true;
        // check if collide with itself
        for (int i = 0; i < mSnake.tails.size(); i++) {
            if (mSnake.tails[0].x + mSnake.tails[0].dirX == mSnake.tails[i].x &&
                mSnake.tails[0].y + mSnake.tails[0].dirY == mSnake.tails[i].y)
                return true;
        }
        return false;
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

        ImGui::Begin("Score window", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("Your score: %i", Score);
        ImGui::Text("x pos: %i", mSnake.tails[0].x);
        ImGui::Text("y pos: %i", mSnake.tails[0].y);
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
        ImGui::Text("tails %i", mSnake.tails.size());
        ImGui::End();
    }
    void renderGameLostGUI() {
        ImVec2 size;
        ImVec2 pos;
        size.x = 2 * WindowWidth / 3.0;
        size.y = 2 * WindowHeight / 3.0;
        pos.x = WindowWidth / 2 - size.x / 2;
        pos.y = WindowHeight / 2 - size.y / 2;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(pos);

        ImGui::Begin("GAME LOST", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
        ImGui::TextColored(ImVec4(2, 2, 2, 2), "GAME LOST");
        ImGui::Text("GAME LOST");
        ImGui::End();
    }
}