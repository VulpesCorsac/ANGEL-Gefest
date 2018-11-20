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

    this->serial.setDataBits(QSerialPort::Data8);
    this->serial.setBaudRate(QSerialPort::Baud9600);
    this->serial.setStopBits(QSerialPort::OneStop);
    this->serial.setParity(QSerialPort::NoParity);
    this->serial.setFlowControl(QSerialPort::HardwareControl);

    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);

    ui->customPlot_1->setNotAntialiasedElements(QCP::aeAll);
    ui->customPlot_1->xAxis->setTickLabelFont(font);
    ui->customPlot_1->yAxis->setTickLabelFont(font);
    ui->customPlot_1->legend->setFont(font);
    ui->customPlot_1->axisRect()->setupFullAxesBox();

    ui->customPlot_2->setNotAntialiasedElements(QCP::aeAll);
    ui->customPlot_2->xAxis->setTickLabelFont(font);
    ui->customPlot_2->yAxis->setTickLabelFont(font);
    ui->customPlot_2->legend->setFont(font);
    ui->customPlot_2->axisRect()->setupFullAxesBox();

    ui->customPlot_3->setNotAntialiasedElements(QCP::aeAll);
    ui->customPlot_3->xAxis->setTickLabelFont(font);
    ui->customPlot_3->yAxis->setTickLabelFont(font);
    ui->customPlot_3->legend->setFont(font);
    ui->customPlot_3->axisRect()->setupFullAxesBox();

    ui->customPlot_4->setNotAntialiasedElements(QCP::aeAll);
    ui->customPlot_4->xAxis->setTickLabelFont(font);
    ui->customPlot_4->yAxis->setTickLabelFont(font);
    ui->customPlot_4->legend->setFont(font);
    ui->customPlot_4->axisRect()->setupFullAxesBox();

    ui->customPlot_1->clearGraphs();
    ui->customPlot_1->addGraph();
    ui->customPlot_1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->customPlot_2->clearGraphs();
    ui->customPlot_2->addGraph();
    ui->customPlot_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->customPlot_3->clearGraphs();
    ui->customPlot_3->addGraph();
    ui->customPlot_3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->customPlot_4->clearGraphs();
    ui->customPlot_4->addGraph();
    ui->customPlot_4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    connect(ui->customPlot_1, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plotChannel_1_Clicked(QMouseEvent*)));
    connect(ui->customPlot_2, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plotChannel_2_Clicked(QMouseEvent*)));
    connect(ui->customPlot_3, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plotChannel_3_Clicked(QMouseEvent*)));
    connect(ui->customPlot_4, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plotChannel_4_Clicked(QMouseEvent*)));

    for (double i = -1; i <= +1; i += 0.01) {
        ui->customPlot_1->graph(0)->addData(i, i);
        ui->customPlot_2->graph(0)->addData(i, i*i);
        ui->customPlot_3->graph(0)->addData(i, i*i*i);
        ui->customPlot_4->graph(0)->addData(i, i*i*i*i);
    }

    ui->customPlot_1->xAxis->setLabel("x");
    ui->customPlot_1->yAxis->setLabel("y = x");
    ui->customPlot_1->xAxis->setRange(-1, 1);
    ui->customPlot_1->yAxis->setRange(-1, 1);
    ui->customPlot_1->replot();

    ui->customPlot_2->xAxis->setLabel("x");
    ui->customPlot_2->yAxis->setLabel("y = x^2");
    ui->customPlot_2->xAxis->setRange(-1, 1);
    ui->customPlot_2->yAxis->setRange(0, 1);
    ui->customPlot_2->replot();

    ui->customPlot_3->xAxis->setLabel("x");
    ui->customPlot_3->yAxis->setLabel("y = x^3");
    ui->customPlot_3->xAxis->setRange(-1, 1);
    ui->customPlot_3->yAxis->setRange(-1, 1);
    ui->customPlot_3->replot();

    ui->customPlot_4->xAxis->setLabel("x");
    ui->customPlot_4->yAxis->setLabel("y = x^4");
    ui->customPlot_4->xAxis->setRange(-1, 1);
    ui->customPlot_4->yAxis->setRange(0, 1);
    ui->customPlot_4->replot();

    QStringList q;
    q.append("Sample 1");
    q.append("Sample 2");
    q.append("Sample 3");
    q.append("Sample 4");
    q.append("Sample 5");
    this->listLoad = true;
    ui->comboBoxChannel_1->addItems(q);
    ui->comboBoxChannel_2->addItems(q);
    ui->comboBoxChannel_3->addItems(q);
    ui->comboBoxChannel_4->addItems(q);
    ui->comboBoxChannel_5->addItems(q);
    this->listLoad = false;

    ui->lineEditXValuePlot_1->setAlignment(Qt::AlignHCenter);
    ui->lineEditYValuePlot_1->setAlignment(Qt::AlignHCenter);
    ui->lineEditXValuePlot_2->setAlignment(Qt::AlignHCenter);
    ui->lineEditYValuePlot_2->setAlignment(Qt::AlignHCenter);
    ui->lineEditXValuePlot_3->setAlignment(Qt::AlignHCenter);
    ui->lineEditYValuePlot_3->setAlignment(Qt::AlignHCenter);
    ui->lineEditXValuePlot_4->setAlignment(Qt::AlignHCenter);
    ui->lineEditYValuePlot_4->setAlignment(Qt::AlignHCenter);

    ui->lineEditXValuePlot_1->setText("Move cursor over plot");
    ui->lineEditYValuePlot_1->setText("Move cursor over plot");
    ui->lineEditXValuePlot_2->setText("Move cursor over plot");
    ui->lineEditYValuePlot_2->setText("Move cursor over plot");
    ui->lineEditXValuePlot_3->setText("Move cursor over plot");
    ui->lineEditYValuePlot_3->setText("Move cursor over plot");
    ui->lineEditXValuePlot_4->setText("Move cursor over plot");
    ui->lineEditYValuePlot_4->setText("Move cursor over plot");

    loadConfig();
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
        for (int i = 0; i < channelsRaw.size(); i++)
            if (!channelsRaw[i].trimmed().isEmpty())
                channels.push_back(channelsRaw[i].trimmed());

        ui->comboBoxComPorts->setDisabled(true);

        ui->comboBoxChannel_1->addItems(channels);
        ui->comboBoxChannel_2->addItems(channels);
        ui->comboBoxChannel_3->addItems(channels);
        ui->comboBoxChannel_4->addItems(channels);
        ui->comboBoxChannel_5->addItems(channels);

        ui->pushButtonStart->show();
