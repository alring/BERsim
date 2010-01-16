#!/bin/python
import numpy as np
import matplotlib.pyplot as plt
import os
from mpl_toolkits.axes_grid import make_axes_locatable


def isNum(txt):
    try:
        float(txt)
        return True
    except ValueError:
        return False

symfile = open('/tmp/symbols_noise.txt','r')
flist = symfile.readlines()

flag=0
x=[]
y=[]
for str in flist:
	str=str.rstrip('\n')
	if str.find('Real') != -1:
		flag=1
	elif flag==1 and str.find('Imag') != -1:
		flag=0
	elif flag==1 and isNum(str):
		x.append(float(str))
	elif flag==0 and isNum(str):
		y.append(float(str))


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


os.remove('/tmp/symbols.txt')
os.remove('/tmp/symbols_noise.txt')
