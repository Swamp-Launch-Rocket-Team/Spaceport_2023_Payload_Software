#ifndef _PID_H_
#define _PID_H_

typedef struct PIDImpl PIDImpl;
typedef struct PID PID;

PID *PID_create(double dt, double max, double min, double Kp, double Kd, double Ki);
void PID_destroy(PID *pid);
double PID_calculate(PID *pid, double setpoint, double pv);

#endif

