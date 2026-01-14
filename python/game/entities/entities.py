"""
Game entities for Geometry Dash clone
"""
from game.entities.box import Box
from game.entities.halfbox import HalfBox
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
        
        # Check if player is landing on any box or halfbox
        platform_y = None
        landed_on_box = False
        
        for box in self.boxes:
            # Check if player is landing on top of this box/halfbox
            if box.is_player_landing_on_top(self.player):
                platform_y = box.y  # Player can land on this box
                landed_on_box = True
                break
            
            # Check if player collides with sides of box/halfbox (death)
            if box.collides_with_sides(self.player):
                self.game_over = True
                return
        
        # Update player with platform info
        self.player.update(self.ground_y, platform_y)
        
        # Spawn boxes (can spawn both Box and HalfBox)
        self.spawn_timer += 1
        if self.spawn_timer >= self.spawn_interval:
            self.spawn_timer = 0
            # Randomly choose between Box and HalfBox
            import random
            if random.random() < 0.5:
                # Spawn regular Box
                self.boxes.append(Box(640, self.ground_y - Box.SIZE))
            else:
                # Spawn HalfBox
                self.boxes.append(HalfBox(640, self.ground_y - HalfBox.HEIGHT))
        
        # Update boxes
        for box in self.boxes:
            box.update(self.scroll_speed)
            
            # Score when box passes player
            player_width = self.player.SIZE
            box_width = getattr(box, 'width', Box.SIZE)
            if box.x + box_width < self.player.x and not hasattr(box, 'scored'):
                box.scored = True
                self.score += 1
        
        # Remove off-screen boxes
        self.boxes = [box for box in self.boxes if not box.is_off_screen()]