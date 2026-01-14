"""
Game entities for Geometry Dash clone
"""
from game.entities.obstacle import Obstacle
from game.entities.player import Player

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