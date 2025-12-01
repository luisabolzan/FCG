#include "scene.h"

#include "collisions.h"
#include "gametext.h"

Scene::Scene()
    : kartModel("../../data/kart/kart.obj"),
      player1("Player1", kartModel, glm::vec4(-20.0f, -1.4f, 3.0f, 1.0f)),
      player2("Player2", kartModel, glm::vec4(20.0f, -1.4f, 3.0f, 1.0f))
{


    // Carrega todas as texturas
    LoadTextureImage("../../data/sky/sky.hdr");                                   // TextureImage0
    LoadTextureImage("../../data/floor/grass.jpg");                               // TextureImage1
    LoadTextureImage("../../data/coin/coin.jpg");                                 // TextureImage2
    LoadTextureImage("../../data/kart/kart.jpg");                                 // TextureImage3
    LoadTextureImage("../../data/rocket/rocket.jpg");                             // TextureImage4
    LoadTextureImage("../../data/floor/trackTexture/City_Road_Sidewalk.jpg");     // TextureImage5
    LoadTextureImage("../../data/palm/palm.jpg");                                 // TextureImage6
    LoadTextureImage("../../data/tree/leaves.jpg");                               // TextureImage7
    LoadTextureImage("../../data/tree/wood.jpg");                                 // TextureImage8
    LoadTextureImage("../../data/frog_skin.jpg");                                 // TextureImage9
    LoadTextureImage("../../data/readme/smash_logo.png");                         // TextureImage10

    // Carrega todos os .obj
    ObjModel spheremodel("../../data/sky/sphere.obj");
    ComputeNormals(&spheremodel);
    BuildTrianglesAndAddToVirtualScene(&spheremodel);

    ObjModel bunnymodel("../../data/bunny.obj");
    ComputeNormals(&bunnymodel);
    BuildTrianglesAndAddToVirtualScene(&bunnymodel);

    ObjModel planemodel("../../data/floor/plane.obj");
    ComputeNormals(&planemodel);
    BuildTrianglesAndAddToVirtualScene(&planemodel);

    ObjModel trackmodel("../../data/floor/trackObj/Modular_Roads_Pack.obj");
    ComputeNormals(&trackmodel);
    BuildTrianglesAndAddToVirtualScene(&trackmodel);

    ObjModel palmmodel("../../data/palm/palm.obj");
    ComputeNormals(&palmmodel);
    BuildTrianglesAndAddToVirtualScene(&palmmodel);

    ObjModel treemodel("../../data/tree/tree.obj");
    ComputeNormals(&treemodel);
    BuildTrianglesAndAddToVirtualScene(&treemodel);

    ObjModel rocketmodel("../../data/rocket/rocket.obj");
    ComputeNormals(&rocketmodel);
    BuildTrianglesAndAddToVirtualScene(&rocketmodel);

    ObjModel coinModel("../../data/coin/coin.obj");
    ComputeNormals(&coinModel);
    BuildTrianglesAndAddToVirtualScene(&coinModel);

    ComputeNormals(&kartModel);
    BuildTrianglesAndAddToVirtualScene(&kartModel);

    // MOEDAS NA PISTA
    coins.emplace_back(
      glm::vec3(0.0f, 0.0f, 22.0f),   // P0
      glm::vec3(-3.0f, 0.0f, 25.0f),  // P1
      glm::vec3(3.0f, 0.0f, 29.0f),   // P2
      glm::vec3(0.0f, 0.0f, 32.0f)    // P3
    );
    coins.emplace_back(
      glm::vec3(0.0f, 0.0f, -15.0f),
      glm::vec3(3.0f, 0.0f, -18.0f),
      glm::vec3(-3.0f, 0.0f, -22.0f),
      glm::vec3(0.0f, 0.0f, -25.0f)
    );
    coins.emplace_back(
      glm::vec3(55.0f, 0.0f, 0.0f),
      glm::vec3(58.0f, 0.0f, 0.0f),
      glm::vec3(58.0f, 0.0f, 5.0f),
      glm::vec3(55.0f, 0.0f, 10.0f)
    );
    coins.emplace_back(
      glm::vec3(-55.0f, 0.0f, 0.0f),
      glm::vec3(-58.0f, 0.0f, 0.0f),
      glm::vec3(-58.0f, 0.0f, 5.0f),
      glm::vec3(-55.0f, 0.0f, 10.0f)
    );

    // Palmeiras
    palmPositions = {
        { 18.0f, -0.3f,  15.0f},
        {-18.0f, -0.3f,  -8.0f},
        { 18.0f, -0.3f,  -8.0f},
        {-18.0f, -0.3f,  15.0f}
    };

    // Árvores
    treePositions = {
        { 23.0f, 4.0f,  15.0f},
        {-23.0f, 4.0f,  -8.0f},
        { 23.0f, 4.0f,  -8.0f},
        {-23.0f, 4.0f,  15.0f}
    };

    enemyAI = new EnemyAI(&player2, &player1, &coins);
}


void Scene::UpdateScene() {

    player1.SetInputs(WPressed, SPressed, APressed, DPressed, SpacePressed);
    player1.Update();

    if (g_CurrentGameMode == MODE_MULTIPLAYER) {
        player2.SetInputs(UpArrowPressed, DownArrowPressed, LeftArrowPressed, RightArrowPressed, RightShiftPressed);
    }
    else if (g_CurrentGameMode == MODE_VERSUS_AI) {
        enemyAI->UpdateAI();
    }
    else {
        player2.SetInputs(false, false, false, false, false);
    }

    player2.Update();

    HandleCollisions(*this);
    for (auto & c : coins) c.UpdateMovement();
}


