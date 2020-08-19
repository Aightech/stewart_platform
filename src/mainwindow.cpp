#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    double deltas[4] = {0.63, 0.16, 0.075, 0.38};
    m_sp = new stp::Platform(deltas, 0.08, 0.44,0);
    m_T[2] = m_sp->get_T(2);
    m_sp->init();
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
    timer->start(1);

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

        m_sp->new_pos(m_T, m_theta);
        m_sp->update_platform();
    }
}

void MainWindow::on_pushButton_start_clicked()
{
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
}

void MainWindow::on_pushButton_pause_clicked()
{
    m_running=false;
    m_sp->pause();
}

void MainWindow::on_pushButton_stop_clicked()
{
    m_running=false;
    m_sp->stop();
}

void MainWindow::update_pos()
{
    ui->lineEdit_pos_x->setText(QString::number(m_T[0]));
    ui->lineEdit_pos_y->setText(QString::number(m_T[1]));
    ui->lineEdit_pos_z->setText(QString::number(m_T[2]));


    ui->lineEdit_pos_roll->setText(QString::number(m_theta[0]));
    ui->lineEdit_pos_pitch->setText(QString::number(m_theta[1]));
    ui->lineEdit_pos_yaw->setText(QString::number(m_theta[2]));
}
