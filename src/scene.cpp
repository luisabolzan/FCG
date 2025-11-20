#include "scene.h"

Scene::Scene()
    : kartModel("../../data/kart/kart.obj"),
      player1("Player1", kartModel, glm::vec4(0.0f, -1.4f, 0.0f, 1.0f)),
      player2("Enemy", kartModel, glm::vec4(5.0f, -1.4f, 0.0f, 1.0f))
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
}


void Scene::Render() {

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(g_GpuProgramID);

    RenderSkySphere();
    RenderGround();
    RenderPalm();
    RenderTree();
    RenderTrackPieces();
    RenderCoins();

    player1.Render();
    player2.Render();
    player2.dummy = true;

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

    glm::mat4 model = Matrix_Translate(18.0f, -0.3f, 15.0f) * Matrix_Scale(2.0f, 2.0f, 2.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, PALM);
    DrawVirtualObject("the_palm");

    model = Matrix_Translate(-18.0f, -0.3f, -8.0f) * Matrix_Scale(2.0f,2.0f,2.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, PALM);
    DrawVirtualObject("the_palm");

    model = Matrix_Translate(18.0f, -0.3f, -8.0f) * Matrix_Scale(2.0f,2.0f,2.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, PALM);
    DrawVirtualObject("the_palm");

    model = Matrix_Translate(-18.0f, -0.3f, 15.0f) * Matrix_Scale(2.0f,2.0f,2.0f);

    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, PALM);
    DrawVirtualObject("the_palm");
}


void Scene::RenderTree() {

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IlluminationModel"), ILLUMINATION_BLINNPHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "IsGouraudShading"), true);

    glm::mat4 model = Matrix_Translate(23.0f, 4.0f, 15.0f) * Matrix_Scale(5.0f, 5.0f, 5.0f);
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, TREEL);
    DrawVirtualObject("tree_leaves");
    glUniform1i(g_object_id_uniform, TREEW);
    DrawVirtualObject("tree_wood");

    model = Matrix_Translate(-23.0f, 4.0f, -8.0f) * Matrix_Scale(5.0f,5.0f,5.0f);
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, TREEL);
    DrawVirtualObject("tree_leaves");
    glUniform1i(g_object_id_uniform, TREEW);
    DrawVirtualObject("tree_wood");

    model = Matrix_Translate(23.0f, 4.0f, -8.0f) * Matrix_Scale(5.0f,5.0f,5.0f);
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, TREEL);
    DrawVirtualObject("tree_leaves");
    glUniform1i(g_object_id_uniform, TREEW);
    DrawVirtualObject("tree_wood");

    model = Matrix_Translate(-23.0f, 4.0f, 15.0f) * Matrix_Scale(5.0f,5.0f,5.0f);
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, TREEL);
    DrawVirtualObject("tree_leaves");
    glUniform1i(g_object_id_uniform, TREEW);
    DrawVirtualObject("tree_wood");
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