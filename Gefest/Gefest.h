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

    QString absolutePath = "";
    QString dataPath     = "";

    QString timeString = "";

    bool pause = false;
    bool stop  = false;

public:
    explicit Gefest(QWidget *parent = 0);
    ~Gefest();

private slots:
    void on_actionCom_UPD_triggered();
    void on_pushButtonAction_clicked();

    void updateTimeString();
    QString getTimeString();

    void on_checkBoxOnChannel_1_stateChanged(int arg1);
    void on_checkBoxOnChannel_2_stateChanged(int arg1);
    void on_checkBoxOnChannel_3_stateChanged(int arg1);
    void on_checkBoxOnChannel_4_stateChanged(int arg1);
    void on_checkBoxOnChannel_5_stateChanged(int arg1);

    void on_plotTemperature_Clicked(QMouseEvent* event) const;
    void on_plotPower_Clicked(QMouseEvent* event) const;

    void on_pushButtonStart_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();

    void RunExperiment();

    void on_toolButtonImport_clicked();
    void on_toolButtonExport_clicked();
    void on_pushButtonImport_clicked();
    void on_pushButtonExport_clicked();

    void on_pushButtonSetP_clicked();
    void on_pushButtonSetT_clicked();

    void loadConfig();
    void saveConfig();

private:
    Ui::Gefest *ui;
};

#endif // GEFEST_H
