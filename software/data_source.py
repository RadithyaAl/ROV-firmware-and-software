import pygame

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    raise Exception("No joystick detected")

j = pygame.joystick.Joystick(0)
j.init()

def deadzone(v, t=0.1):
    return 0 if abs(v) < t else v

def normalize(vals):
    m = max(abs(v) for v in vals)
    return [v/m if m > 1 else v for v in vals]

def get_data():
    pygame.event.pump()

    # --- INPUT ---
    x = deadzone(-j.get_axis(0))
    y = deadzone(-j.get_axis(1))
    yaw = deadzone(-j.get_axis(2))

    # --- D-PAD ---
    hat_x, hat_y = j.get_hat(0)

    # --- HORIZONTAL THRUSTERS ---
    FL = y + x + yaw
    FR = y - x - yaw
    BL = -y + x + yaw
    BR = -y - x - yaw

    FL, FR, BL, BR = normalize([FL, FR, BL, BR])

    # --- VERTICAL ---
    z = hat_y
    VL = z
    VR = z

    # --- SCALE ---
    a = int(FL * 100)
    b = int(FR * 100)
    c = int(BL * 100)
    d = int(BR * 100)
    e = int(VL * 100)
    f = int(VR * 100)
    g = 0
    h = 0

    return [a, b, c, d, e, f, g, h]