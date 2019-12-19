#include "stewart_platform.hpp"
#include "joystick.h"
#define MAX_JOY 32767.


int main()
{
  cJoystick js;
  
  double deltas[4] = {0.63, 0.15, 0.40, 0.075};
  Stewart_platform sp(deltas,0.08,0.44);
  

  double T[3]={0,0,sp.get_T(2)};
  double theta[3]={0,0,0};
  double trans_max = 0.06;
  double ang_max = 0.06;
  while(1)
    {

      T[0]= js.joystickValue(0)/MAX_JOY*trans_max;
      T[1]= js.joystickValue(1)/MAX_JOY*trans_max;
      T[2]+= js.buttonPressed(4)*0.001 -js.buttonPressed(5)*0.001;

      theta[1]= js.joystickValue(2)/MAX_JOY*ang_max;
      theta[1]= js.joystickValue(3)/MAX_JOY*ang_max;
      
      std::cout << T[0] << std::endl;
      sp.new_pos(T, theta);
   
      sp.draw();
      usleep(10000);

	
      
    }
  
}
