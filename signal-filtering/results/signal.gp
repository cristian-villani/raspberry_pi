# Set the terminal to PNG (if you want an image file)
set terminal png
set output 'sh10kHz.png'

# Set title and labels
set title "Input Signal and ADC Output Signal"
set xlabel "Time (ms)"
set ylabel "Amplitude"

# Set time scale for x-axis (milliseconds)
set xrange [0:1000]  # Adjust based on your data's range
set yrange [0:1250]  # ADC range (0-1023)

# Define input square wave: frequency = 10 Hz -> period = 100 ms
# Use an integer division approach to alternate between high and low
f_input(t) = (int(t/50) % 2 == 0) ? 1023 : 0  # 10 Hz square wave with 5ms sampling (values 1023 and 0)

# Plot both the input signal and ADC values
plot \
    f_input(x) with lines title "Input Signal (Square Wave)", \
    "sh10kHz.txt" using 1:2 with lines title "ADC Output"
