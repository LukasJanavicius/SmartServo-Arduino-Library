
#include "SmartServo.h"


//creates an instance of the servo class for you servo motor
//The pin you assign to the servo pin MUST be a PWM pin on your board.
//if you do not pass a speed to this function then you must
SmartServo::SmartServo(int servo_pin, float servo_speed, uint8_t mode, float number_of_sweeps){
  _mode=mode;
  _servo_pin = servo_pin;
  _forward_speed = 0;
  setSpeed(servo_speed,-1);
  _current_position = 0;
  _last_step_time = millis();
  _current_time = _last_step_time;
  _direction = 1;
  _current_sweep = 0;
  _min_position = 0;
  _max_position = 180;

  if(mode == SmartServo::SWEEP){  //have to put this so someone doesn't screw up
    _number_of_sweeps = -1;
  }
  else if(mode == SmartServo::NUMOFSWEEPS){
    _number_of_sweeps = number_of_sweeps; //perform one sweep
  }
  else{
    _number_of_sweeps = 1;
    _number_of_sweeps = number_of_sweeps;
  }

}

//Attaches the servo to the pin
void SmartServo::Attach()
{
    servo.attach(_servo_pin);
//moves the servo to 0 once it is attatched, or keeps in the same position if it is being reattatched.
    servo.write(_current_position);
}

//Detatches the servo from the pin
void SmartServo::Detach()
{
    servo.detach();
}

void SmartServo::write( int position){
  servo.write(position);
}

int SmartServo::currentPosition(){
  return _current_position;
}
int SmartServo::currentSpeed(){
  return  _forward_speed;
}
SmartServo::clock_time_t SmartServo::clock_time()
{
 return millis();
}

bool SmartServo::is_running(){
  if(_step_interval!=0) return true;
  return false;
}
void SmartServo::stop(){
  setSpeed(0);
}

void SmartServo::setRange(int min_position, int max_position){
  _min_position = min_position;
  _max_position =  max_position;
  _current_position = _min_position;
  servo.write(_current_position);
}
void SmartServo::moveTo(int target_position, float target_speed){
  int temp_number_of_sweeps = _number_of_sweeps;
  int temp_current_sweep = _current_sweep;
  int temp_direction = _direction;
  int temp_forward_speed = _forward_speed;
  int temp_backward_speed = _backward_speed;
  if(target_speed>0){
    setSpeed(target_speed,-1);
  }
  _current_sweep = 0;
  _number_of_sweeps = 0.5; //since we don't bounce back perform half a sweep.
  if(target_position - _current_position >0){ //if we need to move forward to get there
    _direction = 1;
    _last_step_time = millis();
    _current_time = millis();
    while((target_position - _current_position)>0){
      run();
    }
  }
  else{
    _direction = -1;
    _last_step_time = millis();
    while((_current_position-target_position)>0){
      run();
    }
  }
  _current_sweep = temp_current_sweep;
  _direction = temp_direction;
  _number_of_sweeps = temp_number_of_sweeps;
  _forward_speed = temp_forward_speed;
  _backward_speed = temp_backward_speed;
  setSpeed(_forward_speed,_backward_speed);
}

//sets the speed of the servo, and calculates step spacing
void SmartServo::setSpeed(float forward_speed, float backward_speed){
  if(backward_speed == -1){
    //check to see if anything needs to be done
    if(forward_speed == _forward_speed) return;
    if(forward_speed == 0.0){ //avoid any divide by zero cases
      _forward_interval = 0;
      _backward_interval = 0;
      return;
    }
    _forward_speed = forward_speed;
    _backward_speed = _forward_speed;
    _forward_interval = 1000/forward_speed;
    _backward_interval = _forward_interval;
  }
  else // if someone has entered custom speeds
  {
    //check to see if anything needs to be done
    if(forward_speed == _forward_speed && backward_speed == _backward_speed) return;
    if(forward_speed == 0.0 && backward_speed == 0.0){ //avoid any divide by zero cases
      _forward_interval = 0;
      _backward_interval = 0;
      return;
    }
    //set the intervals
    _forward_speed = forward_speed;
    _backward_speed = backward_speed;
    _forward_interval = 1000/_forward_speed;
    _backward_interval = 1000/_backward_speed;
  }

}

void SmartServo::run(){
  if(_forward_interval>0){ //check to see if running
    if(_direction>0){
      _step_interval = _forward_interval;
    }
    else{
      _step_interval = _backward_interval;
    }
    _current_time = clock_time();
    if((_current_time - _last_step_time) > _step_interval && _number_of_sweeps != _current_sweep)
    {
      _last_step_time = _current_time;
      _current_position+=_direction;
      servo.write(_current_position);
      if ((_current_position >= _max_position) || (_current_position <= _min_position)) // end of sweep
        {
          _direction = -_direction;
          _current_sweep+=0.5; //increments in 0.5, because one full sweep is
        }
    }
  }
}
