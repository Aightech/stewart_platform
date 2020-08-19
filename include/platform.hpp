#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "CANopen_lxm32.h"
#include "model.hpp"
#include <eigen3/Eigen/Dense>
#include <chrono>

namespace chr = std::chrono;

namespace stp
{
class Platform : public Model
{
    public:
    Platform(double deltas[4], double a, double l, int verbose_level=0) : Model(deltas, a, l, verbose_level){};

    void
    init();
    void
    start();
    void
    pause();
    void
    stop();

    double *
    new_pos(double T[3], double theta[3]);
    
    void
    update_platform();

    protected:
    CANopen::LXM32 *m_motors[NB_LEGS]; /*!< Driver of the platform.*/
private:
    bool m_available;
    double m_motor_pos[NB_LEGS];

    int m_motor_lookup[NB_LEGS] = {5, 2, 0, 1, 4, 3};
    int32_t m_motor_position_offset[NB_LEGS] = { 94500+2*737280, 202100+2*737280, 202500+2*737280, 694800+2*737280, 203500+2*737280, 236000+2*737280};//1:207900
    
    double v=0;
    double t=0;
    
};

}; // namespace stp

#endif
