#include "Gefest.h"
#include "ui_Gefest.h"

Gefest::Gefest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Gefest)
{
    ui->setupUi(this);

    setbuf(stdout, NULL);

    QDir currentFolder = QDir();
    this->absolutePath = currentFolder.absolutePath() + "\\";
    this->dataPath = this->absolutePath + "DataGefest\\";
    QDir dataFolder = QDir(this->dataPath);
    if (!dataFolder.exists())
        dataFolder.mkpath(".");

    on_actionCom_UPD_triggered();

    ui->checkBoxOnChannel_1->hide();
    ui->comboBoxChannel_1->hide();
    ui->lineEditChannel_1->hide();
    ui->checkBoxOnChannel_2->hide();
    ui->comboBoxChannel_2->hide();
    ui->lineEditChannel_2->hide();
    ui->checkBoxOnChannel_3->hide();
    ui->comboBoxChannel_3->hide();
    ui->lineEditChannel_3->hide();
    ui->checkBoxOnChannel_4->hide();
    ui->comboBoxChannel_4->hide();
    ui->lineEditChannel_4->hide();
    ui->checkBoxOnChannel_5->hide();
    ui->comboBoxChannel_5->hide();
    ui->lineEditChannel_5->hide();

    ui->checkBoxSetOutputPower->hide();
    ui->doubleSpinBoxSetOutputPower->hide();

    ui->checkBoxSetTemperature->hide();
    ui->doubleSpinBoxSetTemperature->hide();

    ui->labelP->hide();
    ui->doubleSpinBoxP->hide();
    ui->labelI->hide();
    ui->doubleSpinBoxI->hide();
    ui->labelD->hide();
    ui->doubleSpinBoxD->hide();

    ui->pushButtonPlot->hide();
    ui->pushButtonExport->hide();
    ui->lineEditExportFileHeader->hide();

    ui->labelX->hide();
    ui->lineEditX->hide();
    ui->labelY->hide();
    ui->lineEditY->hide();
    ui->customPlot->hide();

    this->serial.setDataBits(QSerialPort::Data8);
    this->serial.setBaudRate(QSerialPort::Baud9600);
    this->serial.setStopBits(QSerialPort::OneStop);
    this->serial.setParity(QSerialPort::NoParity);
    this->serial.setFlowControl(QSerialPort::HardwareControl);

    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);
    ui->customPlot->xAxis->setTickLabelFont(font);
    ui->customPlot->yAxis->setTickLabelFont(font);
    ui->customPlot->legend->setFont(font);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plot_Clicked(QMouseEvent*)));

    if (freopen("ConfigGefest.conf", "r+", stdin) != nullptr) {
        std::string buffer;
        while (getline(std::cin, buffer)) {
            QString command = QString(buffer.c_str());
            if (command.startsWith("PTC")) {
                if (command.contains("Com")) {
                    ui->comboBoxComPorts->setCurrentText(command.split(" ")[1]);
                    if (ui->comboBoxComPorts->currentText() == command.split(" ")[1])
                        on_pushButtonAction_clicked();
                }
                if (command.contains("Model")) {
                    this->old = command.toLower().contains("old");
                    qDebug() << this->old;
                }
            }
            if (command.startsWith("COM")) {
                if (command.contains("Baud")) {
                    this->serial.setBaudRate(command.split(" ")[1].toInt());
                }
                if (command.contains("FC")) {
                    QString mode = command.split(" ")[1];
                    if (mode == "No")
                        this->serial.setFlowControl(QSerialPort::NoFlowControl);
                    if (mode == "Soft")
                        this->serial.setFlowControl(QSerialPort::SoftwareControl);
                    if (mode == "Hard")
                        this->serial.setFlowControl(QSerialPort::HardwareControl);

                }
            }
        }
    }

}

Gefest::~Gefest()
{
    delete ui;
}

