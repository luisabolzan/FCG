#include "coin.h"
#include "animation.h"
#include <cmath>

Coin::Coin( glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3) {
    active       = true;
    speed        = 0.5f;
    respawnTime  = 5.0f;
    respawnTimer = 0.0f;
    rotationY    = 0.0f;

    // Pontos específicos dessa moeda
    p0 = _p0;
    p1 = _p1;
    p2 = _p2;
    p3 = _p3;
}

void Coin::UpdateMovement() {

    if (!active) return;

    const float t_loop = fmod(currentTime * speed, 2.0f);
    float t;

    if (t_loop < 1.0f)
        t = t_loop; 
    else
        t = 2.0f - t_loop;

    const glm::vec3 bezierPos = Bezier3(p0, p1, p2, p3, t);
    
    position.x = bezierPos.x;
    position.z = bezierPos.z;
}

void Coin::Render() {

    this->UpdateMovement();

    position.y = -1.2f;

    if (!active) {
        respawnTimer += deltaTime;
        if (respawnTimer > respawnTime) {
            active = true;
            respawnTimer = 0.0f;
        }
        return;
    }

    glm::mat4 model = Matrix_Translate(position.x, position.y, position.z)
                    * Matrix_Rotate_Y((float)glfwGetTime())
                    * Matrix_Scale(0.5f, 0.5f, 0.5f);
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, COIN);
    DrawVirtualObject("the_coin");
}