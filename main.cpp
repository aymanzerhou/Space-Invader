#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

class Player {
private:
    sf::ConvexShape shape;
    float speed;
    
public:
    Player(float x, float y) {
        shape.setPointCount(8);
        shape.setPoint(0, sf::Vector2f(20, 0));
        shape.setPoint(1, sf::Vector2f(30, 10));
        shape.setPoint(2, sf::Vector2f(30, 20));
        shape.setPoint(3, sf::Vector2f(25, 25));
        shape.setPoint(4, sf::Vector2f(15, 25));
        shape.setPoint(5, sf::Vector2f(10, 20));
        shape.setPoint(6, sf::Vector2f(10, 10));
        shape.setPoint(7, sf::Vector2f(20, 0));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
        speed = 5.0f;
    }
    
    void moveLeft() {
        if (shape.getPosition().x > 0) {
            shape.move(-speed, 0);
        }
    }
    
    void moveRight() {
        if (shape.getPosition().x < 760) {
            shape.move(speed, 0);
        }
    }
    
    sf::Vector2f getPosition() {
        return shape.getPosition();
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

class Bullet {
private:
    sf::RectangleShape shape;
    float speed;
    bool active;
    
public:
    Bullet(float x, float y) {
        shape.setSize(sf::Vector2f(4, 10));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Yellow);
        speed = 7.0f;
        active = true;
    }
    
    void update() {
        if (active) {
            shape.move(0, -speed);
            if (shape.getPosition().y < 0) {
                active = false;
            }
        }
    }
    
    bool isActive() {
        return active;
    }
    
    sf::FloatRect getBounds() {
        return shape.getGlobalBounds();
    }
    
    void deactivate() {
        active = false;
    }
    
    void draw(sf::RenderWindow& window) {
        if (active) {
            window.draw(shape);
        }
    }
};

class Alien {
private:
    sf::ConvexShape shape;
    bool active;
    
public:
    Alien(float x, float y) {
        shape.setPointCount(12);
        shape.setPoint(0, sf::Vector2f(15, 0));
        shape.setPoint(1, sf::Vector2f(25, 5));
        shape.setPoint(2, sf::Vector2f(25, 10));
        shape.setPoint(3, sf::Vector2f(20, 15));
        shape.setPoint(4, sf::Vector2f(25, 20));
        shape.setPoint(5, sf::Vector2f(20, 20));
        shape.setPoint(6, sf::Vector2f(15, 15));
        shape.setPoint(7, sf::Vector2f(10, 20));
        shape.setPoint(8, sf::Vector2f(5, 20));
        shape.setPoint(9, sf::Vector2f(10, 15));
        shape.setPoint(10, sf::Vector2f(5, 10));
        shape.setPoint(11, sf::Vector2f(5, 5));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
        active = true;
    }
    
    void move(float dx, float dy) {
        if (active) {
            shape.move(dx, dy);
        }
    }
    
    bool isActive() {
        return active;
    }
    
    sf::FloatRect getBounds() {
        return shape.getGlobalBounds();
    }
    
    void destroy() {
        active = false;
    }
    
    void draw(sf::RenderWindow& window) {
        if (active) {
            window.draw(shape);
        }
    }
};

class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::vector<Bullet> bullets;
    std::vector<Alien> aliens;
    std::vector<sf::RectangleShape> stars;
    int score;
    float alienDirection;
    float alienSpeed;
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    bool gameOver;
    
public:
    Game() : player(380, 520) {
        window.create(sf::VideoMode(800, 600), "Space Invaders");
        score = 0;
        alienDirection = 1.0f;
        alienSpeed = 0.5f;
        gameOver = false;
        
        // Load font (using default font for simplicity)
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            // Fallback to built-in font if Arial is not available
            font.loadFromFile("arial.ttf");
        }
        
        // Setup score text
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
        
        // Setup game over text
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(40);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("GAME OVER\nPress R to Restart");
        gameOverText.setPosition(250, 250);
        
