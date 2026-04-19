import pygame
import sys
import math

pygame.init()

width = 800
height = 600

screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Solar System Project")

black = (0, 0, 0)
yellow = (255, 255, 0)
blue = (0, 100, 255)
white = (255, 255, 255)
gray = (120, 120, 120)
light_gray = (180, 180, 180)

def plot_pixel(x, y, color):
    if 0 <= int(x) < width and 0 <= int(y) < height:
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

def draw_circle_points(xc, yc, x, y, color):
    plot_pixel(xc + x, yc + y, color)
    plot_pixel(xc - x, yc + y, color)
    plot_pixel(xc + x, yc - y, color)
    plot_pixel(xc - x, yc - y, color)
    plot_pixel(xc + y, yc + x, color)
    plot_pixel(xc - y, yc + x, color)
    plot_pixel(xc + y, yc - x, color)
    plot_pixel(xc - y, yc - x, color)

def midpoint_circle(xc, yc, r, color):
    x = 0
    y = r
    p = 1 - r

    draw_circle_points(xc, yc, x, y, color)

    while x < y:
        x += 1
        if p < 0:
            p = p + 2 * x + 1
        else:
            y -= 1
            p = p + 2 * x - 2 * y + 1

        draw_circle_points(xc, yc, x, y, color)

def draw_filled_circle(xc, yc, r, color):
    for current_r in range(r, 0, -1):
        midpoint_circle(xc, yc, current_r, color)

sun_x = 400
sun_y = 300

earth_orbit_radius = 150
moon_orbit_radius = 35

earth_angle = 0
moon_angle = 0

clock = pygame.time.Clock()
running = True

while running:
    screen.fill(black)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    earth_angle += 0.02
    moon_angle += 0.08

    earth_x = sun_x + earth_orbit_radius * math.cos(earth_angle)
    earth_y = sun_y + earth_orbit_radius * math.sin(earth_angle)

    moon_x = earth_x + moon_orbit_radius * math.cos(moon_angle)
    moon_y = earth_y + moon_orbit_radius * math.sin(moon_angle)

    # Orbit path of Earth
    midpoint_circle(sun_x, sun_y, earth_orbit_radius, gray)

    # Sun
    draw_filled_circle(sun_x, sun_y, 40, yellow)

    # Earth
    draw_filled_circle(int(earth_x), int(earth_y), 15, blue)

    # Moon
    draw_filled_circle(int(moon_x), int(moon_y), 6, white)

    # DDA line from Sun to Earth
    dda_line(sun_x, sun_y, int(earth_x), int(earth_y), light_gray)

    pygame.display.update()
    clock.tick(60)

pygame.quit()
sys.exit()