#include "scene.h"


Scene::Scene()
    : kartModel("../../data/kart/kart.obj"),
      player1("Player1", kartModel, glm::vec4(0.0f, -1.4f, 0.0f, 1.0f)),
      player2("Enemy", kartModel, glm::vec4(5.0f, -1.4f, 0.0f, 1.0f)),
      coin(glm::vec4(0.0f, -1.4f, 0.0f, 1.0f))
{

      // Carrega todas as texturas
      LoadTextureImage("../../data/sky/sky.hdr");                                   // TextureImage0
      LoadTextureImage("../../data/floor/grass.jpg");                               // TextureImage1
      LoadTextureImage("../../data/coin/coin.jpg");                                 // TextureImage2
      LoadTextureImage("../../data/kart/kart.jpg");                                 // TextureImage3
      LoadTextureImage("../../data/rocket/rocket.jpg");                             // TextureImage4
      LoadTextureImage("../../data/floor/trackTexture/City_Road_Sidewalk.jpg");     // TextureImage5

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

      ObjModel rocketmodel("../../data/rocket/rocket.obj");
      ComputeNormals(&rocketmodel);
      BuildTrianglesAndAddToVirtualScene(&rocketmodel);

      ObjModel coinModel("../../data/coin/coin.obj");
      ComputeNormals(&coinModel);
      BuildTrianglesAndAddToVirtualScene(&coinModel);

      ComputeNormals(&kartModel);
      BuildTrianglesAndAddToVirtualScene(&kartModel);
}


void Scene::Render() {

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(g_GpuProgramID);

      RenderSkySphere();
      RenderGround();
      RenderTrackPieces();
      player1.Render();
      player2.Render();
      player2.dummy = true;

      coin.Update(glfwGetTime());
      float coinGroundHeight = GetHeightAt(coin.position.x, coin.position.z);
      coin.Render(coin.position, coinGroundHeight);
}


void Scene::RenderSkySphere() {

      glm::mat4 model;

      glCullFace(GL_FRONT);
      glDepthMask(GL_FALSE);

      model = Matrix_Translate(0.0f, 0.0f, 0.0f)
            * Matrix_Scale(200.0f, 200.0f, 200.0f);

      glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
      glUniform1i(g_object_id_uniform, SPHERE);
      DrawVirtualObject("the_sphere");

      glDepthMask(GL_TRUE);
      glCullFace(GL_BACK);
}


void Scene::RenderGround() {

      glm::mat4 model;

      model = Matrix_Translate(0.0f, -2.1f, 0.0f)
            * Matrix_Scale(100.0f, 0.0f, 100.0f);

      glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
      glUniform1i(g_object_id_uniform, PLANE);
      DrawVirtualObject("the_plane");
}


void Scene::RenderTrackPieces() {

    glm::mat4 model;
    float track_scale = 0.005f;

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

float Scene::GetHeightAt(float x, float z)
{
    (void)x; (void)z;
    return -2.0f;
}