//        ui->pushButtonExport->show();
//        ui->lineEditExportFileHeader->show();
    } else {
        this->srs.disconnect(&(this->serial));

        ui->comboBoxComPorts->setEnabled(true);

        ui->checkBoxOnChannel_1->setChecked(false);
        ui->checkBoxOnChannel_2->setChecked(false);
        ui->checkBoxOnChannel_3->setChecked(false);
        ui->checkBoxOnChannel_4->setChecked(false);
        ui->checkBoxOnChannel_5->setChecked(false);

        ui->pushButtonAction->setText("Connect");
    }

    return;
}

void Gefest::updateTimeString()
{
    this->timeString = QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss");
}

QString Gefest::getTimeString()
{
    if (this->timeString.isEmpty())
        updateTimeString();

    return this->timeString;
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

void Gefest::on_plotChannel_1_Clicked(QMouseEvent *event) const
{
    ui->lineEditXValuePlot_1->setText(QString::number(ui->customPlot_1->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditYValuePlot_1->setText(QString::number(ui->customPlot_1->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Gefest::on_plotChannel_2_Clicked(QMouseEvent *event) const
{
    ui->lineEditXValuePlot_2->setText(QString::number(ui->customPlot_2->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditYValuePlot_2->setText(QString::number(ui->customPlot_2->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Gefest::on_plotChannel_3_Clicked(QMouseEvent *event) const
{
    ui->lineEditXValuePlot_3->setText(QString::number(ui->customPlot_3->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditYValuePlot_3->setText(QString::number(ui->customPlot_3->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Gefest::on_plotChannel_4_Clicked(QMouseEvent *event) const
{
    ui->lineEditXValuePlot_4->setText(QString::number(ui->customPlot_4->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditYValuePlot_4->setText(QString::number(ui->customPlot_4->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Gefest::on_pushButtonStart_clicked()
{
    if (ui->pushButtonStart->text() == "Plot")
    {
        if (ui->checkBoxOnChannel_1->checkState() == Qt::Unchecked)
            return;

        this->setAttribute(Qt::WA_DeleteOnClose, true);

        ui->pushButtonStart->setText("Stop");

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
        ui->pushButtonStart->setText("Plot");

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

    return;
}

void Gefest::on_pushButtonPause_clicked()
{

    return;
}

void Gefest::on_pushButtonStop_clicked()
{

    return;
}

void Gefest::RunExperiment()
{
    this->yMin = this->yMax = -1;

    double timeStart = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double currentTime = 0;
    double value = 0;

    ui->customPlot_1->xAxis->setLabel("Time");
    ui->customPlot_1->yAxis->setLabel("Value (P || T)");
    ui->customPlot_1->clearGraphs();

    ui->customPlot_2->xAxis->setLabel("Time");
    ui->customPlot_2->yAxis->setLabel("Value (P || T)");
    ui->customPlot_2->clearGraphs();

    ui->customPlot_3->xAxis->setLabel("Time");
    ui->customPlot_3->yAxis->setLabel("Value (P || T)");
    ui->customPlot_3->clearGraphs();

    ui->customPlot_4->xAxis->setLabel("Time");
    ui->customPlot_4->yAxis->setLabel("Value (P || T)");
    ui->customPlot_4->clearGraphs();


    QString File = this->dataPath + QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss") + ".dat";
    freopen(File.toStdString().c_str(), "w+", stdout);

    bool channelChecked_1 = ui->checkBoxOnChannel_1->checkState() == Qt::Checked;
    bool channelChecked_2 = ui->checkBoxOnChannel_2->checkState() == Qt::Checked;
    bool channelChecked_3 = ui->checkBoxOnChannel_3->checkState() == Qt::Checked;
    bool channelChecked_4 = ui->checkBoxOnChannel_4->checkState() == Qt::Checked;
    bool channelChecked_5 = ui->checkBoxOnChannel_5->checkState() == Qt::Checked;

    if (channelChecked_1) {
        std::cout << "Time1\t" << ui->comboBoxChannel_1->currentText().toStdString();
    }
    if (channelChecked_2) {
        std::cout << "\tTime2\t" << ui->comboBoxChannel_2->currentText().toStdString();
    }
    if (channelChecked_3) {
        std::cout << "\tTime3\t" << ui->comboBoxChannel_3->currentText().toStdString();
    }
    if (channelChecked_4) {
        std::cout << "\tTime4\t" << ui->comboBoxChannel_4->currentText().toStdString();
    }
    if (channelChecked_5) {
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

    bool ok = true;

    while (this->run) {

        if (channelChecked_1) {
            this->srs.sendQuery(&(this->serial), channelName_1, response);
            value = response.toDouble(&ok);
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            if (ok) {
                fprintf(stdout, "%0.20e\t%0.20e", currentTime, value);
                ui->lineEditChannel_1->setText(response);
            } else {
                fprintf(stdout, "%0.20e\tFAIL", currentTime);
                ui->lineEditChannel_1->setText("LOST SIGNAL");
            }

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_2) {
            this->srs.sendQuery(&(this->serial), channelName_2, response);
            value = response.toDouble(&ok);
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            if (ok) {
                fprintf(stdout, "%0.20e\t%0.20e", currentTime, value);
                ui->lineEditChannel_2->setText(response);
            } else {
                fprintf(stdout, "%0.20e\tFAIL", currentTime);
                ui->lineEditChannel_2->setText("LOST SIGNAL");
            }

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_3) {
            this->srs.sendQuery(&(this->serial), channelName_3, response);
            value = response.toDouble(&ok);
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            if (ok) {
                fprintf(stdout, "%0.20e\t%0.20e", currentTime, value);
                ui->lineEditChannel_3->setText(response);
            } else {
                fprintf(stdout, "%0.20e\tFAIL", currentTime);
                ui->lineEditChannel_3->setText("LOST SIGNAL");
            }

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_4) {
            this->srs.sendQuery(&(this->serial), channelName_4, response);
            value = response.toDouble(&ok);
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            if (ok) {
                fprintf(stdout, "%0.20e\t%0.20e", currentTime, value);
                ui->lineEditChannel_4->setText(response);
            } else {
                fprintf(stdout, "%0.20e\tFAIL", currentTime);
                ui->lineEditChannel_4->setText("LOST SIGNAL");
            }

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }
        if (channelChecked_5) {
            this->srs.sendQuery(&(this->serial), channelName_5, response);
            value = response.toDouble(&ok);
            currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - timeStart;
            if (ok) {
                fprintf(stdout, "%0.20e\t%0.20e", currentTime, value);
                ui->lineEditChannel_5->setText(response);
            } else {
                fprintf(stdout, "%0.20e\tFAIL", currentTime);
                ui->lineEditChannel_5->setText("LOST SIGNAL");
            }

            if (yMin < 0 || value < yMin)
                yMin = value;
            if (yMax < 0 || value > yMax)
                yMax = value;
        }

        QTest::qWait(5);
        fprintf(stdout, "\n");

        if (ui->checkBoxFixedPlot_1->checkState() == Qt::Unchecked) {
            ui->customPlot_1->xAxis->setRange(0, currentTime);
            ui->customPlot_1->yAxis->setRange(yMin, yMax);
            ui->customPlot_1->replot();
        }
    }

    fclose(stdout);

    return;
}

void Gefest::on_toolButtonExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
        tr("Data Files (*.dat);; Text Files (*.txt);; All Files (*.*)"));
    ui->lineEditExportFileHeader->setText(fileName);

    return;
}

void Gefest::on_pushButtonExport_clicked()
{

    return;
}

void Gefest::on_toolButtonImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File with Data"), "",
            tr("Data Files (*.dat);; Text Files (*.txt);; All Files (*.*)"));
    ui->lineEditExportFileHeader->setText(fileName);

    return;
}

void Gefest::on_pushButtonImport_clicked()
{

    return;
}

void Gefest::on_pushButtonSetP_clicked()
{

    return;
}

void Gefest::on_pushButtonSetT_clicked()
{

    return;
}

void Gefest::loadConfig()
{
    if (freopen("ConfigGefest.conf", "r+", stdin) != nullptr) {
        std::string buffer;
        while (getline(std::cin, buffer)) {
            QString command = QString(buffer.c_str());
            if (command.toUpper().startsWith("PTC")) {
                if (command.toUpper().contains("COM")) {
                    ui->comboBoxComPorts->setCurrentText(command.split(" ")[1]);
                    if (ui->comboBoxComPorts->currentText() == command.split(" ")[1])
                        on_pushButtonAction_clicked();
                }
                if (command.toUpper().contains("MODEL")) {
                    this->old = command.toUpper().contains("OLD");
                    qDebug() << this->old;
                }
            }
            if (command.toUpper().startsWith("COM")) {
                if (command.toUpper().contains("BAUD")) {
                    this->serial.setBaudRate(command.split(" ")[1].toInt());
                }
                if (command.toUpper().contains("FC")) {
                    QString mode = command.split(" ")[1].toUpper();
                    if (mode == "NO")
                        this->serial.setFlowControl(QSerialPort::NoFlowControl);
                    if (mode == "SOFT")
                        this->serial.setFlowControl(QSerialPort::SoftwareControl);
                    if (mode == "HARD")
                        this->serial.setFlowControl(QSerialPort::HardwareControl);
                }
            }
        }
    }

    return;
}

void Gefest::saveConfig()
{

    return;
}

void Gefest::on_comboBoxChannel_1_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    if (ui->comboBoxChannel_1->currentText().isEmpty()) {
        ui->checkBoxChannel_1_Plot_1->setText("Ch 1");
        ui->checkBoxChannel_1_Plot_2->setText("Ch 1");
        ui->checkBoxChannel_1_Plot_3->setText("Ch 1");
        ui->checkBoxChannel_1_Plot_4->setText("Ch 1");
    } else {
        ui->checkBoxChannel_1_Plot_1->setText(ui->comboBoxChannel_1->currentText());
        ui->checkBoxChannel_1_Plot_2->setText(ui->comboBoxChannel_1->currentText());
        ui->checkBoxChannel_1_Plot_3->setText(ui->comboBoxChannel_1->currentText());
        ui->checkBoxChannel_1_Plot_4->setText(ui->comboBoxChannel_1->currentText());
    }

    return;
}

void Gefest::on_comboBoxChannel_2_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    if (ui->comboBoxChannel_2->currentText().isEmpty()) {
        ui->checkBoxChannel_2_Plot_1->setText("Ch 2");
        ui->checkBoxChannel_2_Plot_2->setText("Ch 2");
        ui->checkBoxChannel_2_Plot_3->setText("Ch 2");
        ui->checkBoxChannel_2_Plot_4->setText("Ch 2");
    } else {
        ui->checkBoxChannel_2_Plot_1->setText(ui->comboBoxChannel_2->currentText());
        ui->checkBoxChannel_2_Plot_2->setText(ui->comboBoxChannel_2->currentText());
        ui->checkBoxChannel_2_Plot_3->setText(ui->comboBoxChannel_2->currentText());
        ui->checkBoxChannel_2_Plot_4->setText(ui->comboBoxChannel_2->currentText());
    }

    return;
}

void Gefest::on_comboBoxChannel_3_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    if (ui->comboBoxChannel_3->currentText().isEmpty()) {
        ui->checkBoxChannel_3_Plot_1->setText("Ch 3");
        ui->checkBoxChannel_3_Plot_2->setText("Ch 3");
        ui->checkBoxChannel_3_Plot_3->setText("Ch 3");
        ui->checkBoxChannel_3_Plot_4->setText("Ch 3");
    } else {
        ui->checkBoxChannel_3_Plot_1->setText(ui->comboBoxChannel_3->currentText());
        ui->checkBoxChannel_3_Plot_2->setText(ui->comboBoxChannel_3->currentText());
        ui->checkBoxChannel_3_Plot_3->setText(ui->comboBoxChannel_3->currentText());
        ui->checkBoxChannel_3_Plot_4->setText(ui->comboBoxChannel_3->currentText());
    }

    return;
}

void Gefest::on_comboBoxChannel_4_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    if (ui->comboBoxChannel_4->currentText().isEmpty()) {
        ui->checkBoxChannel_4_Plot_1->setText("Ch 4");
        ui->checkBoxChannel_4_Plot_2->setText("Ch 4");
        ui->checkBoxChannel_4_Plot_3->setText("Ch 4");
        ui->checkBoxChannel_4_Plot_4->setText("Ch 4");
    } else {
        ui->checkBoxChannel_4_Plot_1->setText(ui->comboBoxChannel_4->currentText());
        ui->checkBoxChannel_4_Plot_2->setText(ui->comboBoxChannel_4->currentText());
        ui->checkBoxChannel_4_Plot_3->setText(ui->comboBoxChannel_4->currentText());
        ui->checkBoxChannel_4_Plot_4->setText(ui->comboBoxChannel_4->currentText());
    }

    return;
}

void Gefest::on_comboBoxChannel_5_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    if (ui->comboBoxChannel_5->currentText().isEmpty()) {
        ui->checkBoxChannel_5_Plot_1->setText("Ch 5");
        ui->checkBoxChannel_5_Plot_2->setText("Ch 5");
        ui->checkBoxChannel_5_Plot_3->setText("Ch 5");
        ui->checkBoxChannel_5_Plot_4->setText("Ch 5");
    } else {
        ui->checkBoxChannel_5_Plot_1->setText(ui->comboBoxChannel_5->currentText());
        ui->checkBoxChannel_5_Plot_2->setText(ui->comboBoxChannel_5->currentText());
        ui->checkBoxChannel_5_Plot_3->setText(ui->comboBoxChannel_5->currentText());
        ui->checkBoxChannel_5_Plot_4->setText(ui->comboBoxChannel_5->currentText());
    }

    return;
}
