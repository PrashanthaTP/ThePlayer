#pragma once
#include <SFML/Graphics.hpp>

struct Particle {
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Time lifetime;    // remaining time
    sf::Time maxLifetime; // total time
};

class ParticleSystem : public sf::Drawable {
  private:
    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;

    virtual void draw(sf::RenderTarget &target,
                      sf::RenderStates states) const override;

  public:
    ParticleSystem();
    void emit(sf::Vector2f pos, size_t count);
    void update(sf::Time deltatime);
};
