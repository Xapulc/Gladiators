import numpy as np
import sys
import pylab
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm

from stats.utils import get_win_probability


def p_left(a, b):
    def helper(alpha, beta):
        a1 = alpha * a
        a2 = (1 - alpha) * a
        b1 = beta * b
        b2 = (1 - beta) * b
        return get_win_probability([a1, a2], [b1, b2], "test")[0]
    
    return helper


def get_grid_values(a, b):
    x = np.linspace(0.0001, 0.9999)
    y = np.linspace(0.0001, 0.9999)
    xgrid, ygrid = np.meshgrid(x, y)
    zgrid = np.empty((x.shape[0], y.shape[0]))
    for i, val_x in enumerate(x):
        for j, val_y in enumerate(y):
            zgrid[i, j] = p_left(a, b)(val_x, val_y)
    return xgrid, ygrid, zgrid


if __name__ == "__main__":
    a = float(sys.argv[1]) if len(sys.argv) > 1 else 1
    b = float(sys.argv[2]) if len(sys.argv) > 2 else 1
    
    x, y, z = get_grid_values(a, b)

    fig = pylab.figure()
    axes = Axes3D(fig)

    axes.plot_surface(x, y, z, 
                      rstride=5, cstride=5,
                      cmap=cm.Spectral)
    axes.set_xlabel("alpha")
    axes.set_ylabel("beta")
    axes.set_zlabel("Probability of win left team")
    pylab.show()