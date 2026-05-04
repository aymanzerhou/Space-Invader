#include <windows.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 25;
const int PLAYER_X = 40;
const int PLAYER_Y = 22;

class Player {
private:
    int x, y;
    
public:
    Player() : x(PLAYER_X), y(PLAYER_Y) {}
    
    void moveLeft() {
        if (x > 2) x--;
    }
    
    void moveRight() {
        if (x < WIDTH - 3) x++;
    }
    
    int getX() const { return x; }
    int getY() const { return y; }
    
    void draw(HANDLE hConsole) {
        COORD pos = {x - 2, y};
        SetConsoleCursorPosition(hConsole, pos);
        SetConsoleTextAttribute(hConsole, 10); // Green
        cout << "<-^->";
    }
};

class Bullet {
private:
    int x, y;
    bool active;
    
public:
    Bullet(int px, int py) : x(px), y(py - 1), active(true) {}
    
    void update() {
        if (active && y > 0) {
            y--;
            if (y <= 0) active = false;
        }
    }
    
    bool isActive() const { return active; }
    
    int getX() const { return x; }
    int getY() const { return y; }
    
    void deactivate() { active = false; }
    
    void draw(HANDLE hConsole) {
        if (active) {
            COORD pos = {x, y};
            SetConsoleCursorPosition(hConsole, pos);
            SetConsoleTextAttribute(hConsole, 14); // Yellow
            cout << "|";
        }
    }
};

class Alien {
private:
    int x, y;
    bool active;
    
public:
    Alien(int px, int py) : x(px), y(py), active(true) {}
    
    void move(int dx, int dy) {
        if (active) {
            x += dx;
            y += dy;
        }
    }
    
    bool isActive() const { return active; }
    
    int getX() const { return x; }
    int getY() const { return y; }
    
    void destroy() { active = false; }

void draw(HANDLE hConsole) {
        if (active) {
            COORD pos = {x, y};
            SetConsoleCursorPosition(hConsole, pos);
            SetConsoleTextAttribute(hConsole, 12); // Red
            cout << "@";
        }
    }
};
class Alien {
private:
    int x, y;
    bool active;
    
public:
    Alien(int px, int py) : x(px), y(py), active(true) {}
    
    void move(int dx, int dy) {
        if (active) {
            x += dx;
            y += dy;
        }
    }
    
    bool isActive() const { return active; }
    
    int getX() const { return x; }
    int getY() const { return y; }
    
    void destroy() { active = false; }
    
    void draw(HANDLE hConsole) {
        if (active) {
            COORD pos = {x, y};
            SetConsoleCursorPosition(hConsole, pos);
            SetConsoleTextAttribute(hConsole, 12); // Red
            cout << "@";
        }
    }
};

class Game {
private:
    HANDLE hConsole;
    Player player;
    vector<Bullet> bullets;
    vector<Alien> aliens;
    int score;
    int alienDirection;
    bool gameOver;
    bool won;
    
public:
    Game() {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        score = 0;
        alienDirection = 1;
        gameOver = false;
        won = false;
        
        // Create alien grid
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 10; col++) {
                aliens.push_back(Alien(10 + col * 6, 3 + row * 2));
            }
        }
    }
    
    void clearScreen() {
        COORD topLeft = {0, 0};
        SetConsoleCursorPosition(hConsole, topLeft);
        SetConsoleTextAttribute(hConsole, 8); // Dark gray for border
        
        // Draw border
        for (int i = 0; i < WIDTH; i++) {
            cout << "#";
        }
        
        for (int i = 1; i < HEIGHT - 1; i++) {
            COORD pos = {0, i};
            SetConsoleCursorPosition(hConsole, pos);
            cout << "#";
            pos.X = WIDTH - 1;
            SetConsoleCursorPosition(hConsole, pos);
            cout << "#";
        }
        
        COORD pos = {0, HEIGHT - 1};
        SetConsoleCursorPosition(hConsole, pos);
        for (int i = 0; i < WIDTH; i++) {
            cout << "#";
        }
    }
    
    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 75) { // Left arrow
                player.moveLeft();
            } else if (key == 77) { // Right arrow
                player.moveRight();
            } else if (key == ' ') { // Space
                bullets.push_back(Bullet(player.getX(), player.getY()));
            } else if (key == 'r' || key == 'R') { // Restart
                if (gameOver) {
                    restart();
                }
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
        bullets.erase(remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.isActive(); }), bullets.end());
        
        // Move aliens
        bool changeDirection = false;
        for (auto& alien : aliens) {
            if (alien.isActive()) {
                if (alien.getX() <= 2 || alien.getX() >= WIDTH - 3) {
                    changeDirection = true;
                    break;
                }
            }
        }
