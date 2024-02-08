# Import necessary libraries
import serial
import multiprocessing
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import sys

# Function to read data from the serial port
def read_data(data_queue, port, baudrate):
    try:
        # Initialize serial port
        ser = serial.Serial(port, baudrate, timeout=1)
        while True:
            # Check if data is available
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                data = line.split(',')
                if len(data) == 3:
                    try:
                        # Parse the x, y, and z values
                        x, y, z = [float(val) for val in data]
                        # Put the data into the queue for the main process
                        data_queue.put((x, y, z))
                        # print(f"Subprocess - Sending data: x={x}, y={y}, z={z}")
                    except ValueError:
                        pass
    except Exception as e:
        print(f"Error in subprocess: {e}")

# Function to plot data
def plot_data(data_queue):
    fig, ax = plt.subplots()
    # Normalize the color range (adjust vmin and vmax based on expected z values)
    norm = mcolors.Normalize(vmin=-2, vmax=2)
    # Set the colormap
    cmap = plt.cm.viridis
    # Initialize scatter plot with empty data and set the colormap and normalization
    sc = ax.scatter([], [], c=[], norm=norm, cmap=cmap)
    # Set axis limits
    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    # Add a colorbar to show the mapping of z values to colors
    plt.colorbar(sc, ax=ax)

    # Function to update the plot with new data
    def update_plot():
        while not data_queue.empty():
            # Get data from the queue
            x, y, z = data_queue.get()
            # print(f"Main process - Received data for plotting: x={x}, y={y}, z={z}")
            # Update the scatter plot with the new data point
            sc.set_offsets([(x, y)])
            # Update the color of the point
            sc.set_array([z])
            # Redraw the plot
            fig.canvas.draw_idle()

    # Turn on interactive mode for dynamic plot updates
    plt.ion()
    # Display the plot
    plt.show()

    # Continuous loop to keep updating the plot
    while True:
        update_plot()
        # Short pause to allow GUI to update
        plt.pause(0.1)

if __name__ == '__main__':
    if len(sys.argv) > 1:
        com_port_number = sys.argv[1]  # Step 2: Retrieve the COM port number from command-line argument
        com_port = f'COM{com_port_number}'  # Construct the COM port string
    else:
        print("No COM port number provided. Please provide as an agument.\nExiting...")
        sys.exit(1)

    data_queue = multiprocessing.Queue()
    data_process = multiprocessing.Process(target=read_data, args=(data_queue, com_port, 9600))
    data_process.start()

    plot_data(data_queue)
    data_process.join()
