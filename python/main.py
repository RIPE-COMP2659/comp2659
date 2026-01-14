"""
Atari ST Game Prototype
"""
from atari_display import AtariSTDisplay, create_buffer


def main():
    # Initialize display
    display = AtariSTDisplay(scale=2)

    running = True
    frame = 0

    while running:
        # Create frame buffer
        buffer = create_buffer()
        
        # YOUR GAME LOGIC HERE
        # Example: horizontal gradient with moving vertical line
        for y in range(400):
            gray = (y * 255) // 400
            for x in range(640):
                buffer[y][x] = gray
        
        # Moving vertical line
        x = frame % 640
        for y in range(400):
            buffer[y][x] = 255
        
        # Update display
        display.set_framebuffer(buffer)
        display.refresh()
        
        # Handle events and timing
        running = not display.should_quit()
        display.tick(60)
        
        frame += 1

    display.cleanup()


if __name__ == "__main__":
    main()