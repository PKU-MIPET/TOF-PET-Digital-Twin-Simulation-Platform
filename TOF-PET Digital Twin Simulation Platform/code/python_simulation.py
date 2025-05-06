"""
Signal Simulation Pipeline for Radiation Detector Waveform Generation
Author: [Yuyao Wang], Version: 1.0 (2025-05-10)
"""
# Import required libraries
import os
import random
import numpy as np
from scipy.interpolate import CubicSpline
from matplotlib import pyplot as plt
import scipy.signal as signal

def generate_random_number(mean, std_dex):
    """Generate a random number from Gaussian distribution"""
    return random.gauss(mean, std_dex)

def txt_Read(Path):
    """Read data from text file and extract the 4th column"""
    with open(Path, 'r') as file:
        lines = file.readlines()

        x = []
        for line in lines:
            line = line.strip()  # Remove trailing newline character
            line_tmp = line.split()
            if len(line_tmp) == 0:
                continue  # Skip empty lines
            else:
                x.append(eval(line_tmp[3]))  # Get 4th column value

    return x

def chebyshev_distance(point1, point2):
    """Calculate Chebyshev distance between two points"""
    x1, y1 = point1
    x2, y2 = point2

    distance = max(abs(x2 - x1), abs(y2 - y1))

    return distance

def DataLoad(dataP):
    """Load data from .npy file and transpose it"""
    data_ori = np.load(dataP)

    data_return = []
    [x, y] = data_ori.shape
    for i in range(y):
        temp = []
        for j in range(x):
            temp.append(data_ori[j][i])
        data_return.append(temp)

    return data_return

def smoothFunc(x, y):
    """Apply cubic spline interpolation for smoothing"""
    cs = CubicSpline(x, y)

    # Generate new x values for plotting
    x_new = np.linspace(x[0], x[-1], 400)
    y_new = cs(x_new)

    return x_new, y_new

def save_to_txt(folder_path, filecount, x, y):
    """Save x and y data to text file"""
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)

    new_file_name = filecount
    new_file_path = os.path.join(folder_path, new_file_name)

    # Write data to file
    with open(new_file_path, 'w') as file:
        for xi, yi in zip(x, y):
            file.write(f"{xi} {yi}\n")

    print(f"Data saved to {new_file_path}")
    return 0

##### Time delay module #####
def time_delayFunc():
    """Generate random time delay with normal distribution"""
    std = 210
    sum_delay = np.random.normal(5000, std)

    return sum_delay

def TTSsimulation():
    """Simulate Transit Time Spread using exponential distribution"""
    # Custom rate parameter λ
    lambda_param = 0.023647573654716342  
    return np.random.exponential(1 / lambda_param) * 0.001

def TimeWalk(time, signal):
    """Simulate time walk effect on signal"""
    cs = CubicSpline(time, signal, bc_type='natural')
    time = np.linspace(min(time), max(time), len(time) * 10)
    signal = cs(time)
    signal = signal.tolist()

    # Adjust signal baseline
    for _ in range(len(signal)):
        signal[_] = signal[_] - 0.005

    # Find signal peak
    argmax = signal.index(max(signal))

    # Calculate interpolation value
    inter = signal[argmax] - signal[argmax + 1]

    # Apply correction
    for _ in range(argmax + 1):
        signal[_] -= inter

    # Remove negative values before peak
    for _ in range(argmax):
        if signal[_] < 0:
            signal[_] = 0

    # Limit minimum value
    for _ in range(len(signal)):
        if signal[_] < -0.05:
            signal[_] = -0.05

    return time, signal

def dark_count(time):
    """Simulate dark counts with 50kHz/mm2 rate"""
    drc = generate_random_number(50, 3)
    darkCountRate = drc * 9 * 1000
    darkCountTime = []
    for i in range(len(time)):
        if random.random() < 1 / darkCountRate:
            darkCountTime.append(time[i])
    return darkCountTime

def CrossTALK(time):
    """Simulate cross-talk effect (8% of real signal)"""
    cross_talk_rate = generate_random_number(0.08, 0.005)
    if cross_talk_rate <= 0:
        return time
    temp = random.sample(time, int(len(time) * cross_talk_rate))
    return time + temp

def gauss_noise(t):
    """Add Gaussian white noise to signal"""
    noise_power = 0.000005
    noise = np.sqrt(noise_power / 2) * (np.random.randn(t.shape[0]) + 1j * np.random.randn(t.shape[0]))
    noise = noise.real  # Take only real part as Gaussian white noise
    return noise