if (changeDirection) {
            alienDirection *= -1;
            for (auto& alien : aliens) {
                alien.move(0, 1);
            }
        }
        
        for (auto& alien : aliens) {
            alien.move(alienDirection, 0);
        }
        
        // Check collisions
        for (auto& bullet : bullets) {
            if (bullet.isActive()) {
                for (auto& alien : aliens) {
                    if (alien.isActive() && 
                        bullet.getX() == alien.getX() && 
                        bullet.getY() == alien.getY()) {
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
            won = true;
        }
        
        // Check lose condition
        for (auto& alien : aliens) {
            if (alien.isActive() && alien.getY() >= player.getY() - 1) {
                gameOver = true;
                won = false;
                break;
            }
        }
    }
void draw() {
        clearScreen();
        
        // Draw game objects
        player.draw(hConsole);
        
        for (auto& bullet : bullets) {
            bullet.draw(hConsole);
        }
        
        for (auto& alien : aliens) {
            alien.draw(hConsole);
        }
        
        // Draw score
        COORD pos = {2, 0};
        SetConsoleCursorPosition(hConsole, pos);
        SetConsoleTextAttribute(hConsole, 15); // White
        cout << "Score: " << score;
        
        // Draw instructions
        pos = {WIDTH - 25, 0};
        SetConsoleCursorPosition(hConsole, pos);
        cout << "Arrows: Move Space: Shoot";
        
        // Draw game over message
        if (gameOver) {
            pos = {WIDTH / 2 - 10, HEIGHT / 2};
            SetConsoleCursorPosition(hConsole, pos);
            SetConsoleTextAttribute(hConsole, won ? 10 : 12); // Green for win, Red for lose
            cout << (won ? "YOU WIN!" : "GAME OVER!");
            
            pos = {WIDTH / 2 - 12, HEIGHT / 2 + 1};
            SetConsoleCursorPosition(hConsole, pos);
            SetConsoleTextAttribute(hConsole, 15);
            cout << "Press R to Restart";
        }
    }
    
    void restart() {
        score = 0;
        alienDirection = 1;
        gameOver = false;
        won = false;
        bullets.clear();
        aliens.clear();
        player = Player();
        
        // Recreate alien grid
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 10; col++) {
                aliens.push_back(Alien(10 + col * 6, 3 + row * 2));
            }
        }
    }
    
    void run() {
        // Setup console
        system("cls");
        SetConsoleTextAttribute(hConsole, 15);
        
        // Hide cursor
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
        
        // Game loop
        while (true) {
            handleInput();
            update();
            draw();
            Sleep(100); // Control game speed
            
            // Check for ESC key to exit
            if (_kbhit() && _getch() == 27) break;
        }
        
        // Show cursor before exit
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }
};

int main() {
    cout << "=== SPACE INVADERS ===" << endl;
    cout << "Controls:" << endl;
    cout << "Arrow Keys: Move left/right" << endl;
    cout << "Space: Shoot" << endl;
    cout << "R: Restart (when game over)" << endl;
    cout << "ESC: Exit game" << endl;
    cout << "Press any key to start..." << endl;
    _getch();
    
    Game game;
    game.run();
    
    return 0;
}

