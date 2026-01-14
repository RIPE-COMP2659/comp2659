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

    def collides_with(self, player):
        # Simple AABB collision
        return (self.x < player.x + player.SIZE and
                self.x + self.SIZE > player.x and
                self.y < player.y + player.SIZE and
                self.y + self.SIZE > player.y)

    def is_off_screen(self):
        return self.x + self.SIZE < 0

    def reset(self):
        pass