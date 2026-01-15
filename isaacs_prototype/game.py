import csv


class Player:
    """Player character for Geometry Dash."""
    
    def __init__(self, x, y, size=3):
        self.x = x
        self.y = y
        self.size = size
        self.velocity_y = 0
        self.gravity = 0.8
        self.jump_power = -12
        self.on_ground = False
        self.alive = True
        
    def jump(self):
        """Make the player jump if on ground."""
        if self.on_ground:
            self.velocity_y = self.jump_power
            self.on_ground = False
    
    def update(self):
        """Update player physics."""
        if not self.alive:
            return
            
        # Apply gravity
        self.velocity_y += self.gravity
        self.y += self.velocity_y
        
    def get_bounds(self):
        """Get player bounding box for collision detection."""
        return {
            'left': int(self.x),
            'right': int(self.x + self.size),
            'top': int(self.y),
            'bottom': int(self.y + self.size)
        }


class GameState:
    """Main game class for Geometry Dash."""
    
    def __init__(self, level_file='world.csv'):
        # Load level from CSV
        self.level = self.load_level(level_file)
        self.level_height = len(self.level)
        self.level_width = len(self.level[0]) if self.level_height > 0 else 0
        
        # Display settings (640x400 Atari ST resolution)
        self.display_width = 640
        self.display_height = 400
        
        # Game objects
        self.player = Player(x=20, y=10, size=8)
        self.camera_x = 0
        self.scroll_speed = 2
        
        # Game state
        self.game_over = False
        self.win = False
        self.score = 0
        
    def load_level(self, filename):
        """Load level data from CSV file."""
        with open(filename, 'r') as f:
            reader = csv.reader(f)
            data = []
            for row in reader:
                data.append([int(val) for val in row])
        return data
    
    def check_collision(self, pixel_value):
        """Check if a pixel value is solid."""
        # 255 = ground/platforms, 200 = spikes, 0 = air
        return pixel_value >= 200
    
    def update_player_collision(self):
        """Handle player collision with level."""
        bounds = self.player.get_bounds()
        
        # Check if player is out of bounds
        if self.player.y < 0 or self.player.y >= self.level_height:
            self.player.alive = False
            self.game_over = True
            return
        
        # Check ground collision (below player)
        self.player.on_ground = False
        if self.player.velocity_y >= 0:  # Moving down
            for x in range(bounds['left'], min(bounds['right'], self.level_width)):
                if bounds['bottom'] < self.level_height:
                    pixel = self.level[bounds['bottom'], x]
                    if pixel == 255:  # Ground
                        self.player.y = bounds['bottom'] - self.player.size
                        self.player.velocity_y = 0
                        self.player.on_ground = True
                        break
                    elif pixel == 200:  # Spike
                        self.player.alive = False
                        self.game_over = True
                        return
        
        # Check ceiling collision (above player)
        if self.player.velocity_y < 0:  # Moving up
            for x in range(bounds['left'], min(bounds['right'], self.level_width)):
                if bounds['top'] >= 0:
                    pixel = self.level[bounds['top'], x]
                    if self.check_collision(pixel):
                        self.player.y = bounds['top'] + 1
                        self.player.velocity_y = 0
                        if pixel == 200:  # Hit spike
                            self.player.alive = False
                            self.game_over = True
                            return
        
        # Check side collisions
        for y in range(bounds['top'], min(bounds['bottom'], self.level_height)):
            if bounds['right'] < self.level_width:
                pixel = self.level[y, bounds['right']]
                if pixel == 200:  # Spike
                    self.player.alive = False
                    self.game_over = True
                    return
                elif pixel == 255:  # Wall
                    self.player.x = bounds['right'] - self.player.size - 1
    
    def update(self):
        """Update game state."""
        if self.game_over or self.win:
            return
        
        # Update player
        self.player.update()
        self.update_player_collision()
        
        # Auto-scroll camera
        self.camera_x += self.scroll_speed
        self.score = int(self.camera_x / 10)
        
        # Check if player fell behind
        if self.player.x < self.camera_x:
            self.player.alive = False
            self.game_over = True
        
        # Check win condition
        if self.camera_x + self.display_width >= self.level_width:
            self.win = True
    

    
    def reset(self):
        """Reset game to initial state."""
        self.player = Player(x=20, y=10, size=8)
        self.camera_x = 0
        self.game_over = False
        self.win = False
        self.score = 0


