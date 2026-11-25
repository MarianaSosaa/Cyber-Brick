#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class SpriteRenderer {
public:
    // Constructor que recibe el shader base
    SpriteRenderer(Shader& shader) : shader(shader) {
        this->initRenderData();
    }

    // Destructor para limpiar memoria de OpenGL
    ~SpriteRenderer() {
        glDeleteVertexArrays(1, &this->quadVAO);
    }

    // Dibuja un sprite con posición, tamaño, rotación y color neón
    void DrawSprite(glm::vec2 position, glm::vec2 size, float rotate = 0.0f, glm::vec3 color = glm::vec3(0.0f, 0.94f, 1.0f)) {
        // Activar shader
        this->shader.use();

        // Configurar transformaciones (Matriz Model)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f)); // Mover a la posición (X, Y)

        // Rotación alrededor del origen del sprite
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

        // Escalar al tamaño deseado (ancho y alto)
        model = glm::scale(model, glm::vec3(size, 1.0f));

        this->shader.setMat4("model", model);
        this->shader.setVec3("spriteColor", color);

        // Dibujar el Quad
        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

private:
    Shader &shader;
    unsigned int quadVAO;

    // Inicializa la geometría del rectángulo (Quad) base
    void initRenderData() {
        // Configurar vértices y coordenadas de textura (pos.x, pos.y, tex.x, tex.y)
        float vertices[] = {
            // Pos      // Tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        unsigned int VBO;
        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif
