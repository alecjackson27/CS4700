import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

'''
This script was used to plot the runtimes
'''

if __name__ == "__main__":
    h1 = [
        0.0005630004,
        0.0012330746,
        0.0163325699,
        0.2047874787,
        2.5719677302
    ]
    h2 = [
        0.0001357715,
        0.0013392974,
        0.0174812603,
        0.2118689398,
        2.4920810364
    ]
    h3 = [
        0.0001424741,
        0.0012867784,
        0.0232650633,
        0.2593819715,
        3.3937466416
    ]
    h4 = [
        0.0007329674,
        0.0020785929,
        0.0276706885,
        0.3190316348,
        3.9916126198
    ]
    c1 = [
        0.014,
        0.017,
        0.025,
        0.139,
        0.812
    ]
    c2 = [
        0.009,
        0.011,
        0.017,
        0.082,
        0.513
    ]
    c3 = [
        0.005,
        0.006,
        0.011,
        0.066,
        0.249
    ]
    c4 = [
        0.004,
        0.005,
        0.012,
        0.071,
        0.323
    ]
    x = [10 ** x for x in range(2, 7)]

    # plot data
    line1, = plt.plot(x, h1, "g", label='Haskell 1 Core')
    line2, = plt.plot(x, h2, "g", linestyle='--', label='Haskell 2 Cores')
    line3, = plt.plot(x, h3, "g", linestyle='-.', label='Haskell 3 Cores')
    line4, = plt.plot(x, h4, "g", linestyle=':', label='Haskell 2 Cores')

    line5, = plt.plot(x, c1, "b", label='C++ 1 Core')
    line6, = plt.plot(x, c2, "b", linestyle='--', label='C++ 2 Cores')
    line7, = plt.plot(x, c3, "b", linestyle='-.', label='C++ 3 Cores')
    line8, = plt.plot(x, c4, "b", linestyle=':', label='C++ 4 Cores')
    
    green_patch = mpatches.Patch(color='green', label='Haskell')
    blue_patch = mpatches.Patch(color='blue', label='C++')
    plt.legend(handles=[line1, line2, line3, line4, line5, line6, line7, line8])
    
    plt.title('Parallel Quicksort Runtime as a Function of Array Size')
    plt.xlabel("Array Size")
    plt.ylabel("Runtime (seconds)")
    plt.grid(True)
    plt.show() # Will save graph as RuntimeGraph.png after viewing it
