#define GNUPLOT_ENABLE_PTY
#include "gnuplot-iostream.h"
# define M_PI 3.14159265358979323846  /* pi */
#define NB_LEGS 6


class Stewart_platform
{
 public:
  Stewart_platform(){};
  Stewart_platform(double deltas[4], double a, double l);

  ~Stewart_platform(){};

  
  double* new_pos(double T[3], double theta[3]);
  
  void set_T(double t0, double t1, double t2)
  {
    _T[0]=t0;
    _T[1]=t1;
    _T[2]=t2;
  }
  double get_T(int i){return _T[i];};
  double get_theta(int i){return _theta[i];};
  
  void set_theta(double t0, double t1, double t2)
  {
    _theta[0]=t0;
    _theta[1]=t1;
    _theta[2]=t2;
  }

  void draw();

  

 private:
  void compute_R(double theta[3]);

  
  double _radius[2]; /*!< */
  
  double _a, _a2; /*!< Size of the motors' arms. */
  double _l, _l2; /*!< Size of the legs. */
  double _d2[NB_LEGS]; /*!< Distance between the axe of the motors and the coresponding legs-platform articulation. */

  double _P0[NB_LEGS][3]; /*!< Position of the legs-platform articulation in the base B0.*/
  double _P1[NB_LEGS][3]; /*!< Position of the legs-platform articulation in the base B1.*/
  double _B[NB_LEGS][3]; /*!< Position of the motors-arms articulation in the base B0.*/
  double _A[NB_LEGS][3]; /*!< Position of the arms-legs articulation in the base B0.*/

  
  double _parity[NB_LEGS]; /*!< */
  double _beta[NB_LEGS]; /*!< */
  double _alpha[NB_LEGS]; /*!< */
  double _gamma[2][NB_LEGS]; /*!< */

  double _T[3]; /*!< */
  double _theta[3]; /*!< */
  double _R[3][3]; /*!< */
  double _trig[6]; /*!< */

  Gnuplot _gp;
  std::vector<std::vector<double>> _P0_vect;
  std::vector<std::vector<double>> _A_vect;
  std::vector<std::vector<double>> _B_vect;
};
