import numpy as np
import matplotlib.pyplot as plt

# Simulation parameters
dt = 0.001
t_end = 5
t = np.arange(0, t_end, dt)

# Second-order system parameters
wn = 5.0      # natural frequency
zeta = 0.5    # damping ratio

# PID parameters
Kp = 50.0
Ki = 30.0
Kd = 5.0

# Setpoint
setpoint = 1.0

# Initialize variables
x1 = 0.0  # position
x2 = 0.0  # velocity

integral = 0.0
prev_error = 0.0

# Store results
y = []
u_list = []

for time in t:
    # Output
    y_current = x1
    
    # Error
    error = setpoint - y_current
    
    # PID calculations
    integral += error * dt
    derivative = (error - prev_error) / dt
    
    u = Kp * error + Ki * integral + Kd * derivative
    
    # System dynamics (2nd order ODE)
    dx1 = x2
    dx2 = wn**2 * (u - x1) - 2 * zeta * wn * x2
    
    # Euler integration
    x1 += dx1 * dt
    x2 += dx2 * dt
    
    prev_error = error
    
    # Store
    y.append(y_current)
    u_list.append(u)

# Plot output response
plt.figure(figsize=(10,5))
plt.plot(t, y, label="System Output")
plt.axhline(setpoint, color='r', linestyle='--', label="Setpoint")
plt.xlabel("Time (s)")
plt.ylabel("Output")
plt.title("PID Control of 2nd Order System")
plt.legend()
plt.grid()

# Plot control signal
plt.figure(figsize=(10,5))
plt.plot(t, u_list, label="Control Input (u)", color='g')
plt.xlabel("Time (s)")
plt.ylabel("Control Signal")
plt.title("Control Effort")
plt.legend()
plt.grid()

plt.show()