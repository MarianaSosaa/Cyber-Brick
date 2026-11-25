#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "SpriteRenderer.h"

class BallObject : public GameObject {
public:
    // Propiedades específicas de la pelota
    float   Radius;
    bool    Stuck;

    // Constructores
    BallObject()
        : GameObject(), Radius(12.5f), Stuck(true) {
    }

    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, glm::vec3 color)
        : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), color, velocity), Radius(radius), Stuck(true) {
    }

    // Mueve la pelota, maneja los rebotes con las paredes de la ventana.
    // Devuelve la posición actualizada.
    glm::vec2 Move(float dt, unsigned int window_width) {
        // Si no está pegada a la pala, actualizar posición con la velocidad
        if (!this->Stuck) {
            this->Position += this->Velocity * dt;

            // Colisión con el borde izquierdo
            if (this->Position.x <= 0.0f) {
                this->Velocity.x = -this->Velocity.x;
                this->Position.x = 0.0f;
            }
            // Colisión con el borde derecho (Ancho de ventana - tamaño de la pelota)
            else if (this->Position.x >= window_width - this->Size.x) {
                this->Velocity.x = -this->Velocity.x;
                this->Position.x = window_width - this->Size.x;
            }
            // Colisión con el borde superior
            if (this->Position.y <= 0.0f) {
                this->Velocity.y = -this->Velocity.y;
                this->Position.y = 0.0f;
            }
        }
        return this->Position;
    }

    // Resetea la posición de la pelota pegada a la pala
    void Reset(glm::vec2 position, glm::vec2 velocity) {
        this->Position = position;
        this->Velocity = velocity;
        this->Stuck = true;
    }
};

#endif
