#pragma once
#include <SFML/Graphics.hpp>

struct Particle {
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;    // remaining time
    float maxLifetime; // total time
};

class ParticleSystem {
  private:
    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;

  public:
    ParticleSystem();
    void emitExplosion(sf::Vector2f pos, int count);
    void update(float deltatime);
    void draw(sf::RenderWindow &window);
};
