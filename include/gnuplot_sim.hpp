#ifndef _GNUPLOT_SIM_H_
#define _GNUPLOT_SIM_H_
#define GNUPLOT_ENABLE_PTY
#include "gnuplot-iostream.h"
#include "model.hpp"

namespace stp
{


class Gnuplot_sim : public Model
{
    public:
    Gnuplot_sim(double deltas[4], double a, double l) : Model(deltas, a, l)
    {
        //setup the vector used to draw the platform
        m_plot.P.resize(6);
        m_plot.B.resize(6);
        m_plot.A.resize(6);
        for(int i = 0; i < NB_LEGS; i++)
        {
            m_plot.P[i].resize(3);
            m_plot.B[i].resize(3);
            m_plot.B[i].assign(&(m_B[i][0]), &(m_B[i][3]));
            m_plot.A[i].resize(3);
        }
    };

    double *
    new_pos(double T[3], double theta[3])
    {

        Model::new_pos(T, theta);
        update_draw();
        return m_alpha;
    };

    void
    update_draw()
    {
        for(int i = 0; i < NB_LEGS; i++)
        {

            m_A[i][0] = m_a * cos(m_alpha[i]) * cos(m_beta[i]) + m_B[i][0];
            m_A[i][1] = m_a * cos(m_alpha[i]) * sin(m_beta[i]) + m_B[i][1];
            m_A[i][2] = m_a * sin(m_alpha[i]) + m_B[i][2];
            m_plot.A[i].assign(&(m_A[i][0]), &(m_A[i][3]));

            m_plot.P[i].assign(&(m_P[i][0]), &(m_P[i][3]));
        }
        draw();
    }

    void
    draw();

    private:
    struct Plot
    {
        Gnuplot gp;
        std::vector<std::vector<double>> P;
        std::vector<std::vector<double>> A;
        std::vector<std::vector<double>> B;
    };
    Plot m_plot;
};

}

#endif
