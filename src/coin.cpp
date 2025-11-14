#include "coin.h"
#include <cmath>

Coin::Coin(const glm::vec4 pos) {
    position     = pos;
    active       = true;
    respawnTime  = 5.0f;
    respawnTimer = 0.0f;
    rotationY    = 0.0f;
}

void Coin::Render() {

    if (!active) {
        respawnTimer += deltaTime;
        if (respawnTimer > respawnTime) {
            active = true;
            respawnTimer = 0.0f;
        }
        return;
    }

    glm::mat4 model = Matrix_Translate(position.x, position.y, position.z)
                    * Matrix_Rotate_Y((float)glfwGetTime());
    glUniformMatrix4fv(g_model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(g_object_id_uniform, COIN);
    DrawVirtualObject("the_coin");
}