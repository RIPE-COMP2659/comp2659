"""
Game entities for Geometry Dash clone
"""
from game.entities.box import Box
from game.entities.halfbox import HalfBox
from game.entities.spike import Spike
from game.entities.player import Player
from game.entities.entity import Entity

class GameState(Entity):
    def __init__(self):
        super().__init__(0, 0)  # GameState doesn't need position
        self.player = Player()
        self.boxes = []  # Changed from obstacles to boxes (includes all obstacle types)
        self.scroll_speed = 10
        self.score = 0
        self.game_over = False
        self.ground_y = 350
        self.spawn_timer = 0
        self.spawn_interval = 60  # Frames between obstacles
        
    def reset(self):
        self.player = Player()
        self.boxes = []
        self.score = 0
        self.game_over = False
        self.spawn_timer = 0
    
    def update(self):
        if self.game_over:
            return
        
        # First, check if player should land on a platform
        platform_y = None
        
        for box in self.boxes:
            # Check if player is landing on top of this box/halfbox
            # Spikes will always return False for is_player_landing_on_top
            if box.is_player_landing_on_top(self.player):
                platform_y = box.y  # Player can land on this box
                break
        
        # Update player with platform info (this moves the player)
        self.player.update(self.ground_y, platform_y)
        
        # After player has moved, check for ANY collision that isn't a landing
        for box in self.boxes:
            # Simple AABB collision check
            if (self.player.x < box.x + getattr(box, 'width', Box.SIZE) and
                self.player.x + self.player.SIZE > box.x and
                self.player.y < box.y + getattr(box, 'height', Box.SIZE) and
                self.player.y + self.player.SIZE > box.y):
                
                # There's overlap - check if it's a valid landing
                if not box.is_player_landing_on_top(self.player):
                    # Not a valid landing = death
                    # (Spikes will ALWAYS trigger this since they return False)
                    self.game_over = True
                    return
        
        # Spawn obstacles (can spawn Box, HalfBox, or Spike)
        self.spawn_timer += 1
        if self.spawn_timer >= self.spawn_interval:
            self.spawn_timer = 0
            # Randomly choose between Box, HalfBox, and Spike
            import random
            rand = random.random()
            
            if rand < 0.33:
                # Spawn regular Box
                self.boxes.append(Box(640, self.ground_y - Box.SIZE))
            elif rand < 0.66:
                # Spawn HalfBox
                self.boxes.append(HalfBox(640, self.ground_y - HalfBox.HEIGHT))
            else:
                # Spawn Spike
                self.boxes.append(Spike(640, self.ground_y - Spike.SIZE))
        
        # Update all obstacles
        for box in self.boxes:
            box.update(self.scroll_speed)
            
            # Score when obstacle passes player
            player_width = self.player.SIZE
            box_width = getattr(box, 'width', Box.SIZE)
            if box.x + box_width < self.player.x and not hasattr(box, 'scored'):
                box.scored = True
                self.score += 1
        
        # Remove off-screen obstacles
        self.boxes = [box for box in self.boxes if not box.is_off_screen()]