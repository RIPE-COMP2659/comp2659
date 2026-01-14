"""
Rendering functions for Geometry Dash
"""
TEMP_PLAYER_SIZE = 20

def draw_rect(buffer, x, y, width, height, color):
    """Draw a filled rectangle"""
    for dy in range(height):
        for dx in range(width):
            px = int(x + dx)
            py = int(y + dy)
            if 0 <= px < 640 and 0 <= py < 400:
                buffer[py][px] = color


def draw_line(buffer, y, color):
    """Draw a horizontal line"""
    if 0 <= y < 400:
        for x in range(640):
            buffer[y][x] = color


def draw_text_simple(buffer, text, x, y, size=2):
    """Very simple text rendering - just blocks for numbers"""
    # Simple digit patterns (3x5 pixels each)
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
    }
    
    offset_x = 0
    for char in str(text):
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
            offset_x += 4 * size  # Space between digits


def draw_game(buffer, game_state):
    """Main rendering function"""
    # Clear screen (black background)
    for y in range(400):
        for x in range(640):
            buffer[y][x] = 0
    
    if not game_state.game_over:
        # Draw ground line
        draw_line(buffer, game_state.ground_y, 200)
        
        # Draw player (white square)
        draw_rect(buffer, 
                 game_state.player.x, 
                 game_state.player.y, 
                 TEMP_PLAYER_SIZE, 
                 TEMP_PLAYER_SIZE, 
                 255)
        
        # Draw obstacles (gray)
        for obstacle in game_state.obstacles:
            draw_rect(buffer,
                     obstacle.x,
                     obstacle.y,
                     obstacle.width,
                     obstacle.height,
                     180)
        
        # Draw score
        draw_text_simple(buffer, game_state.score, 10, 10, size=3)
    
    else:
        # Game over screen
        # Draw "GAME OVER" in the middle (simplified)
        draw_rect(buffer, 250, 180, 140, 40, 150)
        draw_text_simple(buffer, game_state.score, 300, 250, size=4)