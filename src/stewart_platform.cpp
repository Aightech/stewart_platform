#include <cmath>
#include <iostream>
#include <vector>

#include <cstdlib>
#include "stewart_platform.hpp"

Stewart_platform::Stewart_platform(double deltas[4], double a, double l)
{
  for(int i=0; i<NB_LEGS; i++)
    {
      motors.push_back(new LXM32("can0", i+1,false));
      if(motors.back()->is_available())
	{
	  motors.back()->init();
	  motors.back()->start(MODE_ProfilePosition, PPctrl_RELATIVE| PPctrl_ON_DIRECT);
	}
      
      m_motor_pos[i]=0;
      m_inc[i]=0;
      
    }


  
  _inc=0;
  //set the length of the arms and legs
  _a=a;
  _l=l;
  _a2= a*a; 
  _l2= l*l;


  //compute the radius of the base and the platform from the delta lengths.
  for(int i =0;i<2;i++)
    _radius[i] = deltas[2*i+1]/(2*sin(atan(deltas[2*i+1]/(2*deltas[2*i]+deltas[2*i+1]))));


  //Set the constant coordinate of B and P1 (P in B1).
  for(int i=0; i<NB_LEGS; i++)
    {
      _parity[i]=(i-i/2*2);
      //angle of the arms on Oxy.
      _beta[i]=i/2*M_PI*2/3 -(2*_parity[i]-1)*M_PI/2;
      //angle of the motor on the base circle.
      _gamma[0][i]=_beta[i] +(2*_parity[i]-1)*M_PI/2 +(2*_parity[i]-1)*asin(deltas[0]/2/_radius[0]);
      _gamma[1][i]=_beta[i] +(2*_parity[i]-1)*M_PI/2 +(2*_parity[i]-1)*asin(deltas[1]/2/_radius[1]);
	
      _B[i][0]= _radius[0]*cos(_gamma[0][i]);
      _B[i][1]= _radius[0]*sin(_gamma[0][i]);
      _B[i][2]= 0;
	
      _P1[i][0]= _radius[1]*cos(_gamma[1][i]);
      _P1[i][1]= _radius[1]*sin(_gamma[1][i]);
      _P1[i][2]=0;

      _alpha_p[i]=0;
    }

  //setup the vector used to draw the platform
  _P0_vect.resize(6);
  _B_vect.resize(6);
  _A_vect.resize(6);
  for(int i=0; i<NB_LEGS; i++)
    {
      _P0_vect[i].resize(3);
      _B_vect[i].resize(3);
      _B_vect[i].assign(&(_B[i][0]), &(_B[i][3]));
      _A_vect[i].resize(3);
    }

  //compute h0
  double bp1_0 = _B[0][0]-_P1[0][0];
  double bp1_1 = _B[0][1]-_P1[0][1];
  double h0 = sqrt(_l2 + _a2-bp1_0*bp1_0 - bp1_0*bp1_0);

  //set up the initial position.
  set_T(0,0,h0);
  set_theta(0,0,0);

  set_T_p(0,0,0);
  set_theta_p(0,0,0);

  new_pos(_T, _theta);


  for(int i =0;i<6; i++)
    {
      _alphaI[i] = _alpha[i];
    }


  
   


};
  

double* Stewart_platform::new_pos(double T[3], double theta[3])
{
  ////////////
  double T_tmp[3];
  double theta_tmp[3];
  for(int j=0; j<3; j++)
    {
      T_tmp[j] = _T[j];
      theta_tmp[j] = _theta[j];
      _T[j]=T[j];
    }
  compute_R(theta);

  ////////////////
  double tmp;
  double L;
  double M;
  double N;
  for(int i=0; i<NB_LEGS; i++)
    {
      _d2[i]=0;
      double l2 = 0;
      double a2 = 0;
      double p2 = 0;
      for(int j=0; j<3; j++)
	{
	  _P0[i][j]=_T[j];
	  for(int k=0; k<3; k++)
	    _P0[i][j] += _R[j][k]*_P1[i][k];
	  tmp = _P0[i][j] - _B[i][j];
	  _d2[i]+=tmp*tmp;	 
	}
      _P0_vect[i].assign(&(_P0[i][0]), &(_P0[i][3]));
      
	
      L = (_a2-_l2+_d2[i])/(2*_a);
      M = cos(_beta[i])*(_P0[i][0]-_B[i][0]) + sin(_beta[i])*(_P0[i][1]-_B[i][1]);
      N = (_P0[i][2]-_B[i][2]);

      if(L/sqrt(M*M + N*N) >= 1 ||  L/sqrt(M*M + N*N) <= -1)
	{
	  new_pos(T_tmp,theta_tmp);
	  break;
	}
      _alpha[i] = asin(L/sqrt(M*M + N*N)) - atan(M/N);

      int i_n = m_motor_lookup[i];
      m_inc[i_n] = -_alpha[i]*m_conv - m_motor_pos[i_n];
      m_motor_pos[i_n]+= m_inc[i_n];
      
      _A[i][0]= _a*cos(_alpha[i])*cos(_beta[i]) + _B[i][0];
      _A[i][1]= _a*cos(_alpha[i])*sin(_beta[i]) + _B[i][1];
      _A[i][2]= _a*sin(_alpha[i]) + _B[i][2];
      _A_vect[i].assign(&(_A[i][0]), &(_A[i][3]));

    }

  for(int i=0; i<NB_LEGS; i++)
    {
      std::cout << m_inc[i] << std::endl; 
      motors[i]->new_pos(m_inc[i]);
    }
	
  return _alpha;
}


