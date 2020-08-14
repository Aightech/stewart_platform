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
            //m_motors[i]->set_velocity();
            m_motors[i]->set_position_offset(m_motor_position_offset[i]);
        }
        else
        {
            std::cout << "Driver n" << i << " unavailable..." << std::endl;
            m_available = false;
        }
        m_motor_pos[i] = m_motors[i]->get_angle();
        IF_VERBOSE(0,std::cout << "Driver n" << i << " position: " << m_motor_pos[i] << std::endl;, m_verbose_level);
        m_motors[i]->set_angle(0);
        //m_motors[i]->pause();
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
        m_motor_pos[i] = m_motors[m_motor_lookup[i]]->get_angle();
        if(fabs(m_motor_pos[i] + m_alpha[i]) > 0.0001) 
        	m_motors[m_motor_lookup[i]]->set_angle(0.3*m_motor_pos[i]-0.7*m_alpha[i]);
       
       //std::cout << m_motor_pos[i] << "\t";
    }
    //std::cout  << "\n";
};

void 
stp::Platform::sin_wave(Eigen::Vector3f dir, double w)
    {
    	double T_mid[3], theta_mid[3];
    	double T[3];
    	for(int i =0; i<3; i++)
    	{
    	 T_mid[i] = get_T(i);
    	 T[i] = T_mid[i];
    	 theta_mid[i] = get_theta(i);
    	}
    	auto prev_time = chr::high_resolution_clock::now();
    	for(;;)
    	{
    		for(int i =0; i<3; i++)
        		T[i] = T_mid[i] + dir[i]*sin(w*chr::duration<double>(chr::high_resolution_clock::now()-prev_time).count());

	    	stp::Model::new_pos(T, theta_mid);
	    	update_platform();
	}
    }
