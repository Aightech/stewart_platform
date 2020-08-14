#include "mainwindow.h"

#include <QApplication>

#define MAX_JOY 52767.
#define ANG_MAX 0.06
#define TRANS_MAX 0.06


int
main(int argc, char *argv[])
{

 QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    //cJoystick js;

//    double deltas[4] = {0.63, 0.16, 0.075, 0.38};
//    stp::Platform sp(deltas, 0.08, 0.44,0);
//    //stp::Gnuplot_sim sp2(deltas, 0.08, 0.44);

//    double T[3] = {0, 0, sp.get_T(2)};
//    double theta[3] = {0, 0, 0};

//    //sp2.print_pos();
//    sp.init();
//    usleep(3000000);
//    std::cout << "ok\n";
//    sp.start();
//sp.stop();    
    
    
    
    //Eigen::Vector3f dir(0, 0, 1.0);
    //sp.sin_wave(TRANS_MAX*0.1*dir.normalized(), 2*M_PI);
//    while(js.buttonPressed(7) == 0)//joystick's start button ends the loop
//    {
//      
//        T[0] = js.joystickValue(0) / MAX_JOY * TRANS_MAX;
//        T[1] = -js.joystickValue(1) / MAX_JOY * TRANS_MAX;
//        T[2] += js.buttonPressed(4) * 0.001 - js.buttonPressed(5) * 0.001;

//        theta[0] = js.joystickValue(2) / MAX_JOY * ANG_MAX;
//        theta[1] = js.joystickValue(3) / MAX_JOY * ANG_MAX;
//	
//	sp.new_pos(T, theta);
//	//sp2.new_pos(T, theta);
//	
//	//sp.print_pos();
//        
//    }
    
    
}
