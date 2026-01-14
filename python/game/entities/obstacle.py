from .entity import Entity

class Obstacle(Entity):
    def __init__(self, x, height=40, width=30):
        super().__init__(x, 350 - height) # Sits on ground
        self.height = height
        self.width = width

    def update(self):
        pass

    def update(self, speed):
        self.x -= speed

    def collides_with(self, player):
        # Simple AABB collision
        return (self.x < player.x + player.SIZE and
                self.x + self.width > player.x and
                self.y < player.y + player.SIZE and
                self.y + self.height > player.y)

    def is_off_screen(self):
        return self.x + self.width < 0

    def reset(self):
        pass
