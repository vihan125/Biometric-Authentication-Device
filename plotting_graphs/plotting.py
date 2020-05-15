import numpy as np 

import matplotlib.pyplot as plt 

'''making grid'''
t=np.linspace(-3,3,1000)
ax1=plt.subplot2grid((3,2),(0,0),colspan=1)

'''graph define'''
u_t = (t>=0)
plt.plot(t,u_t,color='k')
plt.ylim(-.25,1.25)

'''setting vertical and horizontal lines at y=o and x=0'''
plt.axhline(0)
plt.axhline(0,color='b',lineWidth=0.5)
plt.axvline(0,color='b',lineWidth=0.5)
plt.show()
