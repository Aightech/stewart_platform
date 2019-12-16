#include <cmath>
#include <iostream>


int main()
{
  double a = 1;
  double l = 2;

  
  double b[3] = {2,2,0};
  double p[3] = {1,1,0};
  double beta = 0;


  

  double d2[6];
  double a2 = a*a;
  double l2 = l*l;
  double v;

  double T[3] = {0,0,2};
  double R[3] = {0,0,0};
  double alpha[6];
  double trig[6] = { cos(R[0]),sin(R[0]),
		     cos(R[1]),sin(R[1]),
		     cos(R[2]),sin(R[2]) };
  
  double rot[3][3] = { {trig[0]*trig[2], -trig[1]*trig[4]+trig[0]*trig[3]*trig[5], trig[1]*trig[5]+trig[0]*trig[3]*trig[4] },
		       {trig[1]*trig[2], trig[0]*trig[4]+trig[1]*trig[3]*trig[5], -trig[0]*trig[5]+trig[1]*trig[3]*trig[4] },
		       {-trig[3], trig[2]*trig[5], trig[2]*trig[4] } };
  
  for(int j=0; j<6; j++)
    {
      d2[j]=0;
      for(int i=0;i<3;i++)
	{
	  v=T[i]-b[i];
	  for(int j=0;j<3;j++)
	    v+=rot[i][j]*p[i];
	  d2[j]+=v*v;
	}
      double L = (a2-l2+d2[j])/(2*a);
      double M = cos(beta)*(p[0]-b[0]) + sin(beta)*(p[1]-b[1]);
      double N = (p[2]-b[2]);
      double alpha = asin(L/sqrt(M*M + N*N)) - atan(N/M);
    }

  
    

  


  
  
  
}