void Gefest::on_actionCom_UPD_triggered()
{
    QStringList allCom;

    allCom.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        allCom.push_back(info.portName());
    allCom.push_back("SetIt");

    ui->comboBoxComPorts->clear();
    ui->comboBoxComPorts->addItems(allCom);
    ui->comboBoxComPorts->setCurrentText("SetIt");

    return;
}

void Gefest::on_pushButtonAction_clicked()
{
    if (ui->pushButtonAction->text() == "Connect") {
        this->serial.setPortName(ui->comboBoxComPorts->currentText());
        if (!this->srs.openSerial(&(this->serial)))
            return;

        QString idn;
        if (!this->srs.getIDN(&(this->serial), idn))
            return;

        if (!idn.contains("PTC10"))
            return;

        ui->pushButtonAction->setText("Disconnect");

        QString channelsString;
        if (this->old)
            this->srs.sendQuery(&(this->serial), "getOutput.names\n", channelsString);
        else
            this->srs.sendQuery(&(this->serial), "getOutputs.names\n", channelsString);

        QStringList channelsRaw = channelsString.split(',');
        QStringList channels;
        for (size_t i = 0; i < channelsRaw.size(); i++)
            if (!channelsRaw[i].trimmed().isEmpty())
                channels.push_back(channelsRaw[i].trimmed());

        ui->comboBoxComPorts->setDisabled(true);

        ui->comboBoxChannel_1->addItems(channels);
        ui->comboBoxChannel_2->addItems(channels);
        ui->comboBoxChannel_3->addItems(channels);
        ui->comboBoxChannel_4->addItems(channels);
        ui->comboBoxChannel_5->addItems(channels);

        ui->checkBoxOnChannel_1->show();
        ui->comboBoxChannel_1->show();
        ui->lineEditChannel_1->show();

        ui->labelX->show();
        ui->lineEditX->show();
        ui->labelY->show();
        ui->lineEditY->show();
        ui->customPlot->show();

        ui->pushButtonPlot->show();
//        ui->pushButtonExport->show();
//        ui->lineEditExportFileHeader->show();
    } else {
        this->srs.disconnect(&(this->serial));

        ui->comboBoxComPorts->setEnabled(true);

        ui->checkBoxOnChannel_1->hide();
        ui->comboBoxChannel_1->hide();
        ui->lineEditChannel_1->hide();
        ui->checkBoxOnChannel_2->hide();
        ui->comboBoxChannel_2->hide();
        ui->lineEditChannel_2->hide();
        ui->checkBoxOnChannel_3->hide();
        ui->comboBoxChannel_3->hide();
        ui->lineEditChannel_3->hide();
        ui->checkBoxOnChannel_4->hide();
        ui->comboBoxChannel_4->hide();
        ui->lineEditChannel_4->hide();
        ui->checkBoxOnChannel_5->hide();
        ui->comboBoxChannel_5->hide();
        ui->lineEditChannel_5->hide();

        ui->pushButtonPlot->hide();
        ui->pushButtonExport->hide();
        ui->lineEditExportFileHeader->hide();

        ui->labelX->hide();
        ui->lineEditX->hide();
        ui->labelY->hide();
        ui->lineEditY->hide();
        ui->customPlot->hide();

        ui->checkBoxOnChannel_1->setChecked(false);
        ui->checkBoxOnChannel_2->setChecked(false);
        ui->checkBoxOnChannel_3->setChecked(false);
        ui->checkBoxOnChannel_4->setChecked(false);
        ui->checkBoxOnChannel_5->setChecked(false);

        ui->pushButtonAction->setText("Connect");
    }
}

void Gefest::on_checkBoxOnChannel_1_stateChanged(int arg1)
{
    if (ui->checkBoxOnChannel_1->checkState() == Qt::Checked)
    {
        ui->checkBoxOnChannel_2->show();
        ui->comboBoxChannel_2->show();
        ui->lineEditChannel_2->show();
    }
    if (ui->checkBoxOnChannel_1->checkState() == Qt::Unchecked)
    {
        ui->checkBoxOnChannel_2->hide();
        ui->comboBoxChannel_2->hide();
        ui->lineEditChannel_2->hide();
    }

    return;
}

