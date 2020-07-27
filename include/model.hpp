#ifndef _MODEL_H_
#define _MODEL_H_

#define M_PI 3.14159265358979323846 /* pi */
#define NB_LEGS 6


namespace stp
{

class Model
{
    public:
    Model(){};
    Model(double deltas[4], double a, double l);

    double *
    new_pos(double T[3], double theta[3]);

    double
    get_T(int i);

    double
    get_theta(int i);

    void
    init_pos();

    void
    set_verbose(bool v = true)
    {
        m_verbose = v;
    };

    void
    compute_R(double theta[3]);

  void print_pos();

    protected:
    void
    set_T(double t0, double t1, double t2);

    void
    set_T_spd(double t0, double t1, double t2);

    void
    set_theta(double t0, double t1, double t2);

    void
    set_theta_spd(double t0, double t1, double t2);

    double m_radius[2]; /*!< */

    double m_a, m_a2; /*!< Size of the motors' arms. */
    double m_l, m_l2; /*!< Size of the legs. */
    double _d2
        [NB_LEGS]; /*!< Distance between the axe of the motors and the coresponding legs-platform articulation. */

    double m_P
        [NB_LEGS]
        [3]; /*!< Position of the legs-platform articulation in the base B0.*/
    double m_P1
        [NB_LEGS]
        [3]; /*!< Position of the legs-platform articulation in the base B1.*/
    double
        m_B[NB_LEGS]
           [3]; /*!< Position of the motors-arms articulation in the base B0.*/
    double m_A[NB_LEGS]
              [3]; /*!< Position of the arms-legs articulation in the base B0.*/

    double m_parity[NB_LEGS]; /*!< Use to represent the side of the motor arm.*/
    double m_beta[NB_LEGS];   /*!< Orientation of the motor on the base.*/
    double m_alpha[NB_LEGS];  /*!< Angular postion of the rotor. */
    double m_gamma[2]
                  [NB_LEGS]; /*!< Angular position of the motor on the base */

    double m_T[3];     /*!< Postion of the center of the platform. */
    double m_theta[3]; /*!< Orientation of the platform. */
    double m_R[3][3];  /*!< Matrix of rotation. */
    double m_trig[6];  /*!< (cos(theta_i),sin(theta_i))*/

    double m_T_spd[3];     /*!< Velocity of the platform. */
    double m_theta_spd[3]; /*!< Rotation speed of the platform. */
    double m_alpha_spd[6]; /*!< Rotation speed of the motors. */

    double _alphaI[6];

    bool m_verbose = false;
};

}; // namespace stp

#endif
