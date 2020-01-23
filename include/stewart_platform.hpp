#define GNUPLOT_ENABLE_PTY
#include "gnuplot-iostream.h"
# define M_PI 3.14159265358979323846  /* pi */
#define NB_LEGS 6

#include "Lexium32A_canopen.h"

class Stewart_platform
{
 public:
  Stewart_platform(){};
  Stewart_platform(double deltas[4], double a, double l);

  ~Stewart_platform()
  {
    _gp.clearTmpfiles();
  };

  
  double* new_pos(double T[3], double theta[3]);
  
  double* new_speed(double T_p[3], double theta_p[3]);
  void update(double dt);
  
  void set_T(double t0, double t1, double t2)
  {
    _T[0]=t0;
    _T[1]=t1;
    _T[2]=t2;
  }
  
  void set_T_p(double t0, double t1, double t2)
  {
    _T_p[0]=t0;
    _T_p[1]=t1;
    _T_p[2]=t2;
  }
  
  double get_T(int i){return _T[i];};
  double get_theta(int i){return _theta[i];};
  
  void set_theta(double t0, double t1, double t2)
  {
    _theta[0]=t0;
    _theta[1]=t1;
    _theta[2]=t2;
  }
  void set_theta_p(double t0, double t1, double t2)
  {
    _theta_p[0]=t0;
    _theta_p[1]=t1;
    _theta_p[2]=t2;
  }

  void draw();


  

 private:
  void compute_R(double theta[3]);

  long _inc;

  
  double _radius[2]; /*!< */
  
  double _a, _a2; /*!< Size of the motors' arms. */
  double _l, _l2; /*!< Size of the legs. */
  double _d2[NB_LEGS]; /*!< Distance between the axe of the motors and the coresponding legs-platform articulation. */

  double _P0[NB_LEGS][3]; /*!< Position of the legs-platform articulation in the base B0.*/
  double _P1[NB_LEGS][3]; /*!< Position of the legs-platform articulation in the base B1.*/
  double _B[NB_LEGS][3]; /*!< Position of the motors-arms articulation in the base B0.*/
  double _A[NB_LEGS][3]; /*!< Position of the arms-legs articulation in the base B0.*/

  
  double _parity[NB_LEGS]; /*!< Use to represent the side of the motor arm.*/
  double _beta[NB_LEGS]; /*!< Orientation of the motor on the base.*/
  double _alpha[NB_LEGS]; /*!< Angular postion of the rotor. */
  double _gamma[2][NB_LEGS]; /*!< Angular position of the motor on the base */

  double _T[3]; /*!< Postion of the center of the platform. */
  double _theta[3]; /*!< Orientation of the platform. */
  double _R[3][3]; /*!< Matrix of rotation. */
  double _trig[6]; /*!< (cos(theta_i),sin(theta_i))*/

  double _T_p[3]; /*!< Velocity of the platform. */
  double _theta_p[3]; /*!< Rotation speed of the platform. */
  double _alpha_p[6];  /*!< Rotation speed of the motors. */

  Gnuplot _gp;
  std::vector<std::vector<double>> _P0_vect;
  std::vector<std::vector<double>> _A_vect;
  std::vector<std::vector<double>> _B_vect;

  std::vector<LXM32*> motors;
  int32_t m_motor_pos[NB_LEGS];
  int32_t m_inc[NB_LEGS];
  double m_conv=50000.0f/24.0f*180.0f/M_PI;

  int m_motor_lookup[NB_LEGS] = {5,2,0,1,4,3};


  
};
