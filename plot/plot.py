import numpy as np
import matplotlib.pyplot as plt

def plot_fft(y):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    N = 512
    # sample spacing
    T = 1.0 / 30.0
    x = np.linspace(0.0, N*T, N)
    ax.plot(x, y)
    ax.grid(True)


def plot_signal(y, title):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    x = range(0,len(y))
    ax.plot(x,y)
    ax.set_title(title)
    ax.grid(True)


def plot_all(x, signal, filtered, nrme, avg, fft):
    plot_signal(x, "moyenner")
    plot_signal(signal, "nlms")
    plot_signal(filtered, "passe bande")
    plot_signal(nrme, "nrme")
    plot_signal(avg, "moving avg")
    plot_fft(fft)
    plt.show()