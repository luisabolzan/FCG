#ifndef SCENE_H
#define SCENE_H

#include "globals.h"
#include "glad/glad.h"
#include "kart.h"
#include "coin.h"
#include "camera.h"
#include "FCGfunctions.h"
#include <cstdio>
#include <string>

class Scene {

public:
    ObjModel kartModel;
    Kart player1;
    Kart player2;
    std::vector<Coin> coins;
    std::vector<glm::vec3> palmPositions;
    std::vector<glm::vec3> treePositions;

    Scene();
    void Render();
    void RenderSinglePlayer(GLFWwindow* window, Camera& camera);
    void RenderMultiplayer(GLFWwindow* window, Camera& cam1, Camera& cam2);

    void RenderTextSingleplayer(GLFWwindow* window);
    void RenderTextMultiplayer(GLFWwindow* window);

    void RenderMatchTimer(GLFWwindow* window);
    void ResetMatch();

private:
    void RenderSkySphere();
    void RenderGround();

    void RenderPalm();
    void RenderTree();
    void RenderTrackPieces();
    void RenderCoins();

    void RenderKartInfo(GLFWwindow* window, Kart& kart, float x_pos, float y_pos);
    void RenderRanking(GLFWwindow* window, float x_pos, float y_pos);
    void RenderSpeed(GLFWwindow* window, Kart& kart, float x_pos, float y_pos);
    void RenderRespawnMessage(GLFWwindow* window, Kart& target, std::string shooter, float x_center, float y_center);
};

#endif //SCENE_H
