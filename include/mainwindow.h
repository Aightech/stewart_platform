#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>

#include "platform.hpp"
#include "gnuplot_sim.hpp"
#include "joystick.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief update_pos Update the GUI to display the current position of the platform.
     */
    void update_pos();

    /**
     * @brief loop  Called every 1ms to send order depending of the mode.
     */
    void loop();

private slots:
    void on_pushButton_start_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_stop_clicked();


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    stp::Platform *m_sp;
    bool m_running=false;
    double m_T[3] = {0, 0, 0};
    double m_T_mid[3];
    double m_theta[3] = {0, 0, 0};

    QDoubleSpinBox* m_cart_trans[3];
    QDoubleSpinBox* m_cart_anlge[3];
    QDoubleSpinBox* m_sin_dir[3];

    std::chrono::time_point<std::chrono::system_clock> m_prev_time;

    cJoystick m_js;

    bool m_gnuplot = false;
    stp::Gnuplot_sim *m_sim;


};
#endif // MAINWINDOW_H
