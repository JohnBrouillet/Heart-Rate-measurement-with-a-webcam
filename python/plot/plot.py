import numpy as np
import matplotlib.pyplot as plt

def plot_fft(y, nfft):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    N = nfft/2
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


def plot_all(roi_face, post_nlms, post_nrme, detrended, avg, BPfilter, fft, nfft):
    plot_signal(roi_face, "Green signal from the face")
    plot_signal(post_nlms, "NLMS")
    plot_signal(post_nrme, "NRME")
    plot_signal(detrended, "Detrended signal")
    plot_signal(avg, "moving average filter")
    plot_signal(BPfilter, "Bandpass filter")
    plot_fft(fft, nfft)
    plt.show()
