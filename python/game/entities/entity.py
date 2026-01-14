"""
Base Entity class for all game objects with sprite support
"""
class Entity:
    """Base class for all entities in the game"""
    def __init__(self, x, y, sprite=None):
        self.x = x
        self.y = y
        self.sprite = sprite if sprite is not None else self.create_default_sprite()
        self.width = len(self.sprite[0]) if self.sprite else 50
        self.height = len(self.sprite) if self.sprite else 50

    def create_default_sprite(self):
        """Create a default 50x50 sprite (white square)"""
        return [[255 for _ in range(50)] for _ in range(50)]

    def update(self):
        raise NotImplementedError("update() of Entity() not implemented")

    def reset(self):
        raise NotImplementedError("reset() of Entity() not implemented")