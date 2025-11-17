

#include "scene.h"

void Scene::Render(Kart& player1, Kart& player2) {
      RenderSkySphere();
      RenderGround();
      RenderTrackPieces();
      player1.Render();
      player2.Render();
      player2.dummy = true;
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
