import csv


class Platform:
    """A platform object in the game."""
    
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        
    def get_bounds(self):
        """Get platform bounding box."""
        return {
            'left': self.x,
            'right': self.x + self.width,
            'top': self.y,
            'bottom': self.y + self.height
        }


class Obstacle:
    """An obstacle (spike) object in the game."""
    
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        
    def get_bounds(self):
        """Get obstacle bounding box."""
        return {
            'left': self.x,
            'right': self.x + self.width,
            'top': self.y,
            'bottom': self.y + self.height
        }


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
        # Display settings (640x400 Atari ST resolution)
        self.display_width = 640
        self.display_height = 400
        self.chunk_width = self.display_width + 1  # One extra column on the right
        
        # Load level from CSV
        self.level = self.load_level(level_file)
        self.level_height = len(self.level)
        self.level_width = len(self.level[0]) if self.level_height > 0 else 0
        
        # Parse platforms and obstacles from level
        self.platforms = []
        self.obstacles = []
        self.parse_level_objects()
        
        # Game objects
        self.player = Player(x=20, y=10, size=8)
        self.camera_x = 0
        self.scroll_speed = 2
        self.current_chunk_start = 0  # Track current chunk position
        
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
    
    def parse_level_objects(self):
        """Parse level data to extract platforms and obstacles as objects."""
        # Track which pixels we've already processed
        processed = [[False for _ in range(self.level_width)] for _ in range(self.level_height)]
        
        for y in range(self.level_height):
            for x in range(self.level_width):
                if processed[y][x]:
                    continue
                    
                pixel_value = self.level[y][x]
                
                # Found a platform block
                if pixel_value == 255:
                    # Find the extent of this platform
                    width = 1
                    height = 1
                    
                    # Expand horizontally
                    while x + width < self.level_width and self.level[y][x + width] == 255:
                        width += 1
                    
                    # Expand vertically
                    valid_height = True
                    while y + height < self.level_height and valid_height:
                        for dx in range(width):
                            if self.level[y + height][x + dx] != 255:
                                valid_height = False
                                break
                        if valid_height:
                            height += 1
                    
                    # Mark all pixels as processed
                    for dy in range(height):
                        for dx in range(width):
                            processed[y + dy][x + dx] = True
                    
                    self.platforms.append(Platform(x, y, width, height))
                
                # Found an obstacle (spike)
                elif pixel_value == 200:
                    # Find the extent of this obstacle
                    width = 1
                    height = 1
                    
                    # Expand horizontally
                    while x + width < self.level_width and self.level[y][x + width] == 200:
                        width += 1
                    
                    # Expand vertically
                    valid_height = True
                    while y + height < self.level_height and valid_height:
                        for dx in range(width):
                            if self.level[y + height][x + dx] != 200:
                                valid_height = False
                                break
                        if valid_height:
                            height += 1
                    
                    # Mark all pixels as processed
                    for dy in range(height):
                        for dx in range(width):
                            processed[y + dy][x + dx] = True
                    
                    self.obstacles.append(Obstacle(x, y, width, height))
    
    def check_box_collision(self, box1, box2):
        """Check if two bounding boxes collide."""
        return not (box1['right'] <= box2['left'] or 
                    box1['left'] >= box2['right'] or 
                    box1['bottom'] <= box2['top'] or 
                    box1['top'] >= box2['bottom'])
    
    def update_player_collision(self):
        """Handle player collision with platforms and obstacles."""
        bounds = self.player.get_bounds()
        
        # Check if player is out of bounds
        if self.player.y < 0 or self.player.y >= self.level_height:
            self.player.alive = False
            self.game_over = True
            return
        
        # Check ground collision (below player)
        self.player.on_ground = False
        if self.player.velocity_y >= 0:  # Moving down
            for platform in self.platforms:
                # Only check platforms in visible range
                if platform.x + platform.width < self.camera_x - 100:
                    continue
                if platform.x > self.camera_x + self.display_width + 100:
                    continue
                    
                platform_bounds = platform.get_bounds()
                
                # Check if player is landing on top of platform
                if (bounds['bottom'] >= platform_bounds['top'] and 
                    bounds['bottom'] <= platform_bounds['top'] + 3 and
                    bounds['right'] > platform_bounds['left'] and 
                    bounds['left'] < platform_bounds['right']):
                    self.player.y = platform_bounds['top'] - self.player.size
                    self.player.velocity_y = 0
                    self.player.on_ground = True
                    break
        
        # Check obstacle collisions
        for obstacle in self.obstacles:
            # Only check obstacles in visible range
            if obstacle.x + obstacle.width < self.camera_x - 100:
                continue
            if obstacle.x > self.camera_x + self.display_width + 100:
                continue
                
            obstacle_bounds = obstacle.get_bounds()
            if self.check_box_collision(bounds, obstacle_bounds):
                self.player.alive = False
                self.game_over = True
                return
        
        # Check platform collisions (ceiling and walls)
        for platform in self.platforms:
            # Only check platforms in visible range
            if platform.x + platform.width < self.camera_x - 100:
                continue
            if platform.x > self.camera_x + self.display_width + 100:
                continue
                
            platform_bounds = platform.get_bounds()
            
            # Check ceiling collision
            if self.player.velocity_y < 0:
                if self.check_box_collision(bounds, platform_bounds):
                    self.player.y = platform_bounds['bottom']
                    self.player.velocity_y = 0
    
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
        
        # Update current chunk when we scroll past the current frame
        if self.camera_x >= self.current_chunk_start + self.display_width:
            self.current_chunk_start += self.display_width
        
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
        self.current_chunk_start = 0
        self.game_over = False
        self.win = False
        self.score = 0


def draw_game(buffer, game_state):
    """Render game to a 640x400 framebuffer."""
    # Clear screen
    for y in range(400):
        for x in range(640):
            buffer[y][x] = 0
    
    # Calculate visible portion of level (current chunk)
    start_x = int(game_state.camera_x)
    end_x = start_x + game_state.chunk_width
    
    # Draw platforms in current chunk
    for platform in game_state.platforms:
        # Skip platforms outside the visible chunk
        if platform.x + platform.width < start_x:
            continue
        if platform.x >= end_x:
            continue
        
        # Draw platform
        for dy in range(platform.height):
            for dx in range(platform.width):
                world_x = platform.x + dx
                world_y = platform.y + dy
                screen_x = world_x - start_x
                
                if 0 <= screen_x < 640 and 0 <= world_y < 400:
                    buffer[world_y][screen_x] = 255
    
    # Draw obstacles in current chunk
    for obstacle in game_state.obstacles:
        # Skip obstacles outside the visible chunk
        if obstacle.x + obstacle.width < start_x:
            continue
        if obstacle.x >= end_x:
            continue
        
        # Draw obstacle
        for dy in range(obstacle.height):
            for dx in range(obstacle.width):
                world_x = obstacle.x + dx
                world_y = obstacle.y + dy
                screen_x = world_x - start_x
                
                if 0 <= screen_x < 640 and 0 <= world_y < 400:
                    buffer[world_y][screen_x] = 200
    
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
