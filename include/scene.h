

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
    Coin coin;

    Scene();
    void Render();

private:
    void RenderSkySphere();
    void RenderGround();
    void RenderTrackPieces();
};

#endif //SCENE_H
