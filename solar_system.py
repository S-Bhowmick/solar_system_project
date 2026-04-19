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
white = (255, 255, 255)

def plot_pixel(x, y, color):
    screen.set_at((int(x), int(y)), color)

def dda_line(x1, y1, x2, y2, color):
    dx = x2 - x1
    dy = y2 - y1

    steps = max(abs(dx), abs(dy))

    if steps == 0:
        plot_pixel(x1, y1, color)
        return

    x_increment = dx / steps
    y_increment = dy / steps

    x = x1
    y = y1

    for i in range(int(steps) + 1):
        plot_pixel(round(x), round(y), color)
        x += x_increment
        y += y_increment

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

    # DDA line
    dda_line(100, 100, 300, 200, white)

    pygame.display.update()

pygame.quit()
sys.exit()