#!/bin/python
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid import make_axes_locatable


fig = plt.figure(1, figsize=(10,10), dpi=50)

axScatter = plt.subplot(111)
divider = make_axes_locatable(axScatter)

axScatter.scatter(x, y)
axScatter.set_aspect(1.)

axes = fig.get_axes()[0]
axes.set_xlim((-3, 3))
axes.set_ylim((-3, 3))
axes.set_aspect('auto', adjustable='box')

plt.draw()
plt.show()
