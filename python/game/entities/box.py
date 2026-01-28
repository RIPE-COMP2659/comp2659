from .entity import Entity

class Box(Entity):
    SIZE = 50  # Fixed size for boxes

    def __init__(self, x, y=300):
        # Create box sprite (50x50 gray box)
        sprite = self.create_box_sprite()
        super().__init__(x, y, sprite)

    @staticmethod
    def create_box_sprite():
        """Create a 50x50 box sprite"""
        sprite = [[180 for _ in range(50)] for _ in range(50)]
        
        # Add darker border for definition
        for i in range(50):
            sprite[0][i] = 120  # Top border
            sprite[49][i] = 120  # Bottom border
            sprite[i][0] = 120  # Left border
            sprite[i][49] = 120  # Right border
        
        # Add some diagonal lines for texture
        for i in range(0, 50, 10):
            for j in range(50):
                if i + j < 50:
                    sprite[i + j][j] = 150
        
        return sprite

    def update(self, speed):
        self.x -= speed

    def is_player_landing_on_top(self, player):
        """
        Check if player is landing on top of this box
        Returns True if player's bottom edge is at or near the box's top edge
        and player is falling (positive dy)
        """
        # Check horizontal overlap
        horizontal_overlap = (self.x < player.x + player.SIZE and
                             self.x + self.SIZE > player.x)
        
        if not horizontal_overlap:
            return False
        
        # Check if player's bottom is at or just above box's top
        player_bottom = player.y + player.SIZE
        box_top = self.y
        
        # Allow some tolerance for landing detection
        # Player must be falling (dy >= 0) and within 40 pixels of the top
        is_above = player_bottom >= box_top - 40 and player_bottom <= box_top + 40
        is_falling = player.dy >= 0
        
        return is_above and is_falling

    def collides_with_sides(self, player):
        """
        Check if player collides with the sides or bottom of the box
        This is the "death" collision
        """
        # Standard AABB collision
        overlap = (self.x < player.x + player.SIZE and
                  self.x + self.SIZE > player.x and
                  self.y < player.y + player.SIZE and
                  self.y + self.SIZE > player.y)
        
        if not overlap:
            return False
        
        # If overlapping, check if it's NOT a top landing
        # (i.e., player is hitting from the side or bottom)
        player_bottom = player.y + player.SIZE
        box_top = self.y
        
        # If player's bottom is significantly inside the box, it's a side collision
        return player_bottom > box_top + 40 or player.dy < 0

    def is_off_screen(self):
        return self.x + self.SIZE < 0

    def reset(self):
        pass