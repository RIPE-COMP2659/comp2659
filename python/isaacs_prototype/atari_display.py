"""
Atari ST Display Driver Abstraction
Provides a 640x400 monochrome framebuffer interface
"""
import pygame

class AtariSTDisplay:
    """
    Abstraction layer that mimics your Atari ST graphics driver interface.
    Accepts a 2D list (640x400) with grayscale values 0-255.
    """
    
    def __init__(self, width=640, height=400, scale=1):
        pygame.init()
        self.width = width
        self.height = height
        self.scale = scale
        
        self.screen = pygame.display.set_mode((width * scale, height * scale))
        pygame.display.set_caption("Atari ST Prototype")
        
        self.framebuffer = [[0 for _ in range(width)] for _ in range(height)]
        self.surface = pygame.Surface((width, height))
        self.clock = pygame.time.Clock()
    
    def set_framebuffer(self, buffer):
        """Set the framebuffer from a 2D list [height][width] with values 0-255"""
        if len(buffer) != self.height:
            raise ValueError(f"Buffer must have {self.height} rows")
        if any(len(row) != self.width for row in buffer):
            raise ValueError(f"All rows must have {self.width} columns")
        
        self.framebuffer = buffer
    
    def refresh(self):
        """Render the framebuffer to screen"""
        # Convert 2D list to bytes for fast rendering
        try:
            import numpy as np
            # Use numpy for fast conversion
            gray_array = np.array(self.framebuffer, dtype=np.uint8)
            gray_array = np.clip(gray_array, 0, 255)
            # Create RGB array
            rgb_array = np.stack([gray_array] * 3, axis=-1)
            # Transpose for pygame (x, y, color) format
            surface = pygame.surfarray.make_surface(np.transpose(rgb_array, (1, 0, 2)))
        except ImportError:
            # Fallback to slower method if numpy not available
            for y in range(self.height):
                for x in range(self.width):
                    gray = max(0, min(255, self.framebuffer[y][x]))
                    color = (gray, gray, gray)
                    self.surface.set_at((x, y), color)
            surface = self.surface
        
        if self.scale != 1:
            scaled = pygame.transform.scale(surface, 
                                           (self.width * self.scale, 
                                            self.height * self.scale))
            self.screen.blit(scaled, (0, 0))
        else:
            self.screen.blit(surface, (0, 0))
        
        pygame.display.flip()
    
    def should_quit(self):
        """Handle window events, returns True if window closed"""
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return True
        return False
    
    def tick(self, fps=60):
        """Frame rate control"""
        self.clock.tick(fps)
    
    def cleanup(self):
        """Clean up pygame resources"""
        pygame.quit()


def create_buffer(width=640, height=400):
    """Helper function to create an empty framebuffer"""
    return [[0 for _ in range(width)] for _ in range(height)]
