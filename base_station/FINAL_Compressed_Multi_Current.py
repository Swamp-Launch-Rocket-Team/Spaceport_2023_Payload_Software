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
    fig, axs = plt.subplots(3, 2, figsize=(12, 8))
    fig.tight_layout(pad=3.0)

    line_x_moving, = axs[0, 0].plot([], [], label='X Moving')
    line_y_moving, = axs[1, 0].plot([], [], label='Y Moving')
    line_z_moving, = axs[2, 0].plot([], [], label='Z Moving')

    line_x_fixed, = axs[0, 1].plot([], [], label='X Fixed')
    line_y_fixed, = axs[1, 1].plot([], [], label='Y Fixed')
    line_z_fixed, = axs[2, 1].plot([], [], label='Z Fixed')

    data_x, data_y, data_z, time_steps = [], [], [], []
    window_size = 50
    y_padding_factor = 0.1

    # Annotations for current values on the moving graphs
    current_x_text = axs[0, 0].text(1, 1.1, '', transform=axs[0, 0].transAxes, ha='right', va='top')
    current_y_text = axs[1, 0].text(1, 1.1, '', transform=axs[1, 0].transAxes, ha='right', va='top')
    current_z_text = axs[2, 0].text(1, 1.1, '', transform=axs[2, 0].transAxes, ha='right', va='top')

    def update_axes(ax, data, time_steps, window_size, is_fixed_x_min):
        if len(time_steps) > window_size:
            min_time = 0 if is_fixed_x_min else time_steps[-window_size]
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

            line_x_moving.set_data(time_steps, data_x)
            line_y_moving.set_data(time_steps, data_y)
            line_z_moving.set_data(time_steps, data_z)

            line_x_fixed.set_data(time_steps, data_x)
            line_y_fixed.set_data(time_steps, data_y)
            line_z_fixed.set_data(time_steps, data_z)

            # Update current value annotations for moving graphs
            if data_x:
                current_x_text.set_text(f'X: {data_x[-1]:.2f}')
            if data_y:
                current_y_text.set_text(f'Y: {data_y[-1]:.2f}')
            if data_z:
                current_z_text.set_text(f'Z: {data_z[-1]:.2f}')

            update_axes(axs[0, 0], data_x, time_steps, window_size, False)
            update_axes(axs[1, 0], data_y, time_steps, window_size, False)
            update_axes(axs[2, 0], data_z, time_steps, window_size, False)
            update_axes(axs[0, 1], data_x, time_steps, window_size, True)
            update_axes(axs[1, 1], data_y, time_steps, window_size, True)
            update_axes(axs[2, 1], data_z, time_steps, window_size, True)

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
