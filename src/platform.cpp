#include "platform.hpp"
#include "CANopen_lxm32.h"
#include <iostream>

void
stp::Platform::init()
{
    m_available = true;
    for(int i = 0; i < NB_LEGS; i++)
    {
        m_motors[i] = new CANopen::LXM32("can0", i + 1, m_verbose_level);
        if(m_motors[i]->is_available())
        {
            m_motors[i]->start();
            m_motors[i]->profilePosition_mode();
            m_motors[i]->set_position_offset(m_motor_position_offset[i]);
        }
        else
        {
            std::cout << "Driver n" << i << " unavailable..." << std::endl;
            m_available = false;
        }
        m_motors[i]->homing();
        m_motor_pos[i] = m_motors[i]->get_angle();
    }
}

void
stp::Platform::start()
{
    for(int i = 0; i < NB_LEGS; i++)
    {
        if(m_motors[i]->is_available())
        {
            m_motors[i]->start();
            m_motors[i]->profilePosition_mode();
        }
        
    }
}


void
stp::Platform::pause()
{
    for(int i = 0; i < NB_LEGS; i++)
        if(m_motors[i]->is_available())
            m_motors[i]->pause();
}

void
stp::Platform::stop()
{
    for(int i = 0; i < NB_LEGS; i++)
        if(m_motors[i]->is_available())
            m_motors[i]->pause();
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
        m_motor_pos[i] = m_motors[i_n]->get_angle();
        if(fabs(m_motor_pos[i] + m_alpha[i]) > 0.0001)
            m_motors[i_n]->set_angle(m_alpha[i]);
    }
}


