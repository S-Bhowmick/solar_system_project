import pygame
import sys

pygame.init()

width = 800
height = 600

screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Solar System Project")

black = (0, 0, 0)
yellow = (255, 255, 0)
blue = (0, 100, 255)

running = True
while running:
    screen.fill(black)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Sun
    pygame.draw.circle(screen, yellow, (400, 300), 40)

    # Planet
    pygame.draw.circle(screen, blue, (550, 300), 15)

    pygame.display.update()

pygame.quit()
sys.exit()