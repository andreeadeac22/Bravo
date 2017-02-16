"""
Generate dummy data that looks a bit like a chasm :). Since we don't yet have
any high-resolution data from BAS, this should be a decent approximation to
what we want.

Parameters:

width (metres)
height (metres)

resolution (metres)
incline (metres/metre)
incline direction (radians) (anticlockwise from x axis, towards y axis)
"""
import numpy as np
import matplotlib.pyplot as plt

w = 100
h = 100
resolution = 1

incline = 0.001 # 1% incline?
incline_direction = 0 # along x axis (positive), equivalent to rotating about the y axis

X = np.arange(w // resolution)
Y = np.arange(h // resolution)

X, Y = np.meshgrid(X, Y)

Z = ((X * np.cos(incline_direction)) + (Y * np.sin(incline_direction))) * incline
Z = ((X * np.cos(X*100)) + (Y * np.sin(incline_direction))) * incline

print(Z)

from mpl_toolkits.mplot3d import axes3d, Axes3D
fig = plt.figure()

ax = Axes3D(fig)
cset = ax.plot_wireframe(X, Y, Z, rstride=5, cstride=5)
ax.clabel(cset, fontsize=9, inline=1)
plt.show()
