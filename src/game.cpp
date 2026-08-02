// TODO:
// Game window -> creation at desktop center
// Unlimited coin respawning
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "coin.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "game_states.hpp"
#include "particle.hpp"
#include "platforms.hpp"
#include "rng.hpp"

namespace ThePlayer {

static std::vector<Platform> platforms;
static PlatformManager platformManager(4);
static CoinManager coinManager(4);
static ParticleSystem particleSystem;

static unsigned int score = 0;

// TODO: avoid dangling pointer
// std::unique_ptr<sf::Text> scoreTextPtr;
sf::Font font;
sf::Text scoreText(font);

sf::RectangleShape addBoundingBox(const sf::RenderWindow &window,
                                  const sf::Sprite sprite) {
    sf::FloatRect rect = sprite.getGlobalBounds();
    sf::RectangleShape resRect(sf::Vector2f(rect.size.x, rect.size.y));
    resRect.setOutlineColor(sf::Color::Red);
    resRect.setFillColor(sf::Color::Transparent);
    resRect.setOutlineThickness(2.0f);
    resRect.setPosition({rect.position.x, rect.position.y});
    return resRect;
}

Engine::Engine() {
}

Engine::~Engine() {
}

// TODO! Refine direction resolution logic
[[nodiscard]] CollisionDirection
Engine::findCollisionDirection(const Player &player, const Platform &platform) {
    const sf::Vector2f size = player.getSize();
    const sf::Vector2f pos = player.getPosition();
    float leftX = pos.x;
    float rightX = pos.x + size.x;
    float topY = pos.y;
    float bottomY = pos.y + size.y;

    float pLeftX = platform.getPosition().x;
    float pRightX = pLeftX + platform.getSize().x;
    float pTopY = platform.getPosition().y;
    float pBottomY = pTopY + platform.getSize().y;

    float overlapFromLeftX = rightX - pLeftX;
    float overlapFromRightX = pRightX - leftX;
    float overlapFromTopY = bottomY - pTopY;
    float overlapFromBottomY = pBottomY - topY;

    float res = GSettings.width + 10.0f;
    CollisionDirection resDir = CollisionDirection::FROM_LEFT;
    if (overlapFromLeftX && overlapFromLeftX < overlapFromRightX) {
        if (overlapFromLeftX < res) {
            resDir = CollisionDirection::FROM_LEFT;
            res = overlapFromLeftX;
        }
    }
    if (overlapFromRightX && overlapFromRightX < overlapFromLeftX) {
        if (overlapFromRightX < res) {
            resDir = CollisionDirection::FROM_RIGHT;
            res = overlapFromRightX;
        }
    }
    if (overlapFromTopY && overlapFromTopY < overlapFromBottomY) {
        if (overlapFromTopY < res) {
            resDir = CollisionDirection::FROM_TOP;
            res = overlapFromTopY;
        }
    }
    if (overlapFromBottomY && overlapFromBottomY < overlapFromTopY) {
        if (overlapFromBottomY < res) {
            resDir = CollisionDirection::FROM_BOTTOM;
            res = overlapFromBottomY;
        }
    }
    return resDir;
}

void Engine::handleCollisions(Player &player) {
    const sf::Vector2f size = player.getSize();
    const sf::Vector2f pos = player.getPosition();
    float leftX = pos.x;
    float rightX = pos.x + size.x;
    float topY = pos.y;
    float bottomY = pos.y + size.y;
    for (auto &platform : platformManager.getPlatforms()) {

        float pLeftX = platform.getPosition().x;
        float pRightX = pLeftX + platform.getSize().x;
        float pTopY = platform.getPosition().y;
        float pBottomY = pTopY + platform.getSize().y;
        bool xColliding = rightX >= pLeftX && leftX <= pRightX;
        bool yColliding = topY <= pBottomY && bottomY >= pTopY;
        if (xColliding && yColliding) {
            CollisionDirection direction =
                findCollisionDirection(player, platform);
            player.handleCollision(platform, direction);
        }
    }

    sf::FloatRect playerBb = player.getDrawObj().getGlobalBounds();
    // coin collision
    for (auto &coin : coinManager.getCoins()) {
        // check for collision
        // notify Coin/CoinManager of collision
        const sf::FloatRect coinBb = coin.getGlobalBounds();
        if (const std::optional<sf::FloatRect> overlap =
                coinBb.findIntersection(playerBb)) {
            particleSystem.emit(coin.getPosition(), 1000);
            coin.notifyCollision();
            score += coin.getValue();
            // if(scoreTextPtr) {
            scoreText.setString("Score: " + std::to_string(score));
            //}
        }
    }

    if (bottomY >= GSettings.groundY) {
        player.handleGroundCollision();
    }
}

[[nodiscard]] InputState Engine::handleKeyPress() {
    InputState inputState;
    inputState.left = false;
    inputState.right = false;
    inputState.up = false;
    inputState.down = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        inputState.left = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        inputState.right = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        inputState.up = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        inputState.up = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        inputState.left = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        inputState.right = true;
    }
    return inputState;
}

