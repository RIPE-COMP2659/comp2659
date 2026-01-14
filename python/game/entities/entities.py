"""
Game entities for Geometry Dash clone
"""
from game.entities.box import Box
from game.entities.player import Player
from game.entities.entity import Entity

class GameState(Entity):
    def __init__(self):
        super().__init__(0, 0)  # GameState doesn't need position
        self.player = Player()
        self.boxes = []  # Changed from obstacles to boxes
        self.scroll_speed = 10
        self.score = 0
        self.game_over = False
        self.ground_y = 350
        self.spawn_timer = 0
        self.spawn_interval = 60  # Frames between boxes
        
    def reset(self):
        self.player = Player()
        self.boxes = []
        self.score = 0
        self.game_over = False
        self.spawn_timer = 0
    
    def update(self):
        if self.game_over:
            return
        
        # Update player
        self.player.update(self.ground_y)
        
        # Spawn boxes
        self.spawn_timer += 1
        if self.spawn_timer >= self.spawn_interval:
            self.spawn_timer = 0
            # Fixed size box at ground level
            self.boxes.append(Box(640, self.ground_y - Box.SIZE))
        
        # Update boxes
        for box in self.boxes:
            box.update(self.scroll_speed)
            
            # Check collision
            if box.collides_with(self.player):
                self.game_over = True
            
            # Score when box passes player
            if box.x + Box.SIZE < self.player.x and not hasattr(box, 'scored'):
                box.scored = True
                self.score += 1
        
        # Remove off-screen boxes
        self.boxes = [box for box in self.boxes if not box.is_off_screen()]