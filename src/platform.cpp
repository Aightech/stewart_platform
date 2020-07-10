#include "platform.hpp"
#include "CANopen_lxm32.h"
#include <iostream>
void
stp::Platform::init_drivers()
{
    m_available = true;
    for(int i = 0; i < NB_LEGS; i++)
    {

        m_motors[i] = new CANopen::LXM32("can0", i + 1, false);
        if(m_motors[i]->is_available())
        {
            //TODO: initialize the drivers
            //m_motors[i]->init();
            //  m_motors[i]->start(MODE_ProfilePosition, PPctrl_RELATIVE | PPctrl_ON_DIRECT);
        }
        else
        {
            std::cout << "Driver n" << i << " unavailable..." << std::endl;
            m_available = false;
        }

        m_motor_pos[i] = 0;
        m_inc[i] = 0;
    }
}

double *
stp::Platform::new_pos(double T[3], double theta[3])
{
    stp::Model::new_pos(T, theta);
    update_platform();
    return m_alpha;
}

void
stp::Platform::update_platform()
{
    for(int i = 0; i < NB_LEGS; i++)
    {
        int i_n = m_motor_lookup[i];
        m_inc[i_n] = -m_alpha[i] * m_conv - m_motor_pos[i_n];
        m_motor_pos[i_n] += m_inc[i_n];
    }

    for(int i = 0; i < NB_LEGS; i++)
    {
        if(m_verbose)
            std::cout << m_inc[i] << std::endl;
        m_motors[i]->set_position(m_inc[i]);
    }
};
