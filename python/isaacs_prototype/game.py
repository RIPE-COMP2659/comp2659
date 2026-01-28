import csv


class Player:
    """Player character for Geometry Dash."""
    
    def __init__(self, x, y, size=8):
        self.x = float(x)
        self.y = float(y)
        self.size = size
        self.velocity_y = 0.0
        self.gravity = 0.5
        self.jump_power = -10.0
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
        
        # Block scaling - each CSV cell is a 20x20 pixel block
        self.block_size = 20
        
        # Display settings (640x400 Atari ST resolution)
        self.display_width = 640
        self.display_height = 400
        
        # Calculate how many blocks fit on screen
        self.blocks_visible_x = self.display_width // self.block_size  # 32 blocks
        
        # Game objects - player position is in pixels
        # Start player at a safe position (2 blocks above ground)
        player_y = (self.level_height - 5) * self.block_size  # Start above ground
        self.player = Player(x=100, y=player_y, size=16)  # 16x16 pixel player
        
        # Camera position in blocks (will be converted to pixels for rendering)
        self.camera_block_x = 0
        self.scroll_speed = 0.1  # blocks per frame
        
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
    
    def get_block(self, block_x, block_y):
        """Get a block value from the level (in block coordinates)."""
        if 0 <= block_y < self.level_height and 0 <= block_x < self.level_width:
            return self.level[block_y][block_x]
        return 0  # Out of bounds is air
    
    def pixel_to_block(self, pixel_x, pixel_y):
        """Convert pixel coordinates to block coordinates."""
        return int(pixel_x // self.block_size), int(pixel_y // self.block_size)
    
    def is_solid_at_pixel(self, pixel_x, pixel_y):
        """Check if a pixel position is solid."""
        block_x, block_y = self.pixel_to_block(pixel_x, pixel_y)
        block_val = self.get_block(block_x, block_y)
        return block_val >= 200
    
    def is_deadly_at_pixel(self, pixel_x, pixel_y):
        """Check if a pixel position is deadly."""
        block_x, block_y = self.pixel_to_block(pixel_x, pixel_y)
        block_val = self.get_block(block_x, block_y)
        return block_val == 200
    
    def update_player_collision(self):
        """Handle player collision with level (in pixel space)."""
        # Check if player fell off the bottom
        if self.player.y > self.level_height * self.block_size:
            self.player.alive = False
            self.game_over = True
            return
        
        bounds = self.player.get_bounds()
        
        # Ground collision - check downward movement
        self.player.on_ground = False
        if self.player.velocity_y > 0:  # Moving down
            # Check the bottom row of the player hitbox
            for dx in range(0, self.player.size, 2):  # Check every 2 pixels for performance
                check_x = bounds['left'] + dx
                check_y = bounds['bottom']
                
                if self.is_deadly_at_pixel(check_x, check_y):
                    self.player.alive = False
                    self.game_over = True
                    return
                
                if self.is_solid_at_pixel(check_x, check_y):
                    # Snap player to top of the block
                    block_y = int(check_y // self.block_size)
                    self.player.y = block_y * self.block_size - self.player.size
                    self.player.velocity_y = 0
                    self.player.on_ground = True
        
        # Ceiling collision - check upward movement
        if self.player.velocity_y < 0:  # Moving up
            for dx in range(0, self.player.size, 2):
                check_x = bounds['left'] + dx
                check_y = bounds['top']
                
                if self.is_deadly_at_pixel(check_x, check_y):
                    self.player.alive = False
                    self.game_over = True
                    return
                
                if self.is_solid_at_pixel(check_x, check_y):
                    # Hit ceiling - snap down
                    block_y = int(check_y // self.block_size)
                    self.player.y = (block_y + 1) * self.block_size
                    self.player.velocity_y = 0
        
        # Side collisions - check front of player
        bounds = self.player.get_bounds()  # Recalculate after position adjustments
        for dy in range(0, self.player.size, 2):
            check_x = bounds['right']
            check_y = bounds['top'] + dy
            
            if self.is_deadly_at_pixel(check_x, check_y):
                self.player.alive = False
                self.game_over = True
                return
    
    def update(self):
        """Update game state."""
        if self.game_over or self.win:
            return
        
        # Update player physics
        self.player.update()
        
        # Move player forward automatically (in pixels)
        self.player.x += self.scroll_speed * self.block_size
        
        # Update collision detection
        self.update_player_collision()
        
        # Auto-scroll camera (in blocks)
        self.camera_block_x += self.scroll_speed
        self.score = int(self.camera_block_x)
        
        # Check if player fell behind (convert camera position to pixels for comparison)
        camera_pixel_x = self.camera_block_x * self.block_size
        if self.player.x < camera_pixel_x - 20:
            self.player.alive = False
            self.game_over = True
        
        # Check win condition
        if self.camera_block_x + self.blocks_visible_x >= self.level_width:
            self.win = True
    

    
    def reset(self):
        """Reset game to initial state."""
        player_y = (self.level_height - 2) * self.block_size
        self.player = Player(x=100, y=player_y, size=16)
        self.camera_block_x = 0
        self.game_over = False
        self.win = False
        self.score = 0


def draw_game(buffer, game_state):
    """Render game to a 640x400 framebuffer."""
    # Clear screen
    for y in range(400):
        for x in range(640):
            buffer[y][x] = 0
    
    # Calculate which blocks are visible
    start_block_x = int(game_state.camera_block_x)
    end_block_x = start_block_x + game_state.blocks_visible_x + 1
    
    # Camera offset in pixels (for smooth scrolling)
    camera_pixel_offset = (game_state.camera_block_x - start_block_x) * game_state.block_size
    
    # Draw level blocks scaled up
    block_size = game_state.block_size
    
    for block_y in range(game_state.level_height):
        for block_x in range(start_block_x, min(end_block_x, game_state.level_width)):
            block_value = game_state.level[block_y][block_x]
            
            if block_value > 0:  # Don't draw air (0)
                # Calculate screen position
                screen_x = (block_x - start_block_x) * block_size - int(camera_pixel_offset)
                screen_y = block_y * block_size
                
                # Draw the block as a 20x20 square
                for dy in range(block_size):
                    for dx in range(block_size):
                        px = screen_x + dx
                        py = screen_y + dy
                        if 0 <= px < 640 and 0 <= py < 400:
                            buffer[py][px] = block_value
    
    # Draw player (player position is in pixels, relative to level)
    if game_state.player.alive:
        camera_pixel_x = game_state.camera_block_x * game_state.block_size
        
        player_screen_x = int(game_state.player.x - camera_pixel_x)
        player_screen_y = int(game_state.player.y)
        
        for dy in range(game_state.player.size):
            for dx in range(game_state.player.size):
                px = player_screen_x + dx
                py = player_screen_y + dy
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
