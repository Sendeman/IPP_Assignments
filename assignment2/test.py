import numpy as np

startx = 0
stopx = 1
stepsize=.05
lastx = startx

# n= (stopx-startx)/stepsize
f = lambda x: 4/(1+x**2)
traparea = lambda f, a, b: (f(a)+ f(b))*((b-a)/2)

totalarea = 0
for x in np.arange(startx+stepsize, stopx+stepsize, stepsize):
    # print(lastx, x)
    print(lastx, x, traparea(f, lastx, x))
    totalarea+=traparea(f, lastx, x)
    lastx = x

print(totalarea)

# print(f(10))