        // Create starfield background
        srand(time(0));
        for (int i = 0; i < 100; i++) {
            sf::RectangleShape star(sf::Vector2f(2, 2));
            star.setPosition(rand() % 800, rand() % 600);
            star.setFillColor(sf::Color(255, 255, 255, rand() % 155 + 100));
            stars.push_back(star);
        }
        
        // Create alien grid
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 11; col++) {
                aliens.push_back(Alien(100 + col * 50, 50 + row * 40));
            }
        }
    }
    
    void handleInput() {
        if (gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                restart();
            }
            return;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.moveLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.moveRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            static sf::Clock shootClock;
            if (shootClock.getElapsedTime().asMilliseconds() > 300) {
                sf::Vector2f playerPos = player.getPosition();
                bullets.push_back(Bullet(playerPos.x + 18, playerPos.y));
                shootClock.restart();
            }
        }
    }
    
    void update() {
        if (gameOver) return;
        
        // Update bullets
        for (auto& bullet : bullets) {
            bullet.update();
        }
        
        // Remove inactive bullets
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.isActive(); }), bullets.end());
        
        // Move aliens
        bool changeDirection = false;
        for (auto& alien : aliens) {
            if (alien.isActive()) {
                sf::Vector2f pos = alien.getBounds().getPosition();
                if (pos.x <= 0 || pos.x >= 770) {
                    changeDirection = true;
                    break;
                }
            }
        }
        
        if (changeDirection) {
            alienDirection *= -1;
            for (auto& alien : aliens) {
                alien.move(0, 20);
            }
        }
        
        for (auto& alien : aliens) {
            alien.move(alienDirection * alienSpeed, 0);
        }
        
        // Check collisions
        for (auto& bullet : bullets) {
            if (bullet.isActive()) {
                for (auto& alien : aliens) {
                    if (alien.isActive() && bullet.getBounds().intersects(alien.getBounds())) {
                        bullet.deactivate();
                        alien.destroy();
                        score += 10;
                    }
                }
            }
        }
        
        // Check win condition
        bool allAliensDestroyed = true;
        for (auto& alien : aliens) {
            if (alien.isActive()) {
                allAliensDestroyed = false;
                break;
            }
        }
        
        if (allAliensDestroyed) {
            gameOver = true;
            gameOverText.setString("YOU WIN!\nPress R to Restart");
            gameOverText.setFillColor(sf::Color::Green);
        }
        
        // Check lose condition (aliens reach player)
        for (auto& alien : aliens) {
            if (alien.isActive()) {
                sf::Vector2f pos = alien.getBounds().getPosition();
                if (pos.y >= 500) {
                    gameOver = true;
                    gameOverText.setString("GAME OVER\nPress R to Restart");
                    gameOverText.setFillColor(sf::Color::Red);
                    break;
                }
            }
        }
    }
    
    void restart() {
        score = 0;
        gameOver = false;
        alienDirection = 1.0f;
        alienSpeed = 0.5f;
        
        // Clear bullets and aliens
        bullets.clear();
        aliens.clear();
        
        // Reset player position
        player = Player(380, 520);
        
        // Recreate alien grid
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 11; col++) {
                aliens.push_back(Alien(100 + col * 50, 50 + row * 40));
            }
        }
        
        // Reset game over text
        gameOverText.setString("GAME OVER\nPress R to Restart");
        gameOverText.setFillColor(sf::Color::Red);
    }
    
    void draw() {
        window.clear(sf::Color::Black);
        
        // Draw starfield
        for (auto& star : stars) {
            window.draw(star);
        }
        
        player.draw(window);
        
        for (auto& bullet : bullets) {
            bullet.draw(window);
        }
        
        for (auto& alien : aliens) {
            alien.draw(window);
        }
        
        // Update score text
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);
        
        // Draw game over text if needed
        if (gameOver) {
            window.draw(gameOverText);
        }
        
        window.display();
    }
    
    void run() {
        sf::Clock clock;
        
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            
            handleInput();
            update();
            draw();
            
            sf::sleep(sf::milliseconds(16));
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
