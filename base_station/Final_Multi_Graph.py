import serial
import multiprocessing
import matplotlib.pyplot as plt
import sys

def read_data(data_queue, port, baudrate):
    try:
        ser = serial.Serial(port, baudrate, timeout=1)
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                data = line.split(',')
                if len(data) == 3:
                    try:
                        x, y, z = [float(val) for val in data]
                        data_queue.put((x, y, z))
                    except ValueError:
                        pass
    except Exception as e:
        print(f"Error in subprocess: {e}")

def plot_data(data_queue):
    fig, (ax_x, ax_y, ax_z) = plt.subplots(3, 1)
    fig.tight_layout(pad=3.0)

    line_x, = ax_x.plot([], [], label='X')
    line_y, = ax_y.plot([], [], label='Y')
    line_z, = ax_z.plot([], [], label='Z')

    data_x, data_y, data_z, time_steps = [], [], [], []
    window_size = 50
    y_padding_factor = 0.1

    # Annotations for current values
    current_x_text = ax_x.text(1, 1.2, '', transform=ax_x.transAxes, ha='right', va='top')
    current_y_text = ax_y.text(1, 1.2, '', transform=ax_y.transAxes, ha='right', va='top')
    current_z_text = ax_z.text(1, 1.2, '', transform=ax_z.transAxes, ha='right', va='top')

    def update_axes(ax, data, time_steps, window_size):
        if len(time_steps) > window_size:
            min_time = time_steps[-window_size]
            max_time = time_steps[-1]
            ax.set_xlim(min_time, max_time)
            visible_indices = [i for i, t in enumerate(time_steps) if min_time <= t <= max_time]
            visible_data = [data[i] for i in visible_indices]
        else:
            ax.set_xlim(0, max(window_size, len(time_steps)))
            visible_data = data

        if visible_data:
            data_range = max(visible_data) - min(visible_data)
            if data_range == 0: data_range = 1
            padding = data_range * y_padding_factor
            ax.set_ylim(min(visible_data) - padding, max(visible_data) + padding)
        else:
            ax.set_ylim(-1, 1)

    def update_plot():
        while not data_queue.empty():
            x, y, z = data_queue.get()
            time_steps.append(len(time_steps))
            data_x.append(x)
            data_y.append(y)
            data_z.append(z)

            line_x.set_data(time_steps, data_x)
            line_y.set_data(time_steps, data_y)
            line_z.set_data(time_steps, data_z)

            # Update current value annotations
            if data_x:
                current_x_text.set_text(f'X: {data_x[-1]:.2f}')
            if data_y:
                current_y_text.set_text(f'Y: {data_y[-1]:.2f}')
            if data_z:
                current_z_text.set_text(f'Z: {data_z[-1]:.2f}')

            update_axes(ax_x, data_x, time_steps, window_size)
            update_axes(ax_y, data_y, time_steps, window_size)
            update_axes(ax_z, data_z, time_steps, window_size)

            fig.canvas.draw_idle()

    plt.ion()
    plt.show()

    while True:
        update_plot()
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
