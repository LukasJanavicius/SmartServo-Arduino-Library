#ifndef SmartServo.h
#define SmartServo_h
#include <Servo.h>
#include <Arduino.h>

class SmartServo
{
  public:

  typedef enum
    {
      SWEEP,
      NUMOFSWEEPS,
      SINGLE
    }Modes; //DIFFEREnt modes
  typedef unsigned long clock_time_t;

  SmartServo(int servo_pin , float servo_speed = 0, uint8_t Mode = SmartServo::SWEEP , float number_of_sweeps = -1);
  void setSpeed(float forward_speed = 0 , float backward_speed = -1 );       //speed is in units steps/s;
  void setRange(int min_position = 0, int max_position = 180);
  void Attach();
  void Detach();
  void setMode();
  void write(int);
  void stop();
  void run();
  void moveTo(int target_position,float target_speed = -1); //moves to the position from our current position at a given speed.
  bool is_running();
  int currentPosition();
  int currentSpeed();
  clock_time_t clock_time();


private:

Servo servo;              // the servo
int _mode;
int _servo_pin;
int _range_low;
int _range_high;
int _current_position;
int _direction;
int _min_position;
int _max_position;
float _number_of_sweeps;
float _current_sweep;
float _step_interval;
float _forward_interval;
float _forward_speed;
float _backward_interval;
float _backward_speed;
clock_time_t _last_step_time;
clock_time_t _current_time;

};
#endif
