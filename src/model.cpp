#include <cmath>
#include <iostream>
#include <vector>

#include "model.hpp"
#include <cstdlib>

stp::Model::Model(double deltas[4], double a, double l)
{
    //set the length of the arms and legs
    m_a = a;
    m_l = l;
    m_a2 = a * a;
    m_l2 = l * l;

    //compute the radius of the base and the platform from the delta lengths.
    for(int i = 0; i < 2; i++)
        m_radius[i] = deltas[2 * i + 1] /
                      (2 * sin(atan(deltas[2 * i + 1] /
                                    (2 * deltas[2 * i] + deltas[2 * i + 1]))));

    //Set the constant coordinate of B and P1 (P in B1).
    for(int i = 0; i < NB_LEGS; i++)
    {
        m_parity[i] = (i - i / 2 * 2);
        //angle of the arms on Oxy.
        m_beta[i] = i / 2 * M_PI * 2 / 3 - (2 * m_parity[i] - 1) * M_PI / 2;
        //angle of the motor on the base circle.
        m_gamma[0][i] =
            m_beta[i] + (2 * m_parity[i] - 1) * M_PI / 2 +
            (2 * m_parity[i] - 1) * asin(deltas[0] / 2 / m_radius[0]);
        m_gamma[1][i] =
            m_beta[i] + (2 * m_parity[i] - 1) * M_PI / 2 +
            (2 * m_parity[i] - 1) * asin(deltas[1] / 2 / m_radius[1]);

        m_B[i][0] = m_radius[0] * cos(m_gamma[0][i]);
        m_B[i][1] = m_radius[0] * sin(m_gamma[0][i]);
        m_B[i][2] = 0;

        m_P1[i][0] = m_radius[1] * cos(m_gamma[1][i]);
        m_P1[i][1] = m_radius[1] * sin(m_gamma[1][i]);
        m_P1[i][2] = 0;

        m_alpha_spd[i] = 0;
    }
}

void
stp::Model::init_pos()
{
    //compute h0
    double bp1_0 = m_B[0][0] - m_P1[0][0];
    double h0 = sqrt(m_l2 + m_a2 - bp1_0 * bp1_0 - bp1_0 * bp1_0);

    //set up the initial position.
    set_T(0, 0, h0);
    set_theta(0, 0, 0);

    set_T_spd(0, 0, 0);
    set_theta_spd(0, 0, 0);

    new_pos(m_T, m_theta);

    for(int i = 0; i < 6; i++) { _alphaI[i] = m_alpha[i]; }
}

double*
stp::Model::new_pos(double T[3], double theta[3])
{
  
    std::cout << "heya" << std::endl;
    ////////////
    double T_tmp[3];
    double theta_tmp[3];
    for(int j = 0; j < 3; j++)
    {
        T_tmp[j] = m_T[j];
        theta_tmp[j] = m_theta[j];
        m_T[j] = T[j];
    }
    compute_R(theta);

    ////////////////
    double tmp;
    double L;
    double M;
    double N;
    for(int i = 0; i < NB_LEGS; i++)
    {
        _d2[i] = 0;
        for(int j = 0; j < 3; j++)
        {
            m_P[i][j] = m_T[j];
            for(int k = 0; k < 3; k++) m_P[i][j] += m_R[j][k] * m_P1[i][k];
            tmp = m_P[i][j] - m_B[i][j];
            _d2[i] += tmp * tmp;
        }

        L = (m_a2 - m_l2 + _d2[i]) / (2 * m_a);
        M = cos(m_beta[i]) * (m_P[i][0] - m_B[i][0]) +
            sin(m_beta[i]) * (m_P[i][1] - m_B[i][1]);
        N = (m_P[i][2] - m_B[i][2]);

        double D = M * M + N * N;
        if(D < 0 || L / sqrt(D) >= 1 || L / sqrt(D) <= -1)
        {
            std::cout << "Limit reached..." << std::endl;
            new_pos(T_tmp, theta_tmp);
            return m_alpha;
        }
        m_alpha[i] = asin(L / sqrt(D)) - atan(M / N);
    }
    return m_alpha;
};

void
stp::Model::compute_R(double theta[3])
{

    for(int j = 0; j < 3; j++)
    {
        m_theta[j] = theta[j];
        m_trig[j * 2] = cos(m_theta[j]);
        m_trig[j * 2 + 1] = sin(m_theta[j]);
    }
    m_R[0][0] = m_trig[0] * m_trig[2];
    m_R[0][1] = -m_trig[1] * m_trig[4] + m_trig[0] * m_trig[3] * m_trig[5];
    m_R[0][2] = m_trig[1] * m_trig[5] + m_trig[0] * m_trig[3] * m_trig[4];
    m_R[1][0] = m_trig[1] * m_trig[2];
    m_R[1][1] = m_trig[0] * m_trig[4] + m_trig[1] * m_trig[3] * m_trig[5];
    m_R[1][2] = -m_trig[0] * m_trig[5] + m_trig[1] * m_trig[3] * m_trig[4];
    m_R[2][0] = -m_trig[3];
    m_R[2][1] = m_trig[2] * m_trig[5];
    m_R[2][2] = m_trig[2] * m_trig[4];
}

void
stp::Model::set_T(double t0, double t1, double t2)
{
    m_T[0] = t0;
    m_T[1] = t1;
    m_T[2] = t2;
}

void
stp::Model::set_T_spd(double t0, double t1, double t2)
{
    m_T_spd[0] = t0;
    m_T_spd[1] = t1;
    m_T_spd[2] = t2;
}

double
stp::Model::get_T(int i)
{
    return m_T[i];
};

double
stp::Model::get_theta(int i)
{
    return m_theta[i];
};

void
stp::Model::set_theta(double t0, double t1, double t2)
{
    m_theta[0] = t0;
    m_theta[1] = t1;
    m_theta[2] = t2;
}
void
stp::Model::set_theta_spd(double t0, double t1, double t2)
{
    m_theta_spd[0] = t0;
    m_theta_spd[1] = t1;
    m_theta_spd[2] = t2;
}
