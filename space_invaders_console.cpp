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
    