def draw_game(buffer, game_state):
    """Render game to a 640x400 framebuffer."""
    # Clear screen
    for y in range(400):
        for x in range(640):
            buffer[y][x] = 0
    
    # Calculate visible portion of level
    start_x = int(game_state.camera_x)
    
    # Draw level to buffer
    for y in range(min(game_state.level_height, 400)):
        for x in range(640):
            world_x = start_x + x
            if 0 <= world_x < game_state.level_width:
                buffer[y][x] = game_state.level[y][world_x]
    
    # Draw player
    if game_state.player.alive:
        player_screen_x = int(game_state.player.x - game_state.camera_x)
        bounds = game_state.player.get_bounds()
        
        for dy in range(game_state.player.size):
            for dx in range(game_state.player.size):
                px = player_screen_x + dx
                py = int(game_state.player.y) + dy
                if 0 <= px < 640 and 0 <= py < 400:
                    buffer[py][px] = 100  # Dark grey for player cube
    
    # Draw score
    draw_text_simple(buffer, game_state.score, 10, 10, size=2)
    
    # Draw game over message
    if game_state.game_over:
        # Draw semi-transparent overlay
        for y in range(150, 250):
            for x in range(200, 440):
                buffer[y][x] = min(buffer[y][x] + 50, 150)
        
        # Draw "GAME OVER" text
        draw_text_simple(buffer, "GAME", 250, 170, size=4)
        draw_text_simple(buffer, "OVER", 250, 200, size=4)
    
    if game_state.win:
        # Draw win message
        for y in range(150, 250):
            for x in range(200, 440):
                buffer[y][x] = min(buffer[y][x] + 50, 200)
        
        draw_text_simple(buffer, "WIN", 280, 180, size=5)


def draw_text_simple(buffer, text, x, y, size=2):
    """Simple text rendering using block patterns."""
    digits = {
        '0': [[1,1,1],[1,0,1],[1,0,1],[1,0,1],[1,1,1]],
        '1': [[0,1,0],[1,1,0],[0,1,0],[0,1,0],[1,1,1]],
        '2': [[1,1,1],[0,0,1],[1,1,1],[1,0,0],[1,1,1]],
        '3': [[1,1,1],[0,0,1],[1,1,1],[0,0,1],[1,1,1]],
        '4': [[1,0,1],[1,0,1],[1,1,1],[0,0,1],[0,0,1]],
        '5': [[1,1,1],[1,0,0],[1,1,1],[0,0,1],[1,1,1]],
        '6': [[1,1,1],[1,0,0],[1,1,1],[1,0,1],[1,1,1]],
        '7': [[1,1,1],[0,0,1],[0,0,1],[0,0,1],[0,0,1]],
        '8': [[1,1,1],[1,0,1],[1,1,1],[1,0,1],[1,1,1]],
        '9': [[1,1,1],[1,0,1],[1,1,1],[0,0,1],[1,1,1]],
        'G': [[1,1,1],[1,0,0],[1,0,1],[1,0,1],[1,1,1]],
        'A': [[0,1,0],[1,0,1],[1,1,1],[1,0,1],[1,0,1]],
        'M': [[1,0,1],[1,1,1],[1,1,1],[1,0,1],[1,0,1]],
        'E': [[1,1,1],[1,0,0],[1,1,1],[1,0,0],[1,1,1]],
        'O': [[1,1,1],[1,0,1],[1,0,1],[1,0,1],[1,1,1]],
        'V': [[1,0,1],[1,0,1],[1,0,1],[1,0,1],[0,1,0]],
        'R': [[1,1,0],[1,0,1],[1,1,0],[1,0,1],[1,0,1]],
        'W': [[1,0,1],[1,0,1],[1,1,1],[1,1,1],[1,0,1]],
        'I': [[1,1,1],[0,1,0],[0,1,0],[0,1,0],[1,1,1]],
        'N': [[1,0,1],[1,1,1],[1,1,1],[1,0,1],[1,0,1]],
    }
    
    offset_x = 0
    for char in str(text).upper():
        if char == ' ':
            offset_x += 4 * size
            continue
        if char in digits:
            pattern = digits[char]
            for row in range(5):
                for col in range(3):
                    if pattern[row][col]:
                        for sy in range(size):
                            for sx in range(size):
                                px = x + offset_x + col * size + sx
                                py = y + row * size + sy
                                if 0 <= px < 640 and 0 <= py < 400:
                                    buffer[py][px] = 255
            offset_x += 4 * size
