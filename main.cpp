#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_init.h>
#include "Shader.h"
#include <glm/vec3.hpp> // glm::vec3

void handle_window_resize(const int width, const int height) {
    const float new_aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    glViewport(0, 0, width, height);
}

void KeyDown(SDL_Scancode key) {
    switch (key) {
        case SDL_SCANCODE_W: {
            break;
        }
        case SDL_SCANCODE_S: {
            break;
        }
        case SDL_SCANCODE_D: {
            break;
        }
        case SDL_SCANCODE_A: {
            break;
        }
        case SDL_SCANCODE_SPACE: {
            break;
        }
        case SDL_SCANCODE_LSHIFT: {
            break;
        }
        case SDL_SCANCODE_RIGHT: {
            break;
        }
        case SDL_SCANCODE_LEFT: {
            break;
        }
        case SDL_SCANCODE_UP: {
            break;
        }
        case SDL_SCANCODE_DOWN: {
            break;
        }
        default: {
            break;
        }
    }
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    bool keepRunning = true;
    unsigned int shaderProgram = 0;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glm::vec3 a{1,2,3};


    constexpr int screenWidth = 800, screenHeight = 600;
    // Create window
    SDL_Window *window = SDL_CreateWindow("Hello World - VAO and VBO", screenWidth, screenHeight,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Failed to create SDL window. Error :" << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext open_gl_context = SDL_GL_CreateContext(window);
    if (!open_gl_context) {
        std::cerr << "Failed to create OpenGL context. Error : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (GLenum err = glewInit(); err != GLEW_OK) {
        std::cerr << "glewInitFailed: " << glewGetErrorString(err) << std::endl;
        SDL_GL_DestroyContext(open_gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Enable VSync
    SDL_GL_SetSwapInterval(1);

    glViewport(0, 0, 800, 600);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // Enable MSAA
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Request 4x MSAA (adjust as needed)
    glEnable(GL_MULTISAMPLE);

    shaderProgram = InitializeProgram("program1");

    GLfloat triangle[9 + 9] = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0,

        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    
    unsigned int indices[] = {0, 1, 2};
    
    GLuint triangle_vao;
    glGenVertexArrays(1, &triangle_vao);

    GLuint triangle_vbo;
    glGenBuffers(1, &triangle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glBindVertexArray(triangle_vao);

    GLuint triangle_ebo;
    glGenBuffers(1, &triangle_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);


    // Define the vertex attributes (position)
    constexpr int vertex_pos_index = 0;
    glVertexAttribPointer(vertex_pos_index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(vertex_pos_index);

    //Define the vertex attributes (color)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(9 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Rendering loop
    while (keepRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_WINDOW_RESIZED: {
                    handle_window_resize(event.window.data1, event.window.data2);
                    break;
                }
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
                    keepRunning = false;
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    KeyDown(event.key.scancode);
                    break;
                }
                default: {
                    break;
                }
            }
        }
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        //clear the render depth buffer         
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(triangle_vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
        glUseProgram(0);
        
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &triangle_vao);
    glDeleteBuffers(1, &triangle_vbo);
    glDeleteBuffers(1, &triangle_vbo);

    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(open_gl_context);
    SDL_Quit();

    return 0;
}
