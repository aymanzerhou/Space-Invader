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
