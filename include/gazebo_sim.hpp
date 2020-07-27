#ifndef _GAZEBO_SIM_H_
#define _GAZEBO_SIM_H_

#include <gazebo/gazebo_client.hh>
#include <gazebo/gazebo_config.h>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include "gnuplot-iostream.h"
#include "model.hpp"

namespace stp
{

class Gazebo_sim : public Model
{
    public:
    Gazebo_sim(double deltas[4], double a, double l)
        : Model(deltas, a, l), m_node(new gazebo::transport::Node())
    {
        gazebo::client::setup();
        m_node->Init();

        // Publish to the  velodyne topic
        m_pub = m_node->Advertise<gazebo::msgs::Vector3d>("~/stewart_platform/cmd_posstewart_platform::motor_0_joint");

	std::cout << "Subscriber connecting to ~/stewart_platform/cmd_posstewart_platform::motor_0_joint" << std::endl;
        // Wait for a subscriber to connect to this publisher
        m_pub->WaitForConnection();

    };

  ~Gazebo_sim()
    {
        // Make sure to shut everything down.
        gazebo::client::shutdown();
    };

    double *
    new_pos(double T[3], double theta[3])
    {

        Model::new_pos(T, theta);
        update();

	 // Create a a vector3 message
        gazebo::msgs::Vector3d msg;

        // Set the velocity in the x-component

        gazebo::msgs::Set(&msg, 
                          ignition::math::Vector3d(T[0]*10, 0, 0));
        // Send the message
       m_pub->Publish(msg);
        return m_alpha;
    };

    void
    update();

    gazebo::transport::NodePtr m_node;
    gazebo::transport::PublisherPtr m_pub;
};

} // namespace stp

#endif
