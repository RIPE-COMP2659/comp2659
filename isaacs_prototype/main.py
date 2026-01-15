"""
Geometry Dash Clone - Atari ST Prototype
"""
from atari_display import AtariSTDisplay, create_buffer
from game import GameState, draw_game
import pygame


def main():
    display = AtariSTDisplay(scale=2)
    game_state = GameState('world.csv')
    
    running = True
    
    while running:
        # Handle input
        keys = pygame.key.get_pressed()
        if keys[pygame.K_SPACE] or keys[pygame.K_UP]:
            game_state.player.jump()
        
        # Check for restart
        if (game_state.game_over or game_state.win) and keys[pygame.K_r]:
            game_state.reset()
        
        # Update game
        game_state.update()
        
        # Render
        buffer = create_buffer()
        draw_game(buffer, game_state)
        
        display.set_framebuffer(buffer)
        display.refresh()
        
        # Handle events and timing
        running = not display.should_quit()
        display.tick(60)
    
    display.cleanup()


if __name__ == "__main__":
    main()

