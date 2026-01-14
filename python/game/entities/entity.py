"""
Base Entity class for all game objects
"""
class Entity():
    """Base class for all entities in the game"""
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def update(self):
        raise NotImplementedError("update() of Entity() not implemented")

    def reset(self):
        raise NotImplementedError("reset() of Entity() not implemented")