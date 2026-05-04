import tkinter as tk
import random
import math

class SpaceInvadersGame:
    def __init__(self, root):
        self.root = root
        self.root.title("Space Invaders")
        self.root.configure(bg='black')
        
        # Game constants
        self.WIDTH = 800
        self.HEIGHT = 600
        self.PLAYER_SPEED = 5
        self.BULLET_SPEED = 7
        self.ALIEN_SPEED = 0.5
        
        # Create canvas
        self.canvas = tk.Canvas(root, width=self.WIDTH, height=self.HEIGHT, bg='black', highlightthickness=0)
        self.canvas.pack()
        
        # Game variables
        self.player_x = self.WIDTH // 2
        self.player_y = self.HEIGHT - 50
        self.bullets = []
        self.aliens = []
        self.stars = []
        self.score = 0
        self.alien_direction = 1
        self.game_over = False
        self.won = False
        self.shoot_cooldown = 0
        
        # Initialize game
        self.create_stars()
        self.create_aliens()
        self.create_player()
        self.create_score_display()
        
        # Bind controls
        self.root.bind('<Left>', lambda e: self.move_player(-1))
        self.root.bind('<Right>', lambda e: self.move_player(1))
        self.root.bind('<space>', lambda e: self.shoot())
        self.root.bind('<r>', lambda e: self.restart() if self.game_over else None)
        self.root.bind('<R>', lambda e: self.restart() if self.game_over else None)
        
        # Start game loop
        self.game_loop()
        
    def create_stars(self):
        for _ in range(100):
            x = random.randint(0, self.WIDTH)
            y = random.randint(0, self.HEIGHT)
            brightness = random.randint(100, 255)
            color = f'#{brightness:02x}{brightness:02x}{brightness:02x}'
            size = random.choice([1, 2])
            star = self.canvas.create_oval(x, y, x+size, y+size, fill=color, outline='')
            self.stars.append(star)
            
    def create_player(self):
        # Create spaceship shape
        points = [
            self.player_x, self.player_y - 15,
            self.player_x + 20, self.player_y + 15,
            self.player_x + 10, self.player_y + 15,
            self.player_x, self.player_y,
            self.player_x - 10, self.player_y + 15,
            self.player_x - 20, self.player_y + 15
        ]
        self.player = self.canvas.create_polygon(points, fill='green', outline='')
        
    def create_aliens(self):
        for row in range(5):
            for col in range(11):
                x = 100 + col * 50
                y = 50 + row * 40
                # Create alien shape
                points = [
                    x, y,
                    x + 25, y + 5,
                    x + 25, y + 10,
                    x + 20, y + 15,
                    x + 25, y + 20,
                    x + 20, y + 20,
                    x + 15, y + 15,
                    x + 10, y + 20,
                    x + 5, y + 20,
                    x + 10, y + 15,
                    x + 5, y + 10,
                    x + 5, y + 5
                ]
                alien = self.canvas.create_polygon(points, fill='white', outline='')
                self.aliens.append({'id': alien, 'x': x, 'y': y, 'active': True})
                
    def create_score_display(self):
        self.score_text = self.canvas.create_text(10, 10, text=f"Score: {self.score}", 
                                                   fill='white', anchor='nw', font=('Arial', 14))
        self.instructions_text = self.canvas.create_text(self.WIDTH - 10, 10, 
                                                         text="Arrow Keys: Move | Space: Shoot | R: Restart", 
                                                         fill='white', anchor='ne', font=('Arial', 10))
        
