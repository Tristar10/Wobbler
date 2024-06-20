import numpy as np
import matplotlib.pyplot as plt

N = 1024


def mu45_lfo(attack_param, speed_param):
    table = np.zeros(N)
    if speed_param == 100:
        attack_param += 60
    offset = 0 if ((attack_param - 50) / 62.5) < 0 else ((attack_param - 50) / 62.5) * N / 2
    for i in range(int(offset), N):
        if int(offset) < i <= int(N / 2 + ((attack_param - 50) / 62.5) * N / 2):
            table[i] = np.power((i - ((attack_param - 50) / 62.5) * N / 2) / (N / 2), (100 - speed_param) / 10.0)
        elif i > int(N / 2 + ((attack_param - 50) / 62.5) * N / 2):
            table[i] = 1
        else:
            table[i] = 0

    return table


attack_param = 40
speed_param = 100
wavetable = mu45_lfo(attack_param, speed_param)

# Plotting the wavetable
plt.plot(wavetable)
plt.title('Mu45 LFO Wavetable')
plt.xlabel('Sample Index')
plt.ylabel('Amplitude')
plt.show()