void Scene::RenderScene() {

    RenderSkySphere();
    RenderGround();
    RenderPalm();
    RenderTree();
    RenderTrackPieces();
    RenderCoins();
    player1.Render();
    player2.Render();
}

void Scene::RenderSinglePlayer(GLFWwindow* window, Camera& camera) {

    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);

    // Cenário
    camera.UpdateProjectionMatrix(g_ScreenRatio);
    camera.StartCamera(player1);
    RenderScene();

    // Textos
    RenderSinglePlayerHUD(window, player1, player2, CurrentRoundTime);
}


void Scene::RenderMultiplayer(GLFWwindow* window, Camera& cameraP1, Camera& cameraP2) {

    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);
    const int halfWidth = g_ScreenWidth / 2;
    const float splitRatio = (float)halfWidth / (float)g_ScreenHeight;

    // PLAYER 1
    glViewport(0, 0, halfWidth, g_ScreenHeight); // Metade Esquerda
    cameraP1.UpdateProjectionMatrix(splitRatio);
    cameraP1.StartCamera(player1);
    RenderScene();

    // PLAYER 2
    glViewport(halfWidth, 0, halfWidth, g_ScreenHeight); // Metade Direita
    cameraP2.UpdateProjectionMatrix(splitRatio);
    cameraP2.StartCamera(player2);
    RenderScene();

    // Textos
    RenderMultiPlayerHUD(window, player1, player2, CurrentRoundTime);
}


void Scene::RenderSkySphere() {

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_GLOBAL);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), false);

    glCullFace(GL_FRONT);
    glDepthMask(GL_FALSE);

    glm::mat4 model = Matrix_Translate(0.0f, 0.0f, 0.0f)
                      * Matrix_Scale(200.0f, 200.0f, 200.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, SPHERE);
    DrawVirtualObject("the_sphere");

    glDepthMask(GL_TRUE);
    glCullFace(GL_BACK);
}


void Scene::RenderGround() {

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_BLINNPHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), false);

    glm::mat4 model = Matrix_Translate(0.0f, -2.1f, 0.0f)
                      * Matrix_Scale(100.0f, 0.0f, 100.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, PLANE);
    DrawVirtualObject("the_plane");
}


void Scene::RenderPalm() {

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_LAMBERT);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), true);

    for (const auto& pos : palmPositions) {
        glm::mat4 model = Matrix_Translate(pos.x, pos.y, pos.z) * Matrix_Scale(2.0f, 2.0f, 2.0f);
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        DrawVirtualObject("the_palm");
    }
}


void Scene::RenderTree() {

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_BLINNPHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), true);

    for (const auto& pos : treePositions) {
        glm::mat4 model = Matrix_Translate(pos.x, pos.y, pos.z)
                        * Matrix_Scale(5.0f, 5.0f, 5.0f);
        glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(g_object_id_uniform, TREEL);
        DrawVirtualObject("tree_leaves");
        glUniform1i(g_object_id_uniform, TREEW);
        DrawVirtualObject("tree_wood");
    }
}


void Scene::RenderTrackPieces() {

    glm::mat4 model;
    const float track_scale = 0.005f;

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_BLINNPHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), false);

    // Reta 1
    model = Matrix_Translate(0.0f, -2.0f, 27.0f)
        * Matrix_Scale(track_scale, track_scale, track_scale);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, RACETRACK);
    DrawVirtualObject("Road_1X_Straight");

    // Reta 2
    model = Matrix_Translate(0.0f, -2.0f, -19.55f)
        * Matrix_Scale(track_scale, track_scale, track_scale);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, RACETRACK);
    DrawVirtualObject("Road_1X_Straight");

    // Curva direita
    model = Matrix_Translate(40.2f, -2.0f, 3.8f)
        * Matrix_Scale(track_scale, track_scale, track_scale)
        * Matrix_Rotate_Y(3.14159265359f / 2.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, RACETRACK);
    DrawVirtualObject("Road_1X_HalfCircle");

    // Curva esquerda
    model = Matrix_Translate(-40.2f, -2.0f, 3.8f)
        * Matrix_Scale(track_scale, track_scale, track_scale)
        * Matrix_Rotate_Y(-3.14159265359f / 2.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, RACETRACK);
    DrawVirtualObject("Road_1X_HalfCircle");
}

void Scene::RenderCoins() {
    for (auto & c : coins) c.Render();
}


// Reseta todas as variáveis do jogo
void Scene::ResetScene() {

    CurrentRoundTime = RoundTime;
    GameEnded = false;

    WPressed = false;
    APressed = false;
    SPressed = false;
    DPressed = false;
    UpArrowPressed = false;
    DownArrowPressed = false;
    LeftArrowPressed = false;
    RightArrowPressed = false;
    SpacePressed = false;
    RightShiftPressed = false;
    g_LeftMouseButtonPressed = false;
    g_RightMouseButtonPressed = false;

    player1.position = player1.spawnPosition;
    player1.rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    player1.direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    player1.score = 0;
    player1.health = 100;
    player1.ammo = 10;
    player1.isAlive = true;
    player1.speed = 0.0f;
    player1.isInvincible = false;
    player1.invincibleTimer = 0.0f;
    player1.respawnTimer = 0.0f;
    player1.rockets.clear();
    player1.SetInputs(false, false, false, false, false);

    player2.position = player2.spawnPosition;
    player2.rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    player2.direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    player2.score = 0;
    player2.health = 100;
    player2.ammo = 10;
    player2.isAlive = true;
    player2.speed = 0.0f;
    player2.isInvincible = false;
    player2.invincibleTimer = 0.0f;
    player2.respawnTimer = 0.0f;
    player2.rockets.clear();
    player2.SetInputs(false, false, false, false, false);

    for(auto &c : coins) {
        c.active = true;
    }

    StopGameSounds();
}