void Gefest::on_checkBoxOnChannel_2_stateChanged(int arg1)
{
    if (ui->checkBoxOnChannel_2->checkState() == Qt::Checked)
    {
        ui->checkBoxOnChannel_1->setDisabled(true);

        ui->checkBoxOnChannel_3->show();
        ui->comboBoxChannel_3->show();
        ui->lineEditChannel_3->show();
    }
    if (ui->checkBoxOnChannel_2->checkState() == Qt::Unchecked)
    {
        ui->checkBoxOnChannel_1->setEnabled(true);

        ui->checkBoxOnChannel_3->hide();
        ui->comboBoxChannel_3->hide();
        ui->lineEditChannel_3->hide();
    }

    return;
}

void Gefest::on_checkBoxOnChannel_3_stateChanged(int arg1)
{
    if (ui->checkBoxOnChannel_3->checkState() == Qt::Checked)
    {
        ui->checkBoxOnChannel_2->setDisabled(true);

        ui->checkBoxOnChannel_4->show();
        ui->comboBoxChannel_4->show();
        ui->lineEditChannel_4->show();
    }
    if (ui->checkBoxOnChannel_3->checkState() == Qt::Unchecked)
    {
        ui->checkBoxOnChannel_2->setEnabled(true);

        ui->checkBoxOnChannel_4->hide();
        ui->comboBoxChannel_4->hide();
        ui->lineEditChannel_4->hide();
    }

    return;
}

void Gefest::on_checkBoxOnChannel_4_stateChanged(int arg1)
{
    if (ui->checkBoxOnChannel_4->checkState() == Qt::Checked)
    {
        ui->checkBoxOnChannel_3->setDisabled(true);

        ui->checkBoxOnChannel_5->show();
        ui->comboBoxChannel_5->show();
        ui->lineEditChannel_5->show();
    }
    if (ui->checkBoxOnChannel_4->checkState() == Qt::Unchecked)
    {
        ui->checkBoxOnChannel_3->setEnabled(true);

        ui->checkBoxOnChannel_5->hide();
        ui->comboBoxChannel_5->hide();
        ui->lineEditChannel_5->hide();
    }

    return;
}

void Gefest::on_checkBoxOnChannel_5_stateChanged(int arg1)
{
    if (ui->checkBoxOnChannel_5->checkState() == Qt::Checked)
    {
        ui->checkBoxOnChannel_4->setDisabled(true);
    }
    if (ui->checkBoxOnChannel_5->checkState() == Qt::Unchecked)
    {
        ui->checkBoxOnChannel_4->setEnabled(true);
    }

    return;
}

