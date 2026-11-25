#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GameObject.h"
#include "SpriteRenderer.h"

class GameLevel {
public:
    std::vector<GameObject> Bricks;

    GameLevel() {}

    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
        this->Bricks.clear();
        unsigned int tileCode;
        std::string line;
        std::ifstream fstream(file);
        std::vector<std::vector<unsigned int>> tileData;

        if (fstream) {
            while (std::getline(fstream, line)) {
                std::istringstream sstream(line);
                std::vector<unsigned int> row;
                while (sstream >> tileCode) {
                    row.push_back(tileCode);
                }
                if (!row.empty()) {
                    tileData.push_back(row);
                }
            }
            if (!tileData.empty()) {
                this->init(tileData, levelWidth, levelHeight);
            }
        }
        else {
            std::cout << "ERROR::LEVEL: No se pudo abrir el archivo de nivel: " << file << std::endl;
        }
    }

    void Draw(SpriteRenderer& renderer) {
        for (GameObject& tile : this->Bricks) {
            if (!tile.Destroyed) {
                tile.Draw(renderer);
            }
        }
    }

    bool IsCompleted() {
        for (GameObject& tile : this->Bricks) {
            if (!tile.IsSolid && !tile.Destroyed) {
                return false;
            }
        }
        return true;
    }

private:
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
        unsigned int height = static_cast<unsigned int>(tileData.size());
        unsigned int width = static_cast<unsigned int>(tileData[0].size());

        float unit_width = levelWidth / static_cast<float>(width);
        float unit_height = levelHeight / static_cast<float>(height);
        float padding = 2.0f;

        const glm::vec3 colorSolid(0.75f, 0.78f, 0.85f);
        const glm::vec3 colorCyan(0.0f, 0.94f, 1.0f);
        const glm::vec3 colorYellow(1.0f, 0.95f, 0.1f);
        const glm::vec3 colorOrange(1.0f, 0.4f, 0.0f);
        const glm::vec3 colorExplosive(1.0f, 0.0f, 0.5f);

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                unsigned int code = tileData[y][x];
                if (code == 0) continue;

                glm::vec2 pos(unit_width * x + padding * 0.5f, unit_height * y + padding * 0.5f);
                glm::vec2 size(unit_width - padding, unit_height - padding);

                if (code == 1) {
                    // Indestructible
                    this->Bricks.push_back(GameObject(pos, size, colorSolid, glm::vec2(0.0f), 1, true, false));
                }
                else if (code == 2) {
                    // Estándar (1 golpe)
                    this->Bricks.push_back(GameObject(pos, size, colorCyan, glm::vec2(0.0f), 1, false, false));
                }
                else if (code == 3) {
                    // Blindado Ligero (2 golpes)
                    this->Bricks.push_back(GameObject(pos, size, colorYellow, glm::vec2(0.0f), 2, false, false));
                }
                else if (code == 4) {
                    // Blindado Pesado (3 golpes)
                    this->Bricks.push_back(GameObject(pos, size, colorOrange, glm::vec2(0.0f), 3, false, false));
                }
                else if (code == 5) {
                    // Explosivo (1 golpe + pulso en cadena)
                    this->Bricks.push_back(GameObject(pos, size, colorExplosive, glm::vec2(0.0f), 1, false, true));
                }
            }
        }
    }
};

#endif
