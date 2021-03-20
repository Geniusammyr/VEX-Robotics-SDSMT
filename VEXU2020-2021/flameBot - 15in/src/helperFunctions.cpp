#include "../include/helperFunctions.h"

//take in a vecor of motors, and set their speed to a value
void setMotors(std::vector<pros::Motor> & motors, double speed)
{
  for(auto motor : motors)
  {
    motor = speed;
  }
}

//take in a vector of motors, and set their brake type to a given type
void setBrakes(std::vector<pros::Motor> & motors,  pros::motor_brake_mode_e_t brakeType)
{
  for(auto motor: motors)
  {
    motor.set_brake_mode(brakeType);
  }
}

//tqke in a vector of motors, and call the move relative function for all of them with a given distance and speed
void setMotorsRelative(std::vector<pros::Motor> & motors, double distance, double speed)
{
  for(auto motor : motors)
  {
    motor.move_relative(distance, speed);
  }
}

bool pressButton(bool press, bool &debounce)
{
	if(press)
    {
        if(!debounce)
        {
            debounce = true;
            return true;
        }
    }
    else
    {
        debounce = false;
    }

    return false;
}


void unfold()
{
  // We want to run the bottom rollor to detatch its rubber band mounting
  // Our intake is a ratchet, if we run it negative it will lock and move instead of skipping
  bottomDrum = 100;
  setMotors(intakeMotorVector, -100);
  pros::delay(800);
  bottomDrum = 0;
  setMotors(intakeMotorVector, 0);
}

void setIntakeContain()
{
  // Set everything to an intaking but not inserting speeds
  pooper = 0;
  topDrum = inserterRestingConst;

}

void setIntakePoop()
{
  // Set everything to an intaking but not inserting speeds
  pooper = pooperConst;
  topDrum = -inserterConst;
}

void setIntakeInsert()
{
  // Set everything to an intaking but not inserting speeds
  pooper = -pooperConst;
  topDrum = inserterConst;
}

bool isHoldingBall()
{
  return false;
}

Color getBallColor()
{
  int numObjects = visionSensor.get_object_count();
  pros::vision_object_s_t biggestObject;
  if(numObjects != 0)
  {
      biggestObject = visionSensor.get_by_size(0);
  }
  else
  {
    return NA;
  }

  if(biggestObject.signature == BLUE_BALL_SIG_INDEX)
  {
    return blue;
  }
  else if(biggestObject.signature == RED_BALL_SIG_INDEX)
  {
    return red;
  }
  else
  {
    return NA;
  }
}

void autoIntake()
{
  setMotors(intakeMotorVector, intakeConst);
  bottomDrum = intakeConst;
  setIntakeContain();
  float MAX_TIME = 3000;
  float currentTime = 0;
  bool holdingBall = false;

  while(!holdingBall && currentTime < MAX_TIME)
  {
    holdingBall = isHoldingBall();
    currentTime += loopDelay;
    pros::delay(loopDelay);
  }

  Color ballColor = getBallColor();
  pros::lcd::set_text(3, "Got Color: " + std::to_string(ballColor));
  if(ballColor != NA)
  {
    // Either shoot or poop
    if(ballColor == colorToPoop)
    {
      setIntakePoop();
      pros::delay(500);
    }
    else
    {
      setIntakeInsert();
      pros::delay(500);
    }
  }
  setIntakeContain();
}