void Gefest::on_plot_Clicked(QMouseEvent *event) const
{
    ui->lineEditX->setText(QString::number(ui->customPlot->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditY->setText(QString::number(ui->customPlot->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Gefest::on_pushButtonPlot_clicked()
{
    if (ui->pushButtonPlot->text() == "Plot")
    {
        if (ui->checkBoxOnChannel_1->checkState() == Qt::Unchecked)
            return;

        this->setAttribute(Qt::WA_DeleteOnClose, true);

        ui->pushButtonPlot->setText("Stop");

        this->run = true;

        ui->pushButtonAction->setDisabled(true);

        ui->checkBoxOnChannel_1->setDisabled(true);
        ui->checkBoxOnChannel_2->setDisabled(true);
        ui->checkBoxOnChannel_3->setDisabled(true);
        ui->checkBoxOnChannel_4->setDisabled(true);
        ui->checkBoxOnChannel_5->setDisabled(true);
        ui->comboBoxChannel_1->setDisabled(true);
        ui->comboBoxChannel_2->setDisabled(true);
        ui->comboBoxChannel_3->setDisabled(true);
        ui->comboBoxChannel_4->setDisabled(true);
        ui->comboBoxChannel_5->setDisabled(true);

        RunExperiment();
    } else {
        ui->pushButtonPlot->setText("Plot");

        this->setAttribute(Qt::WA_DeleteOnClose, false);

        this->run = false;

        ui->pushButtonAction->setEnabled(true);

        ui->comboBoxChannel_1->setEnabled(true);
        ui->comboBoxChannel_2->setEnabled(true);
        ui->comboBoxChannel_3->setEnabled(true);
        ui->comboBoxChannel_4->setEnabled(true);
        ui->comboBoxChannel_5->setEnabled(true);

        if (ui->checkBoxOnChannel_5->checkState() == Qt::Unchecked) {
             ui->checkBoxOnChannel_1->setEnabled(true);
             ui->checkBoxOnChannel_2->setDisabled(true);
             ui->checkBoxOnChannel_3->setDisabled(true);
             ui->checkBoxOnChannel_4->setDisabled(true);
             ui->checkBoxOnChannel_5->setDisabled(true);
         }
        if (ui->checkBoxOnChannel_1->checkState() == Qt::Checked &&
            ui->checkBoxOnChannel_2->checkState() == Qt::Unchecked) {
            ui->checkBoxOnChannel_1->setEnabled(true);
            ui->checkBoxOnChannel_2->setEnabled(true);
            ui->checkBoxOnChannel_3->setEnabled(true);
            ui->checkBoxOnChannel_4->setEnabled(true);
            ui->checkBoxOnChannel_5->setEnabled(true);
        }
        if (ui->checkBoxOnChannel_2->checkState() == Qt::Checked &&
            ui->checkBoxOnChannel_3->checkState() == Qt::Unchecked) {
            ui->checkBoxOnChannel_1->setDisabled(true);
            ui->checkBoxOnChannel_2->setEnabled(true);
            ui->checkBoxOnChannel_3->setEnabled(true);
            ui->checkBoxOnChannel_4->setEnabled(true);
            ui->checkBoxOnChannel_5->setEnabled(true);
        }
        if (ui->checkBoxOnChannel_3->checkState() == Qt::Checked &&
            ui->checkBoxOnChannel_4->checkState() == Qt::Unchecked) {
            ui->checkBoxOnChannel_1->setDisabled(true);
            ui->checkBoxOnChannel_2->setDisabled(true);
            ui->checkBoxOnChannel_3->setEnabled(true);
            ui->checkBoxOnChannel_4->setEnabled(true);
            ui->checkBoxOnChannel_5->setEnabled(true);
        }
        if (ui->checkBoxOnChannel_4->checkState() == Qt::Checked &&
            ui->checkBoxOnChannel_5->checkState() == Qt::Unchecked) {
             ui->checkBoxOnChannel_1->setDisabled(true);
             ui->checkBoxOnChannel_2->setDisabled(true);
             ui->checkBoxOnChannel_3->setDisabled(true);
             ui->checkBoxOnChannel_4->setEnabled(true);
             ui->checkBoxOnChannel_5->setEnabled(true);
         }
        if (ui->checkBoxOnChannel_5->checkState() == Qt::Checked) {
             ui->checkBoxOnChannel_1->setDisabled(true);
             ui->checkBoxOnChannel_2->setDisabled(true);
             ui->checkBoxOnChannel_3->setDisabled(true);
             ui->checkBoxOnChannel_4->setDisabled(true);
             ui->checkBoxOnChannel_5->setEnabled(true);
         }
    }
}

void Gefest::RunExperiment()
{
    this->yMin = this->yMax = -1;

    double timeStart = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double currentTime = 0;
    double value = 0;

    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Value");
    ui->customPlot->clearGraphs();

    QString File = this->dataPath + QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss") + ".dat";
    freopen(File.toStdString().c_str(), "w+", stdout);

    bool channelChecked_1 = ui->checkBoxOnChannel_1->checkState() == Qt::Checked;
    bool channelChecked_2 = ui->checkBoxOnChannel_2->checkState() == Qt::Checked;
    bool channelChecked_3 = ui->checkBoxOnChannel_3->checkState() == Qt::Checked;
    bool channelChecked_4 = ui->checkBoxOnChannel_4->checkState() == Qt::Checked;
    bool channelChecked_5 = ui->checkBoxOnChannel_5->checkState() == Qt::Checked;

    if (channelChecked_1) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        std::cout << "Time1\t" << ui->comboBoxChannel_1->currentText().toStdString();
    }
    if (channelChecked_2) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        std::cout << "\tTime2\t" << ui->comboBoxChannel_2->currentText().toStdString();
    }
    if (channelChecked_3) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::green));
        std::cout << "\tTime3\t" << ui->comboBoxChannel_3->currentText().toStdString();
    }
    if (channelChecked_4) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::yellow));
        std::cout << "\tTime4\t" << ui->comboBoxChannel_4->currentText().toStdString();
    }
    if (channelChecked_5) {
        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::black));
        std::cout << "\tTime1\t" << ui->comboBoxChannel_5->currentText().toStdString();
    }
    fprintf(stdout, "\n");

    QString response;
    QString channelName_1 = ui->comboBoxChannel_1->currentText();
    if (this->old)
        channelName_1 += ".value";
    channelName_1 += "?";
    QString channelName_2 = ui->comboBoxChannel_2->currentText();
    if (this->old)
        channelName_2 += ".value";
    channelName_2 += "?";
    QString channelName_3 = ui->comboBoxChannel_3->currentText();
    if (this->old)
        channelName_3 += ".value";
    channelName_3 += "?";
    QString channelName_4 = ui->comboBoxChannel_4->currentText();
    if (this->old)
        channelName_4 += ".value";
    channelName_4 += "?";
    QString channelName_5 = ui->comboBoxChannel_5->currentText();
    if (this->old)
        channelName_5 += ".value";
    channelName_5 += "?";

    while (this->run) {

        if (channelChecked_1) {
            this->srs.sendQuery(&(this->serial), channelName_1, response);
            value = response.toDouble();
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            fprintf(stdout, "%0.20e\t%0.20e", currentTime, value);
            ui->lineEditChannel_1->setText(response);
            ui->customPlot->graph(0)->addData(currentTime, value);

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_2) {
            this->srs.sendQuery(&(this->serial), channelName_2, response);
            value = response.toDouble();
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            fprintf(stdout, "\t%0.20e\t%0.20e", currentTime, value);
            ui->lineEditChannel_2->setText(response);
            ui->customPlot->graph(1)->addData(currentTime, value);

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_3) {
            this->srs.sendQuery(&(this->serial), channelName_3, response);
            value = response.toDouble();
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            fprintf(stdout, "\t%0.20e\t%0.20e", currentTime, value);
            ui->lineEditChannel_3->setText(response);
            ui->customPlot->graph(2)->addData(currentTime, value);

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_4) {
            this->srs.sendQuery(&(this->serial), channelName_4, response);
            value = response.toDouble();
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            fprintf(stdout, "\t%0.20e\t%0.20e", currentTime, value);
            ui->lineEditChannel_4->setText(response);
            ui->customPlot->graph(3)->addData(currentTime, value);

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_5) {
            this->srs.sendQuery(&(this->serial), channelName_5, response);
            value = response.toDouble();
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            fprintf(stdout, "\t%0.20e\t%0.20e", currentTime, value);
            ui->lineEditChannel_5->setText(response);
            ui->customPlot->graph(4)->addData(currentTime, value);

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }

        QTest::qWait(5);
        fprintf(stdout, "\n");

        ui->customPlot->xAxis->setRange(0, currentTime);
        ui->customPlot->yAxis->setRange(yMin, yMax);
        ui->customPlot->replot();
    }

    fclose(stdout);

    return;
}
