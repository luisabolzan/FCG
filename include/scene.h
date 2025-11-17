

#ifndef SCENE_H
#define SCENE_H

#include "globals.h"
#include "glad/glad.h"
#include "kart.h"

class Scene {
public:

    void Render(Kart& player1, Kart& player2);

private:

    void RenderSkySphere();
    void RenderGround();
    void RenderTrackPieces();
};

#endif //SCENE_H
