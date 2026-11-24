#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>
#include "shader.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "BallObject.h"
#include "GameLevel.h"

// Dimensiones de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, GameObject& paddle, BallObject& ball, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float velocity = 600.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (paddle.Position.x >= 0.0f) {
            paddle.Position.x -= velocity;
            if (ball.Stuck) ball.Position.x -= velocity;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (paddle.Position.x <= SCR_WIDTH - paddle.Size.x) {
            paddle.Position.x += velocity;
            if (ball.Stuck) ball.Position.x += velocity;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        ball.Stuck = false;
}

// Función que propaga una onda expansiva cuando un bloque explosivo es destruido
void TriggerExplosion(GameObject& explosiveBrick, std::vector<GameObject>& bricks) {
    float radius = explosiveBrick.Size.x * 1.5f; // Radio de cobertura de la explosión

    for (GameObject& brick : bricks) {
        if (!brick.Destroyed && !brick.IsSolid) {
            // Calcular distancia centro a centro
            glm::vec2 brickCenter = brick.Position + brick.Size * 0.5f;
            glm::vec2 expCenter = explosiveBrick.Position + explosiveBrick.Size * 0.5f;

            if (glm::distance(brickCenter, expCenter) <= radius) {
                brick.TakeDamage();
                // Si la explosión alcanza a otro bloque explosivo, encadenar la reacción
                if (brick.Destroyed && brick.IsExplosive) {
                    TriggerExplosion(brick, bricks);
                }
            }
        }
    }
}

// Detección de colisión 2D tipo AABB vs. Círculo
bool CheckCollision(BallObject& ball, GameObject& box) {
    glm::vec2 center(ball.Position + ball.Radius);
    glm::vec2 aabb_half_extents(box.Size.x / 2.0f, box.Size.y / 2.0f);
    glm::vec2 aabb_center(
        box.Position.x + aabb_half_extents.x,
        box.Position.y + aabb_half_extents.y
    );
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;
    return glm::length(difference) < ball.Radius;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cyber Brick - Arcade 2D", NULL, NULL);
    if (window == NULL) {
        std::cout << "Error al crear la ventana de GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cout << "Error al inicializar GLAD v2" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "¡OpenGL inicializado con éxito! Versión: "
        << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader myShader("shaders/sprite.vs", "shaders/sprite.fs");
    SpriteRenderer Renderer(myShader);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),
        static_cast<float>(SCR_HEIGHT), 0.0f,
        -1.0f, 1.0f);

    // Cargar nivel 1
    GameLevel level1;
    level1.Load("levels/level1.txt", SCR_WIDTH, static_cast<unsigned int>(SCR_HEIGHT * 0.4f));

    // Pala y pelota
    GameObject Paddle(glm::vec2(350.0f, 550.0f), glm::vec2(100.0f, 20.0f), glm::vec3(0.0f, 0.94f, 1.0f));
    BallObject Ball(glm::vec2(350.0f + 50.0f - 10.0f, 550.0f - 20.0f), 10.0f, glm::vec2(300.0f, -400.0f), glm::vec3(1.0f, 0.0f, 0.5f));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, Paddle, Ball, deltaTime);
        Ball.Move(deltaTime, SCR_WIDTH);

        // Colisión Pelota vs. Bloques
        if (!Ball.Stuck) {
            for (GameObject& box : level1.Bricks) {
                if (!box.Destroyed && CheckCollision(Ball, box)) {
                    // Aplicar daño si no es indestructible
                    if (!box.IsSolid) {
                        box.TakeDamage();
                        // Reacción en cadena si era explosivo y fue destruido
                        if (box.Destroyed && box.IsExplosive) {
                            TriggerExplosion(box, level1.Bricks);
                        }
                    }

                    // Invertir velocidad vertical de la pelota
                    Ball.Velocity.y = -Ball.Velocity.y;
                    break;
                }
            }
        }

        // Colisión Pelota vs. Pala
        if (!Ball.Stuck && CheckCollision(Ball, Paddle)) {
            float centerBoard = Paddle.Position.x + Paddle.Size.x / 2.0f;
            float distance = (Ball.Position.x + Ball.Radius) - centerBoard;
            float percentage = distance / (Paddle.Size.x / 2.0f);

            float strength = 2.0f;
            glm::vec2 oldVelocity = Ball.Velocity;
            Ball.Velocity.x = 400.0f * percentage * strength;
            Ball.Velocity.y = -1.0f * std::abs(Ball.Velocity.y);
            Ball.Velocity = glm::normalize(Ball.Velocity) * glm::length(oldVelocity);

            Ball.Position.y = Paddle.Position.y - Ball.Size.y;
        }

        // Reiniciar si la pelota cae por debajo de la pantalla
        if (Ball.Position.y >= SCR_HEIGHT) {
            Ball.Reset(glm::vec2(Paddle.Position.x + Paddle.Size.x / 2.0f - Ball.Radius, Paddle.Position.y - Ball.Size.y), glm::vec2(300.0f, -400.0f));
        }

        // Renderizado
        glClearColor(0.07f, 0.0f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.use();
        myShader.setMat4("projection", projectionMatrix);

        level1.Draw(Renderer);
        Paddle.Draw(Renderer);
        Ball.Draw(Renderer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
