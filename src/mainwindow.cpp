#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    m_sp = new stp::Platform({.radius_base=0.415,
                              .radius_platform=0.255,
                              .delta_base=0.63,
                              .delta_platform=0.075,
                              .arm=0.075,
                              .leg=0.4375} ,0);//0.413//;0.63, 0.16, 0.075, 0.38}, 0.08, 0.44,0);
    m_T[2] = m_sp->get_T(2);
    m_sp->init();
    m_js.pulse();
    m_running=false;
    m_sp->stop();
    ui->label_state->setText("[ Stop ]");
    std::cout << "Initialized\n";

    m_cart_anlge[0] = ui->doubleSpinBox_cart_roll;
    m_cart_anlge[1] = ui->doubleSpinBox_cart_pitch;
    m_cart_anlge[2] = ui->doubleSpinBox_cart_yaw;

    m_cart_trans[0] = ui->doubleSpinBox_cart_x;
    m_cart_trans[1] = ui->doubleSpinBox_cart_y;
    m_cart_trans[2] = ui->doubleSpinBox_cart_z;

    m_sin_dir[0] = ui->doubleSpinBox_sin_x;
    m_sin_dir[1] = ui->doubleSpinBox_sin_y;
    m_sin_dir[2] = ui->doubleSpinBox_sin_z;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::loop);
    timer->start(10);

    update_pos();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loop()
{

    if(m_running)
    {
        if (ui->radioButton_cart_mvt->isChecked())
        {
            for(int i = 0; i< 3; i++)
            {
                m_T[i] = m_cart_trans[i]->value();
                m_theta[i] = m_cart_anlge[i]->value();
            }
        }
        if (ui->radioButton_sin_mvt->isChecked())
        {
            double sin_v = sin(ui->doubleSpinBox_sin_omega->value()*chr::duration<double>(chr::high_resolution_clock::now()-m_prev_time).count());
            for(int i =0; i<3; i++)
                m_T[i] = m_T_mid[i] + m_sin_dir[i]->value()*sin_v;
        }
        else if (ui->radioButton_cart_joy->isChecked())
        {
            for(int i = 0; i< 2; i++)
            {
                //m_T[i] = 0.3*m_T[i] + 0.7*m_js.joystickValue(i) / 32767. * 0.07;
                //m_theta[i] = 0.3*m_T[i] + 0.7*m_js.joystickValue(3+i) / 32767. * 0.07;

                m_T[i] = m_js.joystickValue(i) / 32767. * 0.07*(1-2*i);
                m_theta[i+1] = m_js.joystickValue(3+i) / 32767. * 0.1;
            }
            m_T[2] -= m_js.joystickValue(7) / 32767 * 0.005;
            m_theta[0] -= m_js.joystickValue(6) / 32767 * 0.005;
        }

        try {

            m_sp->new_pos(m_T, m_theta);
            update_pos();
        } catch (const std::runtime_error& e) {
            std::cerr << e.what();
            if(m_T[0]+m_T[1]>0)
                m_js.leftPulse();
            else
                m_js.rightPulse();
        }
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    ui->label_state->setText("[ Start ]");
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);
    m_sp->start();
    m_running=true;
    if(ui->radioButton_cart_mvt->isChecked())
    {
        for(int i = 0; i< 3; i++)
        {
            m_T[i] = m_sp->get_T(i);
            m_theta[i] = m_sp->get_theta(i);
            m_cart_trans[i]->setValue(m_T[i]);
            m_cart_anlge[i]->setValue(m_theta[i]);
        }
    }
    else if (ui->radioButton_sin_mvt->isChecked())
    {
        for(int i =0; i<3; i++)
            m_T_mid[i] = m_sp->get_T(i);
        m_prev_time = chr::high_resolution_clock::now();
    }
    else if (ui->radioButton_cart_joy->isChecked())
    {

        for(int i =0; i<3; i++)
            m_T_mid[i] = m_sp->get_T(i);
        m_prev_time = chr::high_resolution_clock::now();
    }
}

void MainWindow::on_pushButton_pause_clicked()
{
    ui->label_state->setText("[ Pause ]");
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
    m_running=false;
    m_sp->pause();
}

void MainWindow::on_pushButton_stop_clicked()
{
    ui->label_state->setText("[ Stop ]");
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    m_running=false;
    m_sp->stop();
}

void MainWindow::update_pos()
{
    double t[3] = {m_sp->get_T(0), m_sp->get_T(1), m_sp->get_T(2) };
    double theta[3] = {m_sp->get_theta(0), m_sp->get_theta(1), m_sp->get_theta(2) };
    ui->lineEdit_pos_x->setText(QString::number(t[0]));
    ui->lineEdit_pos_y->setText(QString::number(t[1]));
    ui->lineEdit_pos_z->setText(QString::number(t[2]));


    ui->lineEdit_pos_roll->setText(QString::number(theta[0]));
    ui->lineEdit_pos_pitch->setText(QString::number(theta[1]));
    ui->lineEdit_pos_yaw->setText(QString::number(theta[2]));

    if(m_gnuplot==true)
        m_sim->new_pos(t, theta);


}

void MainWindow::on_pushButton_clicked()
{
    std::cout << "gnuplot" << std::endl;

    if(m_gnuplot==false)
    {
        std::cout << "gnuplot" << std::endl;
        m_sim = new stp::Gnuplot_sim({.radius_base=0.45,
                                  .radius_platform=0.244,
                                  .delta_base=0.63,
                                  .delta_platform=0.075,
                                  .arm=0.075,
                                  .leg=0.4375});
        m_gnuplot=true;
        update_pos();
    }
}
