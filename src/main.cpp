#include "joystick.h"
#include "stewart_platform.hpp"
#define MAX_JOY 32767.
#define ANG_MAX 0.06
#define TRANS_MAX 0.06

int
main()
{
    cJoystick js;

    double deltas[4] = {0.63, 0.14, 0.40, 0.075};
    Stewart_platform sp(deltas, 0.08, 0.44);

    double T[3] = {0, 0, sp.get_T(2)};
    double theta[3] = {0, 0, 0};

    double T_p[3] = {0, 0, 0.1};
    double theta_p[3] = {0, 0, 0};

    // sp.new_speed(T_p, theta_p );

    int i = 0;
    while(1)
        {
            std::cout << i++ << std::endl;
            T[0] = js.joystickValue(0) / MAX_JOY * TRANS_MAX;
            T[1] = js.joystickValue(1) / MAX_JOY * TRANS_MAX;
            T[2] += js.buttonPressed(4) * 0.001 - js.buttonPressed(5) * 0.001;

            theta[1] = js.joystickValue(2) / MAX_JOY * ANG_MAX;
            theta[1] = js.joystickValue(3) / MAX_JOY * ANG_MAX;

            std::cout << i << std::endl;
            sp.new_pos(T, theta);

            // sp.update(0.001);
            // sp.draw();
            usleep(10000);
        }
}
