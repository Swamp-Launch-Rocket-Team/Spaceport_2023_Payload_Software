#include <stdio.h>

// Angular PID Controller

// Angular Position PID constants
#define Kp 1.0
#define Ki 0.1
#define Kd 0.01

// Function to calculate the speed based on the current position and setpoint
float calculateSpeed(float position, float setpoint, float *prev_error, float *integral) {
    float error = setpoint - position;
    *integral += error;
    float derivative = error - *prev_error;
    *prev_error = error;
    return (Kp * error) + (Ki * (*integral)) + (Kd * derivative);
}

int main() {
    float position = 0.0;  // Current angular position
    float setpoint = 90.0; // Desired angular position
    float prev_error = 0.0;
    float integral = 0.0;
    float measured_speed = 50.0;  // Example value for measured speed
    float rollspeed = 60.0;  // Example value for roll speed

    // Checks if payload is rolling or not
    if (measured_speed < rollspeed) {
        // Calculate the speed based on the current position and setpoint
        float speed = calculateSpeed(position, setpoint, &prev_error, &integral);
        // Output the speed
        printf("Speed: %.2f\n", speed);
    }

    return 0;
}

