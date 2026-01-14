"""
Game entities for Geometry Dash clone
"""
from game.entities.player import Player

class Obstacle:
    def __init__(self, x, height=40, width=30):
        self.x = x
        self.height = height
        self.width = width
        self.y = 350 - height  # Sits on ground
    
    def update(self, speed):
        self.x -= speed
    
    def is_off_screen(self):
        return self.x + self.width < 0
    
    def collides_with(self, player):
        # Simple AABB collision
        return (self.x < player.x + player.size and
                self.x + self.width > player.x and
                self.y < player.y + player.size and
                self.y + self.height > player.y)

class GameState:
    def __init__(self):
        self.player = Player()
        self.obstacles = []
        self.scroll_speed = 5
        self.score = 0
        self.game_over = False
        self.ground_y = 350
        self.spawn_timer = 0
        self.spawn_interval = 60  # Frames between obstacles
        
    def reset(self):
        self.player = Player()
        self.obstacles = []
        self.score = 0
        self.game_over = False
        self.spawn_timer = 0
    
    def update(self):
        if self.game_over:
            return
        
        # Update player
        self.player.update(self.ground_y)
        
        # Spawn obstacles
        self.spawn_timer += 1
        if self.spawn_timer >= self.spawn_interval:
            self.spawn_timer = 0
            # Random height between 30 and 60
            import random
            height = random.randint(30, 60)
            self.obstacles.append(Obstacle(640, height))
        
        # Update obstacles
        for obstacle in self.obstacles:
            obstacle.update(self.scroll_speed)
            
            # Check collision
            if obstacle.collides_with(self.player):
                self.game_over = True
            
            # Score when obstacle passes player
            if obstacle.x + obstacle.width < self.player.x and not hasattr(obstacle, 'scored'):
                obstacle.scored = True
                self.score += 1
        
        # Remove off-screen obstacles
        self.obstacles = [obs for obs in self.obstacles if not obs.is_off_screen()]