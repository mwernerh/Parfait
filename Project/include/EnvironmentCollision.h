#pragma once
#include "SFML/Graphics/VertexArray.hpp"

struct EnvironmentCollision : public sf::VertexArray {
    EnvironmentCollision() : sf::VertexArray(sf::PrimitiveType::TriangleStrip) {}
};