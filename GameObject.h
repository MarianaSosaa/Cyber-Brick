#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "SpriteRenderer.h"

class GameObject {
public:
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color, OriginalColor;
    float       Rotation;
    bool        IsSolid;     // Bloque indestructible
    bool        IsExplosive; // Bloque explosivo
    bool        Destroyed;
    int         HitPoints;   // Vida restante (1 para estándar, 2 o 3 para blindados)
    int         MaxHitPoints;

    GameObject()
        : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f, 0.0f), Color(1.0f), OriginalColor(1.0f),
        Rotation(0.0f), IsSolid(false), IsExplosive(false), Destroyed(false), HitPoints(1), MaxHitPoints(1) {
    }

    GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), int hp = 1, bool solid = false, bool explosive = false)
        : Position(pos), Size(size), Velocity(velocity), Color(color), OriginalColor(color),
        Rotation(0.0f), IsSolid(solid), IsExplosive(explosive), Destroyed(false), HitPoints(hp), MaxHitPoints(hp) {
    }

    // Aplica daño al recibir un impacto y atenúa/cambia la intensidad del color neón
    void TakeDamage() {
        if (!IsSolid && HitPoints > 0) {
            HitPoints--;
            if (HitPoints <= 0) {
                Destroyed = true;
            }
            else {
                // Atenúa la intensidad neón proporcionalmente al daño recibido
                float factor = static_cast<float>(HitPoints) / static_cast<float>(MaxHitPoints);
                Color = OriginalColor * factor;
            }
        }
    }

    virtual void Draw(SpriteRenderer& renderer) {
        if (!Destroyed) {
            renderer.DrawSprite(this->Position, this->Size, this->Rotation, this->Color);
        }
    }
};

#endif
