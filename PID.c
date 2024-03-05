#include <stdio.h>

// Angular Position PID Controller

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

// Angular Speed PID constants
#define Kp_speed 1.0
#define Ki_speed 0.1
#define Kd_speed 0.01

// Function to calculate the final speed of reaction wheel after iteration
float calculateSpeedFinal(float measured_speed, float speed, float *prev_error_speed, float *integral_speed) {
    float error_speed = speed - measured_speed;  // Speed is 0 for controlling roll
    *integral_speed += error_speed;
    float derivative_speed = error_speed - *prev_error_speed;
    *prev_error_speed = error_speed;
    return (Kp_speed * error_speed) + (Ki_speed * (*integral_speed)) + (Kd_speed * derivative_speed);
}

int main() {
    float position = 0.0;  // Current angular position
    float setpoint = 0.0; // Desired angular position
    float prev_error = 0.0;
    float integral = 0.0;
    float measured_speed = 0; 
    float rollspeed = 0;  // At what speed do we need to stop rolling
    float speed = 0;
    
    // Checks if payload is rolling or not
    if (measured_speed < rollspeed) {
        // Calculate speed based on the current position and setpoint
        speed = calculateSpeed(position, setpoint, &prev_error, &integral);
    } else {
        // If payload is rolling, set speed to 0
        speed = 0.0;
    }
    
    // Angular Speed PID Controller
    float prev_error_speed = 0.0;
    float integral_speed = 0.0;
    float speedfinal = 0.0;

    speedfinal = calculateSpeedFinal(measured_speed, speed, &prev_error_speed, &integral_speed)*5.76;
    // Multiply by 5.76, the ratio of the inertias

    return 0;
}