double* Stewart_platform::new_speed(double T_p[3], double theta_p[3])
{
  return _alpha_p;
}

void Stewart_platform::update(double dt)
{
  double T[3];
  double theta[3];
  double alpha[6];
  for(int i = 0; i<3; i++)
    {
      T[i] = _T[i]+ _T_p[i]*dt;
      theta[i] = _theta[i]+ _theta_p[i]*dt;
      alpha[2*i] = _alpha[2*i];
      alpha[2*i+1] = _alpha[2*i+1];
    }
  
  new_pos(T, theta);

  std::cout << "#######################" << std::endl;
  for(int i =0; i<6; i++)
    {
      std::cout <<  i  << ": " << _alpha[i] - alpha[i] << " " <<  _alpha_p[i]*dt << std::endl;
    }
  
}


void Stewart_platform::compute_R(double theta[3])
{
    
  for(int j=0; j<3; j++)
    {
      _theta[j] = theta[j];
      _trig[j*2] = cos(_theta[j]);
      _trig[j*2+1] = sin(_theta[j]);
    }
  _R[0][0] = _trig[0]*_trig[2];
  _R[0][1] =-_trig[1]*_trig[4] + _trig[0]*_trig[3]*_trig[5];
  _R[0][2] = _trig[1]*_trig[5] + _trig[0]*_trig[3]*_trig[4];
  _R[1][0] = _trig[1]*_trig[2];
  _R[1][1] = _trig[0]*_trig[4] + _trig[1]*_trig[3]*_trig[5];
  _R[1][2] =-_trig[0]*_trig[5] + _trig[1]*_trig[3]*_trig[4];
  _R[2][0] =-_trig[3];
  _R[2][1] = _trig[2]*_trig[5];
  _R[2][2] = _trig[2]*_trig[4];
}

void Stewart_platform::draw()
{
  std::vector<std::vector<std::vector<std::vector<double>>>> lines(15);
  std::vector<double> pt;

  lines[0].resize(1);
  lines[1].resize(1);
  for(int i=0; i<NB_LEGS; i++)
    {
      if(i==2)
	i++;
      lines[0][0].push_back(_B_vect[i]);
      lines[1][0].push_back(_P0_vect[i]);

      lines[i+3].resize(1);
      lines[i+3][0].push_back(_B_vect[i]);
      lines[i+3][0].push_back(_A_vect[i]);

      lines[i+3+6].resize(1);
      lines[i+3+6][0].push_back(_A_vect[i]);
      lines[i+3+6][0].push_back(_P0_vect[i]);

      
    }
  lines[0][0].push_back(_B_vect[0]);
  lines[1][0].push_back(_P0_vect[0]);

  
  lines[2].resize(1);
  for(int i=0; i<360;i++)
    {
      std::vector<double> v(3);
      v[0]= _radius[0]*cos(M_PI/180*i);
      v[1]= _radius[0]*sin(M_PI/180*i);
      v[2]= _B[0][2];
      lines[2][0].push_back(v);
    }
  
  _gp << "set zrange [-0.2:1]\n";
  _gp << "set hidden3d nooffset\n";
  _gp << "splot ";
  for(int i =1; i<3; i++)
    {
      _gp << _gp.binFile2d(lines[i],"record");
      _gp << "with lines notitle,";
    }
  for(int i =3; i<lines.size(); i++)
    {
      _gp << _gp.binFile2d(lines[i],"record");
      _gp << "with lines notitle lw 2,";
    }

  _gp << " \n";
  
  
}
