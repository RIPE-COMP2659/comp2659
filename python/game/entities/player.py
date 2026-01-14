from .entity import Entity

class Player(Entity):
    SIZE = 50  # Updated from 20 to 50
    GRAVITY = 0.8
    JUMP_STRENGTH = -15

    def __init__(self, x=100, y=300):
        # Create player sprite (50x50 white square with a simple face/design)
        sprite = self.create_player_sprite()
        super().__init__(x, y, sprite)
        self.dy = 0
        self.is_jumping = False
        self.on_ground = False

    @staticmethod
    def create_player_sprite():
        """Create a 50x50 player sprite"""
        sprite = [[255 for _ in range(50)] for _ in range(50)]
        
        # Add a simple design - darker border
        for i in range(50):
            sprite[0][i] = 200  # Top border
            sprite[49][i] = 200  # Bottom border
            sprite[i][0] = 200  # Left border
            sprite[i][49] = 200  # Right border
        
        # Add simple "eyes" (two dark squares)
        for y in range(15, 20):
            for x in range(15, 20):
                sprite[y][x] = 100
            for x in range(30, 35):
                sprite[y][x] = 100
        
        # Add a "mouth" (dark line)
        for x in range(15, 35):
            sprite[35][x] = 100
        
        return sprite

    def jump(self):
        if self.on_ground:
            self.dy = self.JUMP_STRENGTH
            self.is_jumping = True
            self.on_ground = False

    def update(self, ground_y=350):
        # Apply gravity
        self.dy += self.GRAVITY
        self.y += self.dy

        # Check ground collision
        if self.y + self.SIZE >= ground_y:
            self.y = ground_y - self.SIZE
            self.dy = 0
            self.on_ground = True
            self.is_jumping = False
        else:
            self.on_ground = False

    def reset(self, x=100, y=300):
        self.x = x
        self.y = y
        self.dy = 0
        self.is_jumping = False
        self.on_ground = False
