#include "coin.h"
#include "animation.h"
#include <cmath>

Coin::Coin(const glm::vec4 pos) {
    position     = pos;
    active       = true;
    respawnTime  = 5.0f;
    respawnTimer = 0.0f;
    rotationY    = 0.0f;
}

void Coin::Update(float time) {
    if (!active) return; 

    float speed = 0.5f; 
    float t_loop = fmod(time * speed, 2.0f);
    float t;

    if (t_loop < 1.0f)
        t = t_loop; 
    else
        t = 2.0f - t_loop;

    glm::vec3 bezierPos = Bezier3(p0, p1, p2, p3, t);
    
    position.x = bezierPos.x;
    position.z = bezierPos.z;
}

void Coin::Render(const glm::vec4 pos, float terrainHeight) {

    position.y = terrainHeight + 0.5f;

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