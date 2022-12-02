//#include "MySnake.h"
//
////namespace MySnake {
//    void MySnake::HandleKeyDownEvent(const SDL_Event* e)
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
//    void MySnake::UpKeyReaction() {
//        mSnake.dirX = 0;
//        mSnake.dirY = -1;
//    }
//    void MySnake::DownKeyReaction() {
//        mSnake.dirX = 0;
//        mSnake.dirY = 1;
//    }
//    void MySnake::LeftKeyReaction() {
//        mSnake.dirX = -1;
//        mSnake.dirY = 0;
//    }
//    void MySnake::RightKeyReaction() {
//        mSnake.dirX = 1;
//        mSnake.dirY = 0;
//    }
//    bool MySnake::moveSnake() {
//        if (mSnake.x + mSnake.dirX > matrix.columns ||
//            mSnake.x + mSnake.dirX < 1)
//            return false;
//        if (mSnake.y + mSnake.dirY > matrix.rows ||
//            mSnake.y + mSnake.dirY < 1)
//            return false;
//
//        mSnake.prevX = mSnake.x;
//        mSnake.prevY = mSnake.y;
//        mSnake.x += mSnake.dirX;
//        mSnake.y += mSnake.dirY;
//    }
//    void MySnake::renderGameGUI() {
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
//        ImGui::End();
//    }
//    void MySnake::renderPauseMenu() {
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
//        ImGui::End();
//    }
//    void MySnake::renderGameLostGUI() {
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
//            ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
//            ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);
//        ImGui::TextColored(ImVec4(2, 2, 2, 2), "GAME LOST");
//        ImGui::Text("GAME LOST");
//        ImGui::End();
//    }
////}