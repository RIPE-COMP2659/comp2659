from .entity import Entity

class Spike(Entity):
    SIZE = 50  # Fixed size for spikes

    def __init__(self, x, y=300):
        # Create spike sprite (50x50 with triangular spikes)
        sprite = self.create_spike_sprite()
        super().__init__(x, y, sprite)

    @staticmethod
    def create_spike_sprite():
        """Create a 50x50 spike sprite with triangular spikes pointing upward"""
        sprite = [[0 for _ in range(50)] for _ in range(50)]
        
        # Create multiple triangular spikes along the bottom
        # We'll make 5 spikes, each 10 pixels wide
        for spike_num in range(5):
            base_x = spike_num * 10
            
            # Draw triangle spike (pointing upward)
            for y in range(50):
                # Calculate width of triangle at this height
                # Triangle gets narrower as we go down from top
                progress = y / 50.0  # 0.0 at top, 1.0 at bottom
                half_width = int(progress * 5)  # Max 5 pixels on each side at bottom
                
                center_x = base_x + 5  # Center of this spike
                
                for dx in range(-half_width, half_width + 1):
                    x = center_x + dx
                    if 0 <= x < 50:
                        sprite[y][x] = 200  # Light gray for the spike body
        
        # Add darker outline for definition
        for spike_num in range(5):
            base_x = spike_num * 10
            center_x = base_x + 5
            
            # Draw left edge of spike
            for y in range(50):
                progress = y / 50.0
                half_width = int(progress * 5)
                left_x = center_x - half_width
                if 0 <= left_x < 50:
                    sprite[y][left_x] = 100  # Dark gray outline
            
            # Draw right edge of spike
            for y in range(50):
                progress = y / 50.0
                half_width = int(progress * 5)
                right_x = center_x + half_width
                if 0 <= right_x < 50:
                    sprite[y][right_x] = 100  # Dark gray outline
        
        # Draw base line at bottom
        for x in range(50):
            sprite[49][x] = 100
        
        return sprite

    def update(self, speed):
        """Move spike left with scroll speed"""
        self.x -= speed

    def is_player_landing_on_top(self, player):
        """
        Spikes CANNOT be landed on - always return False
        Any contact with a spike is deadly
        """
        return False

    def collides_with_sides(self, player):
        """
        Check if player collides with the spike
        ANY collision with a spike is deadly
        """
        # Standard AABB collision
        overlap = (self.x < player.x + player.SIZE and
                  self.x + self.SIZE > player.x and
                  self.y < player.y + player.SIZE and
                  self.y + self.SIZE > player.y)
        
        return overlap

    def is_off_screen(self):
        """Check if spike is off screen"""
        return self.x + self.SIZE < 0

    def reset(self):
        """Reset spike state (currently nothing to reset)"""
        pass