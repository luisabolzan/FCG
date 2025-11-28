#include "scene.h"

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
}


void Scene::Render() {

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
    player1.SetInputs(WPressed, SPressed, APressed, DPressed, SpacePressed);
    camera.UpdateProjectionMatrix(g_ScreenRatio);
    camera.StartCamera(player1);
    Render();
    HandleCollisions(*this);

    // Textos
    RenderTextSingleplayer(window);
}


void Scene::RenderMultiplayer(GLFWwindow* window, Camera& cameraP1, Camera& cameraP2) {

    glfwGetFramebufferSize(window, &g_ScreenWidth, &g_ScreenHeight);
    const int halfWidth = g_ScreenWidth / 2;
    const float splitRatio = (float)halfWidth / (float)g_ScreenHeight;

    // PLAYER 1
    player1.SetInputs(WPressed, SPressed, APressed, DPressed, SpacePressed);
    glViewport(0, 0, halfWidth, g_ScreenHeight); // Metade Esquerda
    cameraP1.UpdateProjectionMatrix(splitRatio);
    cameraP1.StartCamera(player1);
    Render();
    HandleCollisions(*this);

    // PLAYER 2
    player2.SetInputs(UpArrowPressed, DownArrowPressed, LeftArrowPressed, RightArrowPressed, RightShiftPressed);
    glViewport(halfWidth, 0, halfWidth, g_ScreenHeight); // Metade Direita
    cameraP2.UpdateProjectionMatrix(splitRatio);
    cameraP2.StartCamera(player2);
    Render();
    HandleCollisions(*this);

    // Textos
    RenderTextMultiplayer(window);
}


void Scene::RenderTextSingleplayer(GLFWwindow* window) {
    // tela cheia
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);
    glDisable(GL_DEPTH_TEST);

    RenderMatchTimer(window);
    RenderKartInfo(window, player1, -0.95f, 0.85f);
    RenderRanking(window, 0.75f, 0.8f);
    RenderSpeed(window, player1, -0.95f, -0.9f);
    RenderRespawnMessage(window, player1, "PLAYER 2", 0.0f, 0.15f);

    glEnable(GL_DEPTH_TEST);
}


void Scene::RenderTextMultiplayer(GLFWwindow* window) {
    // Para renderizar o texto sem distorções (Tela Cheia)
    glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);
    glDisable(GL_DEPTH_TEST);
    RenderMatchTimer(window);
    // --- PLAYER 1 (Lado Esquerdo) ---
    // Ajustei levemente o Ranking para -0.30f para não colar no meio, ajuste como preferir
    RenderKartInfo(window, player1, -0.95f, 0.85f);
    RenderRanking(window, -0.20f, 0.8f);
    RenderSpeed(window, player1, -0.95f, -0.9f);
    RenderRespawnMessage(window, player1, player2.name, -0.5f, 0.15f);

    // --- PLAYER 2 (Lado Direito) ---
    RenderKartInfo(window, player2, 0.05f, 0.85f);
    RenderRanking(window, 0.80f, 0.8f);
    RenderSpeed(window, player2, 0.05f, -0.9f);
    RenderRespawnMessage(window, player2, player1.name, 0.5f, 0.15f);

    glEnable(GL_DEPTH_TEST);
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

void Scene::RenderKartInfo(GLFWwindow* window, Kart& kart, float x_pos, float y_pos) {

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s | HP: %d | Ammo: %d | Score: %d",
             kart.name.c_str(), kart.health, kart.ammo, kart.score);

    float scale = 1.5f;
    TextRendering_PrintString(window, buffer, x_pos, y_pos, scale);
}

void Scene::RenderRanking(GLFWwindow* window, float x_pos, float y_pos) {

    char p1Text[64];
    char p2Text[64];
    float scale = 1.2f;

    Kart* primeiro;
    Kart* segundo;

    if (player1.score >= player2.score) {
        primeiro = &player1;
        segundo = &player2;
    } else {
        primeiro = &player2;
        segundo = &player1;
    }

    // Título
    TextRendering_PrintString(window, "RANKING:", x_pos, y_pos + 0.1f, scale);

    // 1º Lugar
    snprintf(p1Text, sizeof(p1Text), "1. %s (%d)", primeiro->name.c_str(), primeiro->score);
    TextRendering_PrintString(window, p1Text, x_pos, y_pos, scale);

    // 2º Lugar
    snprintf(p2Text, sizeof(p2Text), "2. %s (%d)", segundo->name.c_str(), segundo->score);
    TextRendering_PrintString(window, p2Text, x_pos, y_pos - 0.1f, scale);
}


void Scene::RenderSpeed(GLFWwindow* window, Kart& kart, float x_pos, float y_pos) {
    char buffer[64];

    int speedVal = (int)(std::abs(kart.speed) * 3.6f);
    snprintf(buffer, sizeof(buffer), "SPEED: %02d km/h", speedVal);

    float scale = 3.0f;
    TextRendering_PrintString(window, buffer, x_pos, y_pos, scale);
}


void Scene::RenderRespawnMessage(GLFWwindow* window, Kart& target, std::string shooter, float x_center, float y_center) {

    if (target.isAlive) return;

    int timeLeft = (int)(target.respawnTime - target.respawnTimer) + 1;

    char msgHit[64];
    char msgTimer[64];
    float scale = 3.0f;
    float char_width = TextRendering_CharWidth(window);

    snprintf(msgHit, sizeof(msgHit), "ELIMINADO POR %s", shooter.c_str());
    snprintf(msgTimer, sizeof(msgTimer), "RESPAWN EM: %d", timeLeft);

    float widthHit = strlen(msgHit) * char_width * scale;
    float x_hit = x_center - (widthHit / 2.0f);

    float widthTimer = strlen(msgTimer) * char_width * scale;
    float x_timer = x_center - (widthTimer / 2.0f);

    TextRendering_PrintString(window, msgHit, x_hit, y_center, scale);
    TextRendering_PrintString(window, msgTimer, x_timer, y_center - 0.15f, scale);
}

void Scene::RenderMatchTimer(GLFWwindow* window) {
    char buffer[32];
    
    int t = (int)ceil(RoundTime); 
    if (t < 0) t = 0;

    snprintf(buffer, sizeof(buffer), "TIME: %02d", t);

    float scale = 4.0f;
    
    float char_width = TextRendering_CharWidth(window);
    float text_width = char_width * strlen(buffer) * scale;
    float x_pos = 0.0f - (text_width / 2.0f);
    float y_pos = 0.90f;

    TextRendering_PrintString(window, buffer, x_pos, y_pos, scale);
}

void Scene::ResetMatch() {
    RoundTime = 60.0f;
    g_GameEnded = false;

    player1.position = player1.spawnPosition;
    player1.score = 0;
    player1.health = 100;
    player1.ammo = 10;
    player1.isAlive = true;
    player1.speed = 0.0f;
    player1.rockets.clear(); 

    player2.position = player2.spawnPosition;
    player2.score = 0;
    player2.health = 100;
    player2.ammo = 10;
    player2.isAlive = true;
    player2.speed = 0.0f;
    player2.rockets.clear();

    for(auto &c : coins) {
        c.active = true;
    }
}