def AM(x):
    """Generate amplitude modulated signal"""
    # Parameters
    T = 0.1  # Total time (seconds)
    t = np.linspace(T / 2, T, len(x), endpoint=False)  # Time array

    # Carrier signal parameters
    fc = 75  # Carrier frequency (Hz)
    Ac = 1   # Carrier amplitude

    # Modulation signal parameters
    fm = 5   # Modulation frequency (Hz)
    Am = 10  # Modulation amplitude

    # Generate carrier and modulation signals
    carrier_signal = Ac * np.sin(2 * np.pi * fc * t)
    modulating_signal = Am * np.sin(2 * np.pi * fm * t)

    # Generate AM signal
    am_signal = (Ac + modulating_signal) * np.sin(2 * np.pi * fc * t) * 0.08

    return am_signal

def IIR(signal_in):
    """Apply IIR low-pass filter to signal"""
    f_low = 100   # Low cutoff frequency (Hz)
    f_high = 4e9  # High cutoff frequency (Hz)

    # Design bandpass filter
    nyquist = 10e9  # Nyquist frequency
    f_band = [f_low / nyquist, f_high / nyquist]  # Normalized frequency
    b, a = signal.butter(N=20, Wn=f_band[1], btype='low')  # Butterworth filter

    # Apply filter
    signal_out = signal.lfilter(b, a, signal_in)
    return signal_out

def generate_output_signal(time):
    """Generate complete output signal (time in ns)"""
    def generate_signal(rise_time, fall_time, total_time, sampling_rate):
        """Generate single pulse signal with given parameters"""
        t = np.arange(0, total_time, 1/sampling_rate)
        a = 6 * 1e-4
        tau_rise = rise_time / np.log(9)  
        tau_fall = fall_time / np.log(9)  
        
        # Generate rising edge
        rise_signal = (1 - np.exp(-t / tau_rise)) * a
        
        # Generate falling edge
        fall_signal = (np.exp(-(t - rise_time) / tau_fall)) * a
        
        # Combine rising and falling edges
        signal = np.where(t < rise_time, rise_signal, fall_signal)
        
        return t, signal

    # Initialize output array
    Y = [0] * int((max(time)) * 1000 + 50000)
    x = np.linspace(0, (len(Y) - 1) / 1000, len(Y))
    time = time + dark_count(x[0: len(x) - 50000])

    # Generate signal for each time point
    for i in range(len(time)):
        rise_time = 0.4
        fall_time = 3
        total_time = 50  # Total time length
        sampling_rate = 1000  # Sampling rate

        # Generate signal
        t, y = generate_signal(rise_time, fall_time, total_time, sampling_rate)

        # Add to output
        for j in range(len(y)):
            Y[int(time[i] * 1000 + j)] = Y[int(time[i] * 1000 + j)] + y[j] - y[-1]

    # Downsample
    Y = [Y[i] for i in range(0, len(Y), 25)]
    x = [x[i] for i in range(0, len(x), 25)]

    # Post-processing
    temp = find_index(Y, 0)
    for i in range(temp):
        Y[i] = 0
    temp = find_index_min(Y, -0.05)
    for i in range(temp - 1, len(Y)):
        Y[i] = -0.05
    
    # Add noise and filter
    white_noise = gauss_noise(np.array(x))
    for i in range(len(Y)):
        Y[i] = Y[i] + white_noise[i]
    Y = IIR(Y)
    return x, Y  

def find_index(lst, x):
    """Find first index where list value > x"""
    index_lst = [i for i in range(len(lst)) if lst[i] > x]
    if index_lst:
        return index_lst[0]
    else:
        return -1

def find_index_min(lst, x):
    """Find first index where list value < x"""
    index_lst = [i for i in range(len(lst)) if lst[i] < x]
    if index_lst:
        return index_lst[0]
    else:
        return len(lst)

# Global parameter (unused in current code)
count_P = 100

if __name__ == '__main__':
    # File paths
    readPath = "C:\\Users\\wangy\\Desktop\\Simulation\\results\\Geant4_output\\1.txt"
    SavePath = "C:\\Users\\wangy\\Desktop\\Simulation\\results\\Python_output\\"
    name = "1.txt"

    # Read and process time data
    Time = txt_Read(readPath)
    Time = CrossTALK(Time)
    
    # Apply time delay
    for i in range(len(Time)):  # Convert time to ns
        Time[i] = time_delayFunc() * 0.001 + Time[i] 
    
    # Generate and save output signal
    x, Y = generate_output_signal(Time)
    plt.plot(x, Y)
    plt.savefig(SavePath+"1.png")
    save_to_txt(SavePath, name, x, Y)

    print("All processes completed.")
