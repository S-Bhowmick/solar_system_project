import pygame
import sys
import math

pygame.init()

# Window size
width = 900
height = 700

# Create screen
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("2D Animated Solar System")

# Colors
black = (0, 0, 0)
yellow = (255, 255, 0)
blue = (0, 120, 255)
white = (255, 255, 255)
gray = (120, 120, 120)
light_gray = (180, 180, 180)
red = (255, 80, 80)

# Font
font = pygame.font.SysFont("Arial", 20)

# Plot one pixel
def plot_pixel(x, y, color):
    if 0 <= int(x) < width and 0 <= int(y) < height:
        screen.set_at((int(x), int(y)), color)

# DDA Line Algorithm
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

    for _ in range(int(steps) + 1):
        plot_pixel(round(x), round(y), color)
        x += x_increment
        y += y_increment

# Draw 8 symmetric points of circle
def draw_circle_points(xc, yc, x, y, color):
    plot_pixel(xc + x, yc + y, color)
    plot_pixel(xc - x, yc + y, color)
    plot_pixel(xc + x, yc - y, color)
    plot_pixel(xc - x, yc - y, color)
    plot_pixel(xc + y, yc + x, color)
    plot_pixel(xc - y, yc + x, color)
    plot_pixel(xc + y, yc - x, color)
    plot_pixel(xc - y, yc - x, color)

# Midpoint Circle Algorithm
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

# Fill circle by drawing many circle outlines
def draw_filled_circle(xc, yc, r, color):
    for current_r in range(r, 0, -1):
        midpoint_circle(xc, yc, current_r, color)

# Center of solar system
sun_x = 450
sun_y = 430

# Orbit sizes
earth_orbit_radius = 140
mars_orbit_radius = 185
saturn_orbit_radius = 220
moon_orbit_radius = 30

# Angles for animation
earth_angle = 0
mars_angle = 0
saturn_angle = 0
moon_angle = 0
sun_scale_angle = 0

comet_x = 80
comet_y = 180
comet_speed_x = 2
comet_speed_y = 1

# Stars
stars = [
    (60, 70), (120, 140), (200, 80), (300, 60), (500, 100),
    (650, 70), (730, 150), (100, 500), (220, 430), (350, 520),
    (480, 460), (620, 540), (750, 400), (700, 250), (90, 250)
]

clock = pygame.time.Clock()
running = True
paused = False
speed_multiplier = 1.0

while running:
    screen.fill(black)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                paused = not paused
                print("Paused:", paused)

            elif event.key == pygame.K_UP:
                speed_multiplier += 0.2

            elif event.key == pygame.K_DOWN:
                speed_multiplier = max(0.2, speed_multiplier - 0.2)

    # Animation speed
    if not paused:
        earth_angle += 0.02 * speed_multiplier
        mars_angle += 0.012 * speed_multiplier
        saturn_angle += 0.009 * speed_multiplier
        moon_angle += 0.05 * speed_multiplier
        sun_scale_angle += 0.05 * speed_multiplier
        
        comet_x += comet_speed_x * speed_multiplier
        comet_y += comet_speed_y * speed_multiplier

        if comet_x > width + 40 or comet_y > height + 40:
            comet_x = -40
            comet_y = 180

    # Rotation / revolution positions
    earth_x = sun_x + earth_orbit_radius * math.cos(earth_angle)
    earth_y = sun_y + earth_orbit_radius * math.sin(earth_angle)

    mars_x = sun_x + mars_orbit_radius * math.cos(mars_angle)
    mars_y = sun_y + mars_orbit_radius * math.sin(mars_angle)
    
    saturn_x = sun_x + saturn_orbit_radius * math.cos(saturn_angle)
    saturn_y = sun_y + saturn_orbit_radius * math.sin(saturn_angle)

    moon_x = earth_x + moon_orbit_radius * math.cos(moon_angle)
    moon_y = earth_y + moon_orbit_radius * math.sin(moon_angle)

    # Scaling effect for sun
    sun_radius = int(40 + 3 * math.sin(sun_scale_angle))

    # Draw stars
    for sx, sy in stars:
        plot_pixel(sx, sy, white)

    # Draw orbit paths
    midpoint_circle(sun_x, sun_y, earth_orbit_radius, gray)
    midpoint_circle(sun_x, sun_y, mars_orbit_radius, gray)
    midpoint_circle(sun_x, sun_y, saturn_orbit_radius, gray)
    
    # Draw sun
    draw_filled_circle(sun_x, sun_y, sun_radius, yellow)

    # Draw earth
    draw_filled_circle(int(earth_x), int(earth_y), 15, blue)

    # Draw moon
    draw_filled_circle(int(moon_x), int(moon_y), 6, white)

    # Draw mars
    draw_filled_circle(int(mars_x), int(mars_y), 12, red)
    
    # Draw saturn
    draw_filled_circle(int(saturn_x), int(saturn_y), 14, (210, 180, 120))
    
    # Saturn ring
    dda_line(int(saturn_x - 20), int(saturn_y), int(saturn_x + 20), int(saturn_y), light_gray)
    dda_line(int(saturn_x - 18), int(saturn_y + 3), int(saturn_x + 18), int(saturn_y + 3), light_gray)

    # Labels
    sun_label = font.render("Sun", True, white)
    earth_label = font.render("Earth", True, white)
    moon_label = font.render("Moon", True, white)
    mars_label = font.render("Mars", True, white)
    saturn_label = font.render("Saturn", True, white)
    comet_label = font.render("Comet", True, white)

    screen.blit(sun_label, (sun_x - 18, sun_y + sun_radius + 8))
    screen.blit(earth_label, (int(earth_x) - 18, int(earth_y) + 22))
    screen.blit(moon_label, (int(moon_x) + 8, int(moon_y) - 18))
    screen.blit(mars_label, (int(mars_x) - 15, int(mars_y) + 18))
    screen.blit(saturn_label, (int(saturn_x) - 22, int(saturn_y) + 18))
    screen.blit(comet_label, (int(comet_x) + 12, int(comet_y) - 12))
    
    # Draw DDA lines
    dda_line(sun_x, sun_y, int(earth_x), int(earth_y), light_gray)
    dda_line(sun_x, sun_y, int(mars_x), int(mars_y), light_gray)
    dda_line(sun_x, sun_y, int(saturn_x), int(saturn_y), light_gray)
    
    # Draw comet head
    draw_filled_circle(int(comet_x), int(comet_y), 5, white)

    title_text = font.render("2D Animated Solar System", True, white)
    info_text = font.render("DDA Line | Midpoint Circle | Translation | Rotation | Scaling", True, white)
    control_text = font.render("UP Faster | DOWN Slower | SPACE Pause", True, white)

    if paused:
        status_text = font.render(f"Paused | Speed: {speed_multiplier:.1f}x", True, white)
    else:
        status_text = font.render(f"Running | Speed: {speed_multiplier:.1f}x", True, white)

    screen.blit(title_text, (300, 10))
    screen.blit(info_text, (145, 38))
    screen.blit(control_text, (240, 66))
    screen.blit(status_text, (340, 92))
    if paused:
        pause_text = font.render("PAUSED", True, white)
        screen.blit(pause_text, (360, 130))

    pygame.display.update()
    clock.tick(60)
pygame.quit()
sys.exit()