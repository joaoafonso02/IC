import matplotlib.pyplot as plt
import numpy as np

data = [0,0]
xlabel_name = ['dct frac', 'mantissa size']
# DCT Fraq data
data[0] = [
    [0.1, 1831260, 15375, 9.25],
    [0.2, 358191, 11198, 16.34],
    [0.3, 101633, 11326, 21.81],
    [0.4, 48022, 11568, 25],
    [0.5, 25985, 11625, 27.73],
    [0.6, 16149, 11800, 29.8],
    [0.7, 8427, 11833, 32.6],
    [0.8, 3730, 11814, 36.16],
    [0.9, 2153, 11822, 38.55],
    [1, 2105, 11833, 38.64]
]

data[1] = [
    [1, 513606, 11489, 14.77],
    [2, 152015, 11603, 20.06],
    [3, 42626, 11656, 25.58],
    [4, 11722, 11699, 30.83],
    [5, 4798, 11784, 35.07],
    [6, 2791, 11813, 37.42],
    [7, 2275, 11825, 38.3],
    [8, 2146, 11830, 38.56],
    [9, 2114, 11832, 38.63],
    [10, 2105, 11833, 38.65]
]

for k, i in enumerate(data):
    x_values = np.arange(i[0][0], i[-1][0]+i[1][0]-i[0][0], i[1][0]-i[0][0])

    plt.figure(figsize=(12,4))

    plt.subplot(131)
    plt.plot(x_values, [j[1] for j in i])
    plt.xlabel(xlabel_name[k], fontsize=16)
    plt.ylabel('MSE', fontsize=16)
    plt.title('Mean Squared Error', fontsize=18)

    plt.subplot(132)
    plt.plot(x_values, [j[2] for j in i])
    plt.xlabel(xlabel_name[k], fontsize=16)
    plt.ylabel('max absolute error', fontsize=16)
    plt.title('Max Absolute Error', fontsize=18)

    plt.subplot(133)
    plt.plot(x_values, [j[3] for j in i])
    plt.xlabel(xlabel_name[k], fontsize=16)
    plt.ylabel('SNR (db)', fontsize=16)
    plt.title('Signal to Noise Ratio', fontsize=18)

    plt.tight_layout()

    plt.savefig(xlabel_name[k])
    plt.clf()



