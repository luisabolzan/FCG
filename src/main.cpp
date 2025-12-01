/*=========================================================================================
 *                    Trabalho Final de Fundamentos de Computação Gráfica
 *                      Luísa Righi Bolzan e Rafael Silveira Bandeira
 =========================================================================================*/

#include <iostream>
#include "FCGfunctions.h"
#include "globals.h"
#include "camera.h"
#include "scene.h"
#include "gametext.h"
#include "control.h"
#include "audio.h"

int main(int argc, char* argv[]) {

    // =========================================================================
    //  INICIALIZAÇÃO GLFW/GLAD E CONFIGURAÇÕES OPENGL
    // =========================================================================
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(ErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;
    window = glfwCreateWindow(g_ScreenWidth, g_ScreenHeight, "INF01047 - Smash Karts", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, g_ScreenWidth, g_ScreenHeight);

    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    LoadShadersFromFiles();

    TextRendering_Init();

    // Habilitamos o Z-buffer.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Sincroniza o FPS com a taxa de atualização do monitor
    glfwSwapInterval(1);

    // =========================================================================
    //  INICIALIZAÇÃO DO JOGO (Cena, Câmera, Áudio e Tempo)
    // =========================================================================

    Camera cameraP1;
    Camera cameraP2;
    Scene scene;
    AudioInit();

    // Reset inicial para garantir variáveis limpas
    scene.ResetScene();

    // Inicializa o tempo
    lastTime = (float)glfwGetTime();

    // =========================================================================
    // GAME LOOP PRINCIPAL
    // =========================================================================
    while (!glfwWindowShouldClose(window)) {

        // Cálculo do DeltaTime
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        currentTime = currentFrame;

        // Limpeza de Tela
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(g_GpuProgramID);

        // Máquina de Estados

        // ESTADO 1: MENU PRINCIPAL
        if (g_ShowMenu) {
            RenderMenu(window);
            scene.ResetScene();
        }

        // ESTADO 2: FIM DE JOGO
        else if (GameEnded) {
            StopGameSounds();
            RenderGameOver(window, scene.player1.score, scene.player2.score);
        }

        // ESTADO 3: JOGO RODANDO
        else {

            // Cronometro dos Rounds
            CurrentRoundTime -= deltaTime;
            if (CurrentRoundTime <= 0.0f) {
                CurrentRoundTime = 0.0f;
                GameEnded = true;
            }

            // Atualiza Física e Inputs
            scene.UpdateScene();

            // Renderiza Cena 3D + Interface (HUD)
            if (g_CurrentGameMode == MODE_MULTIPLAYER)
                scene.RenderMultiplayer(window, cameraP1, cameraP2);
            else
                scene.RenderSinglePlayer(window, cameraP1);
        }

        TextRendering_ShowFramesPerSecond(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // =========================================================================
    // FINALIZAÇÃO
    // =========================================================================
    AudioCleanup();
    glfwTerminate();
    return 0;
}