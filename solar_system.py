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
yellow = (255, 230, 0)
blue = (50, 140, 255)
white = (255, 255, 255)
gray = (70, 70, 70)
light_gray = (150, 150, 150)
red = (255, 95, 75)

venus_color = (255, 180, 0)
jupiter_color = (210, 170, 120)
saturn_color = (220, 205, 150)

sun_glow_1 = (80, 60, 0)
sun_glow_2 = (140, 100, 0)
earth_glow = (20, 60, 120)
mars_glow = (100, 30, 20)
venus_glow = (120, 70, 0)
jupiter_glow = (100, 70, 40)
saturn_glow = (110, 100, 60)
moon_glow = (120, 120, 120)

# Font
font = pygame.font.SysFont("Calibri", 18)

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
venus_orbit_radius = 100
earth_orbit_radius = 140
mars_orbit_radius = 185
jupiter_orbit_radius = 210
saturn_orbit_radius = 250
moon_orbit_radius = 30

# Angles for animation
venus_angle = 0
earth_angle = 0
mars_angle = 0
jupiter_angle = 0
saturn_angle = 0
moon_angle = 0
sun_scale_angle = 0
twinkle_angle = 0

comet_x = 80
comet_y = 180
comet_speed_x = 2
comet_speed_y = 1

# Stars
stars = [
    (60, 70, 1), (120, 140, 1), (200, 80, 2), (300, 60, 1), (500, 100, 2),
    (650, 70, 1), (730, 150, 2), (100, 500, 1), (220, 430, 2), (350, 520, 1),
    (480, 460, 1), (620, 540, 2), (750, 400, 1), (700, 250, 1), (90, 250, 2),
    (50, 300, 1), (150, 600, 2), (260, 200, 1), (410, 140, 2), (560, 620, 1),
    (680, 320, 2), (820, 180, 1), (850, 500, 2), (300, 650, 1), (520, 260, 1)
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
        venus_angle += 0.026 * speed_multiplier
        earth_angle += 0.02 * speed_multiplier
        mars_angle += 0.012 * speed_multiplier
        jupiter_angle += 0.010 * speed_multiplier
        saturn_angle += 0.008 * speed_multiplier
        moon_angle += 0.03 * speed_multiplier
        sun_scale_angle += 0.05 * speed_multiplier
        twinkle_angle += 0.08 * speed_multiplier
        
        comet_x += comet_speed_x * speed_multiplier
        comet_y += comet_speed_y * speed_multiplier

        if comet_x > width + 40 or comet_y > height + 40:
            comet_x = -40
            comet_y = 180

    # Rotation / revolution positions
    venus_x = sun_x + venus_orbit_radius * math.cos(venus_angle)
    venus_y = sun_y + venus_orbit_radius * math.sin(venus_angle)

    earth_x = sun_x + earth_orbit_radius * math.cos(earth_angle)
    earth_y = sun_y + earth_orbit_radius * math.sin(earth_angle)

    mars_x = sun_x + mars_orbit_radius * math.cos(mars_angle)
    mars_y = sun_y + mars_orbit_radius * math.sin(mars_angle)

    jupiter_x = sun_x + jupiter_orbit_radius * math.cos(jupiter_angle)
    jupiter_y = sun_y + jupiter_orbit_radius * math.sin(jupiter_angle)

    saturn_x = sun_x + saturn_orbit_radius * math.cos(saturn_angle)
    saturn_y = sun_y + saturn_orbit_radius * math.sin(saturn_angle)

    moon_x = earth_x + moon_orbit_radius * math.cos(moon_angle)
    moon_y = earth_y + moon_orbit_radius * math.sin(moon_angle)
    
    # Scaling effect for sun
    sun_radius = int(40 + 3 * math.sin(sun_scale_angle))

    # Draw twinkling stars
    for i, (sx, sy, size) in enumerate(stars):
        if int((math.sin(twinkle_angle + i) + 1) * 2) % 2 == 0:
            if size == 1:
                plot_pixel(sx, sy, white)
            else:
                draw_filled_circle(sx, sy, 2, white)

    # Draw orbit paths
    midpoint_circle(sun_x, sun_y, earth_orbit_radius, gray)
    midpoint_circle(sun_x, sun_y, mars_orbit_radius, gray)
    midpoint_circle(sun_x, sun_y, saturn_orbit_radius, gray)
    midpoint_circle(sun_x, sun_y, jupiter_orbit_radius, gray)
    midpoint_circle(sun_x, sun_y, saturn_orbit_radius, gray)
    
    # Sun glow
    draw_filled_circle(sun_x, sun_y, sun_radius + 12, (80, 60, 0))
    draw_filled_circle(sun_x, sun_y, sun_radius + 6, (140, 100, 0))

    # Sun glow layers
    draw_filled_circle(sun_x, sun_y, sun_radius + 18, sun_glow_1)
    draw_filled_circle(sun_x, sun_y, sun_radius + 10, sun_glow_2)
    draw_filled_circle(sun_x, sun_y, sun_radius + 4, (180, 130, 0))

    # Sun core
    draw_filled_circle(sun_x, sun_y, sun_radius, yellow)

    # Draw venus
    draw_filled_circle(int(venus_x), int(venus_y), 14, venus_glow)
    draw_filled_circle(int(venus_x), int(venus_y), 10, venus_color)

    # Draw earth
    draw_filled_circle(int(earth_x), int(earth_y), 20, earth_glow)
    draw_filled_circle(int(earth_x), int(earth_y), 15, blue)

    # Draw moon
    draw_filled_circle(int(moon_x), int(moon_y), 8, moon_glow)
    draw_filled_circle(int(moon_x), int(moon_y), 5, white)

    # Draw mars
    draw_filled_circle(int(mars_x), int(mars_y), 16, mars_glow)
    draw_filled_circle(int(mars_x), int(mars_y), 12, red)

    # Draw jupiter
    draw_filled_circle(int(jupiter_x), int(jupiter_y), 21, jupiter_glow)
    draw_filled_circle(int(jupiter_x), int(jupiter_y), 16, jupiter_color)

    # Draw saturn
    draw_filled_circle(int(saturn_x), int(saturn_y), 18, saturn_glow)
    draw_filled_circle(int(saturn_x), int(saturn_y), 14, saturn_color)
    
    # Saturn ring
    dda_line(int(saturn_x - 24), int(saturn_y - 3), int(saturn_x + 24), int(saturn_y - 3), light_gray)
    dda_line(int(saturn_x - 26), int(saturn_y), int(saturn_x + 26), int(saturn_y), white)
    dda_line(int(saturn_x - 24), int(saturn_y + 3), int(saturn_x + 24), int(saturn_y + 3), light_gray)

    # Labels
    sun_label = font.render("Sun", True, white)
    venus_label = font.render("Venus", True, white)
    earth_label = font.render("Earth", True, white)
    moon_label = font.render("Moon", True, white)
    mars_label = font.render("Mars", True, white)
    jupiter_label = font.render("Jupiter", True, white)
    saturn_label = font.render("Saturn", True, white)
    comet_label = font.render("Comet", True, white)

    screen.blit(sun_label, (sun_x - 16, sun_y + sun_radius + 10))
    screen.blit(venus_label, (int(venus_x) + 10, int(venus_y) + 8))
    screen.blit(earth_label, (int(earth_x) - 18, int(earth_y) + 28))
    screen.blit(moon_label, (int(moon_x) + 14, int(moon_y) - 10))
    screen.blit(mars_label, (int(mars_x) - 18, int(mars_y) + 22))
    screen.blit(jupiter_label, (int(jupiter_x) - 28, int(jupiter_y) + 24))
    screen.blit(saturn_label, (int(saturn_x) - 28, int(saturn_y) + 24))
    screen.blit(comet_label, (int(comet_x) + 16, int(comet_y) - 8))
    
    # Draw DDA lines
    dda_line(sun_x, sun_y, int(earth_x), int(earth_y), light_gray)
    dda_line(sun_x, sun_y, int(mars_x), int(mars_y), light_gray)
    dda_line(sun_x, sun_y, int(saturn_x), int(saturn_y), light_gray)
    
    # Draw comet head
    draw_filled_circle(int(comet_x), int(comet_y), 5, white)
    draw_filled_circle(int(mars_x), int(mars_y), 12, red)
    draw_filled_circle(int(saturn_x), int(saturn_y), 14, saturn_color)

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

    legend1 = font.render("Animated: Venus, Earth, Moon, Mars, Jupiter, Saturn, Comet", True, white)
    legend2 = font.render("Algorithms: DDA Line, Midpoint Circle", True, white)

    screen.blit(legend1, (20, height - 70))
    screen.blit(legend2, (20, height - 42))
    pygame.display.update()
    clock.tick(60)
pygame.quit()
sys.exit()