#include "control.h"
#include "audio.h"
#include "globals.h"

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        g_LeftMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_RightMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        g_RightMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_MiddleMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        g_MiddleMouseButtonPressed = false;
    }
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{

    if (!g_LeftMouseButtonPressed)
    {
        const float dx = xpos - g_LastCursorPosX;
        const float dy = ypos - g_LastCursorPosY;

        g_CameraTheta -= 0.002f * dx;
        g_CameraPhi   -= 0.002f * dy;

        const float phimax = 3.141592f / 2.0f;
        const float phimin = -phimax;

        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;

        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;

        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }

    if (g_RightMouseButtonPressed)
    {
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }

    if (g_MiddleMouseButtonPressed)
    {
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}


void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_CameraDistance -= 0.1f*yoffset;

    const float MIN_DISTANCE = 2.0f;
    const float MAX_DISTANCE = 10.0f;

    // Aplica as restrições
    if (g_CameraDistance < MIN_DISTANCE)
        g_CameraDistance = MIN_DISTANCE;

    if (g_CameraDistance > MAX_DISTANCE)
        g_CameraDistance = MAX_DISTANCE;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Mutar a música de fundo
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        AudioMute();
    
    // MENU
    if (g_ShowMenu) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // Opção 1: SINGLEPLAYER
        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            isMultiplayer = false;         // Define modo Single
            g_ShowMenu = false;            // Fecha o menu
            RoundTime = 60.0f;
            g_GameEnded = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Trava o mouse
        }

        // Opção 2: MULTIPLAYER
        if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
            isMultiplayer = true;          // Define modo Multi
            g_ShowMenu = false;            // Fecha o menu
            RoundTime = 60.0f;
            g_GameEnded = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Trava o mouse
        }
    }

    // FIM DE JOGO
    else if (g_GameEnded) {

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        if (key == GLFW_KEY_I && action == GLFW_PRESS) {
            g_ShowMenu = true;
        }
    }

    // JOGO
    else {

        // --- CONTROLES GERAIS ---
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
        {
            CPressed = !CPressed;
        }

        if (key == GLFW_KEY_I && action == GLFW_PRESS) {
            g_ShowMenu = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (key == GLFW_KEY_M && action == GLFW_PRESS)
        {
            MPressed = !MPressed;
            if (MPressed)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        if (key == GLFW_KEY_H && action == GLFW_PRESS)
        {
            g_ShowInfoText = !g_ShowInfoText;
        }

        if (key == GLFW_KEY_LEFT_SHIFT)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) LeftShiftPressed = true;
            else if (action == GLFW_RELEASE) LeftShiftPressed = false;
        }

        // --- CONTROLES PLAYER 1 (WASD + Space) ---

        if (key == GLFW_KEY_W)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) WPressed = true;
            else if (action == GLFW_RELEASE) WPressed = false;
        }

        if (key == GLFW_KEY_S)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) SPressed = true;
            else if (action == GLFW_RELEASE) SPressed = false;
        }

        if (key == GLFW_KEY_A)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) APressed = true;
            else if (action == GLFW_RELEASE) APressed = false;
        }

        if (key == GLFW_KEY_D)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) DPressed = true;
            else if (action == GLFW_RELEASE) DPressed = false;
        }

        if (key == GLFW_KEY_SPACE)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) SpacePressed = true;
            else if (action == GLFW_RELEASE) SpacePressed = false;
        }

        // --- CONTROLES PLAYER 2 (Setas + Right Shift) ---

        if (key == GLFW_KEY_UP)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) UpArrowPressed = true;
            else if (action == GLFW_RELEASE) UpArrowPressed = false;
        }

        if (key == GLFW_KEY_DOWN)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) DownArrowPressed = true;
            else if (action == GLFW_RELEASE) DownArrowPressed = false;
        }

        if (key == GLFW_KEY_LEFT)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) LeftArrowPressed = true;
            else if (action == GLFW_RELEASE) LeftArrowPressed = false;
        }

        if (key == GLFW_KEY_RIGHT)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) RightArrowPressed = true;
            else if (action == GLFW_RELEASE) RightArrowPressed = false;
        }

        if (key == GLFW_KEY_RIGHT_SHIFT)
        {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) RightShiftPressed = true;
            else if (action == GLFW_RELEASE) RightShiftPressed = false;
        }
    }
}