void setCursorToType(sf::RenderWindow &window, sf::Cursor::Type cursorType) {
    // BUG!! -> Local scope cursor -> should live as long as the `window` for
    // proper functionality
    const auto cursor = sf::Cursor::createFromSystem(cursorType).value();
    window.setMouseCursor(cursor);
}

bool checkIfCursorWithinBounds(sf::RenderWindow &window,
                               const sf::RectangleShape &shape) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);
    sf::FloatRect bounds = shape.getGlobalBounds();
    return bounds.contains(mousePosF);
}

void Engine::run() {
    sf::RenderWindow window(sf::VideoMode({GSettings.width, GSettings.height}),
                            GSettings.name,
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!font.openFromFile("assets/fonts/ComicNeue-Regular.ttf")) {
        std::cerr << "Unable to open font\n";
        return exit(EXIT_FAILURE);
    }

    sf::Text text(font);
    text.setString("Start");
    text.setCharacterSize(21);
    text.setFillColor(sf::Color::Black);

    // sf::Text scoreText(font);
    // scoreTextPtr.reset(&scoreText);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(21);
    scoreText.setFillColor(sf::Color(255, 255, 255, 255));
    scoreText.setPosition({GSettings.width - 150.0f, 5.0f});

    // Set text's origin to its own center
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                    textRect.position.y + textRect.size.y / 2.0f});

    sf::RectangleShape groundLine({GSettings.width * 1.0f, 2});
    groundLine.setFillColor(sf::Color::Red);
    groundLine.setPosition({0, GSettings.groundY});

    sf::RectangleShape startButton({100, 30});
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition(
        {GSettings.width / 2.0f - 50.0f, GSettings.height / 2.0f - 15.0f});

    sf::Vector2f rectPos = startButton.getPosition();
    sf::Vector2f rectSize = startButton.getSize();
    text.setPosition(
        {rectPos.x + rectSize.x / 2.0f, rectPos.y + rectSize.y / 2.0f});

    sf::Texture bgTexture;

    if (!bgTexture.loadFromFile("assets/bg.png")) {
        std::cerr << "Error loading background image;\n";
        exit(EXIT_FAILURE);
    }

    sf::Sprite bgSprite(bgTexture);
    bgSprite.setPosition({0.0f, 0.0f});

    sf::Texture startBgTexture;
    if (!startBgTexture.loadFromFile("assets/start.png")) {
        std::cerr << "Error loading start background image;\n";
        exit(EXIT_FAILURE);
    }
    sf::Sprite startBgSprite(startBgTexture);
    startBgSprite.setPosition({0.0f, 0.0f});

    sf::Clock clock;

    PlayerPhysics physics = {
        .velocity = {0.0f, 0.0f},
        .pos = {(float)GSettings.width / 2 - 15.0f, GSettings.groundY - 60.0f},
        .size = {30.0f, 60.0f},
        .jumpforce = 450.0f,
        .friction = 0.9f,
        .gravity = 800.0f,
        .acceleration = 900.0f,
        .velocityLimits = {-400.0f, 400.0f},
        .groundY = GSettings.groundY};
    Player player(physics);

    GameState gameState = GameState::GS_FIRST_SCREEN;

    std::optional<sf::Cursor> cursor;
    sf::Vector2f mouseClickPosition;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto *mouseEvent =
                    event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    mouseClickPosition =
                        window.mapPixelToCoords(mouseEvent->position);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            window.close();
        }

        /*if(event->is<sf::Event::MouseButtonPressed>()){

        }*/
        window.clear();
        // setCursorToType(window, sf::Cursor::Type::Arrow);
        cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);
        window.setMouseCursor(cursor.value());
        //
        float time = clock.restart().asSeconds();
        switch (gameState) {
        case GameState::GS_FIRST_SCREEN:
            if (checkIfCursorWithinBounds(window, startButton)) {
                // setCursorToType(window, sf::Cursor::Type::Hand);
                cursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);
                window.setMouseCursor(cursor.value());

                if (startButton.getGlobalBounds().contains(
                        mouseClickPosition)) {
                    gameState = GameState::GS_START;
                }
            } else {
                // setCursorToType(window, sf::Cursor::Type::Arrow);
            }
            window.draw(startBgSprite);
            window.draw(startButton);
            window.draw(text);
            break;
        case GameState::GS_START: {

            platformManager.update(window, time);
            InputState inputState = handleKeyPress();
            player.update(inputState);
            particleSystem.update(sf::seconds(time));
            handleCollisions(player);

            // window.draw(bgSprite);

            window.draw(player.getDrawObj());
            window.draw(groundLine);

            platformManager.draw(window);
            coinManager.draw(window);
            window.draw(scoreText);
            window.draw(particleSystem);
            break;
        }
        default:
            break;
        }

        window.display();
    }
    // auto temp = scoreTextPtr.release();
}

} // namespace ThePlayer
