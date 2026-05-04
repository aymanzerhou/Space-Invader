import pygame
import sys
import random
import math

# Initialize Pygame
pygame.init()

# Constants
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
YELLOW = (255, 255, 0)
STAR_COLOR = (255, 255, 255)

class Player:
    def __init__(self):
        self.width = 40
        self.height = 30
        self.x = SCREEN_WIDTH // 2 - self.width // 2
        self.y = SCREEN_HEIGHT - 60
        self.speed = 5
        self.color = GREEN
        
    def move_left(self):
        if self.x > 0:
            self.x -= self.speed
            
    def move_right(self):
        if self.x < SCREEN_WIDTH - self.width:
            self.x += self.speed
            
    def draw(self, screen):
        # Draw spaceship shape
        points = [
            (self.x + self.width // 2, self.y),
            (self.x + self.width, self.y + self.height),
            (self.x + self.width - 10, self.y + self.height),
            (self.x + self.width // 2, self.y + 15),
            (self.x + 10, self.y + self.height),
            (self.x, self.y + self.height)
        ]
        pygame.draw.polygon(screen, self.color, points)
        
    def get_rect(self):
        return pygame.Rect(self.x, self.y, self.width, self.height)

class Bullet:
    def __init__(self, x, y):
        self.width = 4
        self.height = 10
        self.x = x + 18  # Center of player
        self.y = y
        self.speed = 7
        self.color = YELLOW
        self.active = True
        
    def update(self):
        self.y -= self.speed
        if self.y < 0:
            self.active = False
            
    def draw(self, screen):
        if self.active:
            pygame.draw.rect(screen, self.color, (self.x, self.y, self.width, self.height))
            
    def get_rect(self):
        return pygame.Rect(self.x, self.y, self.width, self.height)

class Alien:
    def __init__(self, x, y):
        self.width = 30
        self.height = 20
        self.x = x
        self.y = y
        self.color = WHITE
        self.active = True
        
    def move(self, dx, dy):
        if self.active:
            self.x += dx
            self.y += dy
            
    def draw(self, screen):
        if self.active:
            # Draw alien shape
            points = [
                (self.x + self.width // 2, self.y),
                (self.x + self.width - 5, self.y + 5),
                (self.x + self.width - 5, self.y + 10),
                (self.x + self.width - 10, self.y + 15),
                (self.x + self.width - 5, self.y + 20),
                (self.x + self.width - 10, self.y + 20),
                (self.x + self.width // 2, self.y + 15),
                (self.x + 10, self.y + 20),
                (self.x + 5, self.y + 20),
                (self.x + 10, self.y + 15),
                (self.x + 5, self.y + 10),
                (self.x + 5, self.y + 5)
            ]
            pygame.draw.polygon(screen, self.color, points)
            
    def get_rect(self):
        return pygame.Rect(self.x, self.y, self.width, self.height)
        
    def destroy(self):
        self.active = False

class Star:
    def __init__(self):
        self.x = random.randint(0, SCREEN_WIDTH)
        self.y = random.randint(0, SCREEN_HEIGHT)
        self.brightness = random.randint(100, 255)
        self.size = random.choice([1, 2])
        
    def draw(self, screen):
        color = (self.brightness, self.brightness, self.brightness)
        pygame.draw.circle(screen, color, (self.x, self.y), self.size)

class Game:
    def __init__(self):
        self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
        pygame.display.set_caption("Space Invaders")
        self.clock = pygame.time.Clock()
        self.font = pygame.font.Font(None, 36)
        self.small_font = pygame.font.Font(None, 24)
        self.reset_game()
        
    def reset_game(self):
        self.player = Player()
        self.bullets = []
        self.aliens = []
        self.stars = [Star() for _ in range(100)]
        self.score = 0
        self.alien_direction = 1
        self.alien_speed = 0.5
        self.game_over = False
        self.won = False
        self.shoot_cooldown = 0
        
        # Create alien grid
        for row in range(5):
            for col in range(11):
                alien = Alien(100 + col * 50, 50 + row * 40)
                self.aliens.append(alien)
                
    def handle_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_r and self.game_over:
                    self.reset_game()
                elif event.key == pygame.K_ESCAPE:
                    return False
                    
        # Handle continuous key presses
        if not self.game_over:
            keys = pygame.key.get_pressed()
            if keys[pygame.K_LEFT]:
                self.player.move_left()
            if keys[pygame.K_RIGHT]:
                self.player.move_right()
            if keys[pygame.K_SPACE] and self.shoot_cooldown <= 0:
                self.bullets.append(Bullet(self.player.x, self.player.y))
                self.shoot_cooldown = 10
                
        return True
        
    def update(self):
        if self.game_over:
            return
            
        # Update shoot cooldown
        if self.shoot_cooldown > 0:
            self.shoot_cooldown -= 1
            
        # Update bullets
        for bullet in self.bullets[:]:
            bullet.update()
            if not bullet.active:
                self.bullets.remove(bullet)
                
        # Move aliens
        change_direction = False
        for alien in self.aliens:
            if alien.active:
                if alien.x <= 0 or alien.x >= SCREEN_WIDTH - alien.width:
                    change_direction = True
                    break
                    
        if change_direction:
            self.alien_direction *= -1
            for alien in self.aliens:
                alien.move(0, 20)
                
        for alien in self.aliens:
            alien.move(self.alien_direction * self.alien_speed, 0)
            
        # Check collisions
        for bullet in self.bullets:
            if bullet.active:
                for alien in self.aliens:
                    if alien.active and bullet.get_rect().colliderect(alien.get_rect()):
                        bullet.active = False
                        alien.destroy()
                        self.score += 10
                        
        # Check win condition
        active_aliens = sum(1 for alien in self.aliens if alien.active)
        if active_aliens == 0:
            self.game_over = True
            self.won = True
            
        # Check lose condition
        for alien in self.aliens:
            if alien.active and alien.y >= self.player.y - 20:
                self.game_over = True
                self.won = False
                break
                
    def draw(self):
        self.screen.fill(BLACK)
        
        # Draw stars
        for star in self.stars:
            star.draw(self.screen)
            
        # Draw game objects
        self.player.draw(self.screen)
        
        for bullet in self.bullets:
            bullet.draw(self.screen)
            
        for alien in self.aliens:
            alien.draw(self.screen)
            
        # Draw score
        score_text = self.small_font.render(f"Score: {self.score}", True, WHITE)
        self.screen.blit(score_text, (10, 10))
        
        # Draw instructions
        instructions = self.small_font.render("Arrow Keys: Move | Space: Shoot | R: Restart | ESC: Exit", True, WHITE)
        self.screen.blit(instructions, (SCREEN_WIDTH - 500, 10))
        
        # Draw game over message
        if self.game_over:
            if self.won:
                text = self.font.render("YOU WIN!", True, GREEN)
            else:
                text = self.font.render("GAME OVER!", True, RED)
            text_rect = text.get_rect(center=(SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2 - 50))
            self.screen.blit(text, text_rect)
            
            restart_text = self.small_font.render("Press R to Restart", True, WHITE)
            restart_rect = restart_text.get_rect(center=(SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2))
            self.screen.blit(restart_text, restart_rect)
            
            final_score_text = self.small_font.render(f"Final Score: {self.score}", True, WHITE)
            final_score_rect = final_score_text.get_rect(center=(SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2 + 30))
            self.screen.blit(final_score_text, final_score_rect)
            
        pygame.display.flip()
        
    def run(self):
        running = True
        while running:
            running = self.handle_events()
            self.update()
            self.draw()
            self.clock.tick(60)  # 60 FPS
            
        pygame.quit()
        sys.exit()

def main():
    print("=== SPACE INVADERS ===")
    print("Controls:")
    print("Arrow Keys: Move left/right")
    print("Space: Shoot")
    print("R: Restart (when game over)")
    print("ESC: Exit game")
    print("Starting game...")
    
    game = Game()
    game.run()

if __name__ == "__main__":
    main()
