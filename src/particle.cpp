#include "particle.hpp"
#include <cmath>

ParticleSystem::ParticleSystem() {
    m_vertices.setPrimitiveType(sf::PrimitiveType::Points);
}

const float PI{3.14159365f};
float to_radians(float angleInDegrees) {
    return (angleInDegrees * PI) / 180.0f;
}

void ParticleSystem::emit(sf::Vector2f pos, size_t count) {
    m_particles.clear();
    m_vertices.resize(count);

    for (size_t i = 0; i < count; i++) {
        auto &vertex = m_vertices[i];

        float angle = to_radians(std::rand() % 360);
        float speed = (std::rand() % 150);

        Particle p;
        p.velocity =
            sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        // 0.5 to 1.5 seconds
        float life = 0.5f + static_cast<float>(std::rand() % 100) / 100.0f;
        p.lifetime = sf::seconds(life);
        p.maxLifetime = p.lifetime;

        m_particles.push_back(p);
        vertex.position = pos;
        vertex.color = sf::Color(255, 128, 0, 255);
    }
}

void ParticleSystem::update(sf::Time elapsed) {
    // update lifetime, position, color
    for (size_t i = 0; i < m_particles.size(); i++) {
        auto &particle = m_particles[i];
        auto &vertex = m_vertices[i];

        particle.lifetime -= elapsed;

        if (particle.lifetime <= sf::Time::Zero) {
            vertex.color.a = 0;
            continue;
        }

        vertex.position += particle.velocity * elapsed.asSeconds();

        float ratio =
            particle.lifetime.asSeconds() / particle.maxLifetime.asSeconds();

        vertex.color.r = static_cast<unsigned int>(255 * ratio);
        vertex.color.g = static_cast<unsigned int>(128 * ratio);
        vertex.color.a = static_cast<unsigned int>(255 * ratio);
    }
}

void ParticleSystem::draw(sf::RenderTarget &target,
                          sf::RenderStates states) const {
    target.draw(m_vertices, states);
}
