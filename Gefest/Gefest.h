#ifndef GEFEST_H
#define GEFEST_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <string>

#include "../ANGEL/Angel.h"

namespace Ui {
    class Gefest;
}

class Gefest : public QMainWindow
{
    Q_OBJECT

private:
    QSerialPort serial;
    SRS srs;

    double yMin = 0;
    double yMax = 0;

    bool run = false;
    bool old = false;

    QString absolutePath       = "";
    QString dataPath           = "";

public:
    explicit Gefest(QWidget *parent = 0);
    ~Gefest();

private slots:
    void on_actionCom_UPD_triggered();
    void on_pushButtonAction_clicked();

    void on_checkBoxOnChannel_1_stateChanged(int arg1);
    void on_checkBoxOnChannel_2_stateChanged(int arg1);
    void on_checkBoxOnChannel_3_stateChanged(int arg1);
    void on_checkBoxOnChannel_4_stateChanged(int arg1);
    void on_checkBoxOnChannel_5_stateChanged(int arg1);

    void on_plot_Clicked(QMouseEvent* event) const;

    void on_pushButtonPlot_clicked();
    void RunExperiment();

private:
    Ui::Gefest *ui;
};

#endif // GEFEST_H
