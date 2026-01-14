from .entity import Entity

class Player(Entity):
    SIZE = 20
    GRAVITY = 0.8
    JUMP_STRENGTH = -15

    def __init__(self, x=100, y=300):
        super().__init__(x, y)
        self.dy = 0
        self.is_jumping = False
        self.on_ground = False

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