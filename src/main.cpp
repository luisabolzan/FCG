/*=========================================================================================
 *                    Trabalho Final de Fundamentos de Computação Gráfica
 *                      Luísa Righi Bolzan e Rafael Silveira Bandeira
 =========================================================================================*/
#include <iostream>
#include "FCGfunctions.h"
#include "globals.h"
#include "camera.h"
#include "kart.h"
#include "collisions.h"
#include "scene.h"
#include "menu.h"
#include "control.h"
#include "miniaudio.h"

int main(int argc, char* argv[]) {
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
    window = glfwCreateWindow(800, 600, "INF01047 - Smash Karts", NULL, NULL);
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
    FramebufferSizeCallback(window, 800, 600);

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

    //============================================================================================
    //                                Inicialização do Áudio
    //============================================================================================
    if (ma_engine_init(NULL, &g_AudioEngine) != MA_SUCCESS) {
        std::cerr << "Erro ao iniciar o engine de áudio.\n";
    }
    ma_result result = ma_sound_init_from_file(&g_AudioEngine, "../../data/audio/LadyJane.mp3", MA_SOUND_FLAG_STREAM, NULL, NULL, &g_Music);
    ma_sound_set_volume(&g_Music, 0.2f);
    ma_sound_set_looping(&g_Music, MA_TRUE);
    ma_sound_start(&g_Music);

    
    //============================================================================================
    //                                Criação da Camera e Cenário
    //============================================================================================

    Camera camera;
    Scene scene;

    // Loop infinito até que o usuário feche a janela
    while (!glfwWindowShouldClose(window)) {

        if (g_ShowMenu)
            RenderMenu(window);

        else {
            // Inicializa e mantém o funcionamento da camera
            camera.StartCamera(scene.player1);
            scene.Render();
            HandleCollisions(scene);
        }

        TextRendering_ShowFramesPerSecond(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ma_engine_uninit(&g_AudioEngine);
    glfwTerminate();
    return 0;
}