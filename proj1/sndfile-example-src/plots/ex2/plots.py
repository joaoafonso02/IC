import matplotlib.pyplot as plt

# Read data from text files
with open('mse.txt', 'r') as mse_file:
    mse_data = [float(line.strip()) for line in mse_file]

with open('max_absolute_error.txt', 'r') as mae_file:
    mae_data = [float(line.strip()) for line in mae_file]

with open('snr.txt', 'r') as snr_file:
    snr_data = [float(line.strip()) for line in snr_file]

# Create x-axis values (e.g., 16 - i for each data point)
x_values = [16 - i for i in range(16)]

# Create subplots
plt.figure(figsize=(12, 4))

# MSE plot
plt.subplot(131)
plt.plot(x_values, mse_data)
plt.xlabel('Quantization Level')
plt.ylabel('MSE')
plt.title('Mean Squared Error')

# Max Absolute Error plot
plt.subplot(132)
plt.plot(x_values, mae_data)
plt.xlabel('Quantization Level')
plt.ylabel('Max Absolute Error')
plt.title('Max Absolute Error')

# SNR plot
plt.subplot(133)
plt.plot(x_values, snr_data)
plt.xlabel('Quantization Level')
plt.ylabel('SNR (dB)')
plt.title('Signal-to-Noise Ratio')

plt.tight_layout()  # Ensures the plots do not overlap

# Save or display the plots
plt.savefig('graphs.png')  # Save the plots to a file
plt.show()  # Display the plots on screen
