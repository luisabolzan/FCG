#ifndef SCENE_H
#define SCENE_H

#include "globals.h"
#include "glad/glad.h"
#include "kart.h"
#include "coin.h"
#include "FCGfunctions.h"

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

private:
    void RenderSkySphere();
    void RenderGround();

    void RenderPalm();
    void RenderTree();
    void RenderTrackPieces();
    void RenderCoins();
};

#endif //SCENE_H
