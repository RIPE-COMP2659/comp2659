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
        # Handle events first
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
                elif event.key == pygame.K_r:
                    game_state.reset()
                elif event.key in (pygame.K_SPACE, pygame.K_UP):
                    game_state.player.jump()
        
        # Also allow holding space to jump
        keys = pygame.key.get_pressed()
        if keys[pygame.K_SPACE] or keys[pygame.K_UP]:
            game_state.player.jump()
        
        # Update game
        game_state.update()
        
        # Render
        buffer = create_buffer()
        draw_game(buffer, game_state)
        
        display.set_framebuffer(buffer)
        display.refresh()
        
        # Frame rate control
        display.tick(60)
    
    display.cleanup()


if __name__ == "__main__":
    main()

