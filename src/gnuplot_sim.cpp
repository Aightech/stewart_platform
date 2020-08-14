#include "gnuplot_sim.hpp"

void
stp::Gnuplot_sim::draw()
{
    std::vector<std::vector<std::vector<std::vector<double>>>> lines(15);
    std::vector<double> pt;

    lines[0].resize(1);
    lines[1].resize(1);
    for(int i = 0; i < NB_LEGS; i++)
    {
        lines[0][0].push_back(m_plot.B[i]);
        lines[1][0].push_back(m_plot.P[i]);

        lines[i + 3].resize(1);
        lines[i + 3][0].push_back(m_plot.B[i]);
        lines[i + 3][0].push_back(m_plot.A[i]);

        lines[i + 3 + 6].resize(1);
        lines[i + 3 + 6][0].push_back(m_plot.A[i]);
        lines[i + 3 + 6][0].push_back(m_plot.P[i]);
    }
    lines[0][0].push_back(m_plot.B[0]);
    lines[1][0].push_back(m_plot.P[0]);

    lines[2].resize(1);
    for(int i = 0; i < 360; i++)
    {
        std::vector<double> v(3);
        v[0] = m_radius[0] * cos(M_PI / 180 * i);
        v[1] = m_radius[0] * sin(M_PI / 180 * i);
        v[2] = m_B[0][2];
        lines[2][0].push_back(v);
    }

    m_plot.gp << "set zrange [-0.2:1]\n";
    m_plot.gp << "set hidden3d nooffset\n";
    m_plot.gp << "splot ";
    for(int i = 1; i < 3; i++)
    {
        m_plot.gp << m_plot.gp.binFile2d(lines[i], "record");
        m_plot.gp << "with lines notitle,";
    }
    for(int i = 3; i < lines.size(); i++)
    {
        m_plot.gp << m_plot.gp.binFile2d(lines[i], "record");
        m_plot.gp << "with lines notitle lw 2,";
    }

    m_plot.gp << " \n";
    
//    for(int i = 0; i < NB_LEGS; i++)
//    {
//        std::cout << "\t" << m_alpha[i];//m_motors[m_motor_lookup[i]]->set_angle(m_alpha[i]);
//    }
//    std::cout << "\n";
}
