"""
Generate a path between two points as a fractal. As the points get closer
together, peturbe the midpoint by slightly less.
"""
import numpy as np
import matplotlib.pyplot as plt

# between two sides of a square (100x100)

start = np.array((30.0, 0.0))
end = np.array((70.0, 100.0))

bounds = [(0.0,0.0), (0.0,100.0), (100.0,100.0), (100.0,0.0)] # anticlockwise

def fractal(a, b, bounds, limit, divisor):
    distance = np.linalg.norm(a - b)

    if distance <= limit:
        return []

    c = (a + b) / 2
    cx = c[0]
    cy = c[1]

    # now generate a random x and y shift for the centre point

    dx = np.random.normal(0.0, scale=distance/divisor)
    dy = np.random.normal(0.0, scale=distance/divisor)

    x = cx + dx
    y = cy + dy

    c = np.array([x, y])

    left = fractal(a, c, bounds, limit, divisor)
    right = fractal(c, b, bounds, limit, divisor)

    return [a] + left + [c] + right + [b]


path = fractal(start, end, None, 0.1, 10.0)

x, y = zip(*path)

plt.plot(x, y)
plt.show()

