#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pid.h"

void PIDImpl_init(PIDImpl *pid, double dt, double max, double min, double Kp, double Kd, double Ki) {
    pid->dt = dt;
    pid->max = max;
    pid->min = min;
    pid->Kp = Kp;
    pid->Kd = Kd;
    pid->Ki = Ki;
    pid->pre_error = 0;
    pid->integral = 0;
}

double PIDImpl_calculate(PIDImpl *pid, double setpoint, double pv) {
    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = pid->Kp * error;

    // Integral term
    pid->integral += error * pid->dt;
    double Iout = pid->Ki * pid->integral;

    // Derivative term
    double derivative = (error - pid->pre_error) / pid->dt;
    double Dout = pid->Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if (output > pid->max)
        output = pid->max;
    else if (output < pid->min)
        output = pid->min;

    // Save error to previous error
    pid->pre_error = error;

    return output;
}

void PID_init(PID *pid, double dt, double max, double min, double Kp, double Kd, double Ki) {
    pid->pimpl = (PIDImpl *)malloc(sizeof(PIDImpl));
    if (pid->pimpl == NULL) {
        fprintf(stderr, "Failed to allocate memory for PIDImpl\n");
        exit(1);
    }
    PIDImpl_init(pid->pimpl, dt, max, min, Kp, Kd, Ki);
}

double PID_calculate(PID *pid, double setpoint, double pv) {
    return PIDImpl_calculate(pid->pimpl, setpoint, pv);
}

void PID_destroy(PID *pid) {
    free(pid->pimpl);
}
