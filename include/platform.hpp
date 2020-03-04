#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "Lexium32A_canopen.h"
#include "model.hpp"

namespace stp
{

class Platform : public Model
{
    public:
    Platform(double deltas[4], double a, double l) : Model(deltas, a, l){};

    void
    init_drivers();

    double *
    new_pos(double T[3], double theta[3]);

    void
    update_platform();

    private:
    LXM32 *m_motors[NB_LEGS]; /*!< Driver of the platform.*/
    bool m_available;
    int32_t m_motor_pos[NB_LEGS];
    int32_t m_inc[NB_LEGS];
    double m_conv = 50000.0f / 24.0f * 180.0f / M_PI;

    int m_motor_lookup[NB_LEGS] = {5, 2, 0, 1, 4, 3};
};

}; // namespace stp

#endif
