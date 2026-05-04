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
           def move_player(self, direction):
        if not self.game_over:
            new_x = self.player_x + direction * self.PLAYER_SPEED
            if 20 <= new_x <= self.WIDTH - 20:
                self.player_x = new_x
                self.canvas.delete(self.player)
                self.create_player()
                
    def shoot(self):
        if not self.game_over and self.shoot_cooldown <= 0:
            bullet = self.canvas.create_rectangle(self.player_x - 2, self.player_y - 20, 
                                                 self.player_x + 2, self.player_y - 10, 
                                                 fill='yellow', outline='')
            self.bullets.append({'id': bullet, 'x': self.player_x, 'y': self.player_y - 20, 'active': True})
            self.shoot_cooldown = 10
            
    def update_bullets(self):
        for bullet in self.bullets[:]:
            if bullet['active']:
                bullet['y'] -= self.BULLET_SPEED
                if bullet['y'] < 0:
                    self.canvas.delete(bullet['id'])
                    self.bullets.remove(bullet)
                else:
                    self.canvas.coords(bullet['id'], bullet['x'] - 2, bullet['y'] - 5, 
                                     bullet['x'] + 2, bullet['y'] + 5)
                    
    def update_aliens(self):
        if self.game_over:
            return
            
        change_direction = False
        for alien in self.aliens:
            if alien['active']:
                if alien['x'] <= 10 or alien['x'] >= self.WIDTH - 30:
                    change_direction = True
                    break
                    
        if change_direction:
            self.alien_direction *= -1
            for alien in self.aliens:
                if alien['active']:
                    alien['y'] += 20
                    self.canvas.move(alien['id'], 0, 20)
                    
        for alien in self.aliens:
            if alien['active']:
                alien['x'] += self.alien_direction * self.ALIEN_SPEED
                self.canvas.move(alien['id'], self.alien_direction * self.ALIEN_SPEED, 0)
                
    def check_collisions(self):
        for bullet in self.bullets[:]:
            if bullet['active']:
                for alien in self.aliens:
                    if alien['active']:
                        if (abs(bullet['x'] - alien['x'] - 15) < 20 and 
                            abs(bullet['y'] - alien['y'] - 10) < 15):
                            bullet['active'] = False
                            alien['active'] = False
                            self.canvas.delete(bullet['id'])
                            self.canvas.delete(alien['id'])
                            self.score += 10
                            self.canvas.itemconfig(self.score_text, text=f"Score: {self.score}")
                            if bullet in self.bullets:
                                self.bullets.remove(bullet)
                            break
                            
    def check_game_over(self):
        active_aliens = sum(1 for alien in self.aliens if alien['active'])
        if active_aliens == 0:
            self.game_over = True
            self.won = True
            self.show_game_over_message()
        else:
            for alien in self.aliens:
                if alien['active'] and alien['y'] >= self.player_y - 30:
                    self.game_over = True
                    self.won = False
                    self.show_game_over_message()
                    break
                    
    def show_game_over_message(self):
        if self.won:
            message = "YOU WIN!"
            color = 'green'
        else:
            message = "GAME OVER!"
            color = 'red'
            
        self.game_over_text = self.canvas.create_text(self.WIDTH // 2, self.HEIGHT // 2 - 50, 
                                                      text=message, fill=color, 
                                                      font=('Arial', 36, 'bold'))
        self.restart_text = self.canvas.create_text(self.WIDTH // 2, self.HEIGHT // 2, 
                                                     text="Press R to Restart", fill='white', 
                                                     font=('Arial', 18))
        self.final_score_text = self.canvas.create_text(self.WIDTH // 2, self.HEIGHT // 2 + 30, 
                                                        text=f"Final Score: {self.score}", fill='white', 
                                                        font=('Arial', 16))
        
    def restart(self):
        # Clear canvas
        self.canvas.delete("all")
        
        # Reset variables
        self.player_x = self.WIDTH // 2
        self.player_y = self.HEIGHT - 50
        self.bullets = []
        self.aliens = []
        self.score = 0
        self.alien_direction = 1
        self.game_over = False
        self.won = False
        self.shoot_cooldown = 0
        
        # Reinitialize game
        self.create_stars()
        self.create_aliens()
        self.create_player()
        self.create_score_display()
        
    def game_loop(self):
        if not self.game_over:
            if self.shoot_cooldown > 0:
                self.shoot_cooldown -= 1
                
            self.update_bullets()
            self.update_aliens()
            self.check_collisions()
            self.check_game_over()
            
        self.root.after(50, self.game_loop)  # 20 FPS

def main():
    root = tk.Tk()
    root.resizable(False, False)
    game = SpaceInvadersGame(root)
    root.mainloop()

if __name__ == "__main__":
    main()

        
