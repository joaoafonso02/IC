import sys
import matplotlib.pyplot as plt

def plot_histogram(data):
    # Split the data into x and y values
    x_values, y_values, z_values= zip(*[map(float, line.split()) for line in data])

    # Plot the histogram
    plt.plot(x_values, y_values)
    plt.plot(x_values, z_values)

    # Set labels and title
    plt.xlabel('Quantization')
    plt.ylabel('Bit Rate (kbps) & Golomb M')
    plt.title('(Red) Bit Rate    (Blue) M')

    # Show the plot
    plt.show()

def read_data_from_file(filename):
    try:
        with open(filename, 'r') as file:
            data = file.readlines()
        return data
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
        sys.exit(1)

if __name__ == "__main__":
    # Check if a filename is provided as a command-line argument
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    input_data = read_data_from_file(filename)
    plot_histogram(input_data)
