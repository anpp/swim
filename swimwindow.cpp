#include "swimwindow.h"

SwiMWindow::SwiMWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createControls();
    getInterfaces();

    connect(cbxInterfaces, SIGNAL(currentIndexChanged (const QString &)), SLOT(slotInterfaceSelected(const QString &)));
    connect(btnRefresh, SIGNAL(clicked()), SLOT(slotRefresh()));
    cbxInterfaces->setCurrentIndex(cbxInterfaces->findText(interface));

    this->setWindowIcon(QIcon(":/icons/wifi_manager.png"));
    QApplication::setStyle("plastique");
    settings.loadSettings();
}

SwiMWindow::~SwiMWindow()
{
    settings.saveSettings();
    delete cbxInterfaces;
    delete tableAPs;
    delete lErr;
    delete lInterface;
    delete mainFrame;
}

void SwiMWindow::getInterfaces()
{
    runCommand("ifconfig -l");
    if(s_output.length() > 1)
        s_output.remove(s_output.length() - 1, 1);
    cbxInterfaces->addItems(s_output.split(" "));
}


void SwiMWindow::createControls()
{
    mainFrame = new QFrame(this);
    lTop = new QHBoxLayout();
    lMain = new QVBoxLayout(mainFrame);

    btnRefresh = new QPushButton(tr("Refresh"));
    tableAPs = new QTableWidget();
    tableAPs->setFrameStyle(QFrame::NoFrame);
    tableAPs->setStyleSheet("QTableWidget{selection-background-color: transparent; background-color: transparent; border-top: 1px solid gray}");
    tableAPs->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);


    cbxInterfaces = new QComboBox();
    lErr = new QLabel();
    lInterface = new QLabel(tr("Interface: "));
    lblList = new QLabel(tr("Networks list"));

    lTop->addWidget(lInterface);
    lTop->addWidget(cbxInterfaces);
    lTop->addWidget(btnRefresh);
    lTop->addStretch();

    lMain->addLayout(lTop);
    lMain->addSpacing(SPACING);
    lMain->addWidget(lblList);
    lMain->addWidget(tableAPs);
    mainFrame->setLayout(lMain);

    this->setCentralWidget(mainFrame);
    this->statusBar()->addWidget(lErr);
}

QString SwiMWindow::connectedSSID()
{
    QString ssid, status, inet, return_ssid = "";

    if(!runCommand("ifconfig " + interface, "-a ssid"))
    {
        setError(s_err);
        return return_ssid;
    }
    ssid = removeChar(removeChar(s_output, '\t'), '\n');
    if(!runCommand("ifconfig " + interface, "-a status"))
    {
        setError(s_err);
        return return_ssid;
    }
    status = removeChar(removeChar(s_output, '\t'), '\n');
    if(!runCommand("ifconfig " + interface, "-a inet"))
    {
        setError(s_err);
        return return_ssid;
    }
    inet = removeChar(removeChar(s_output, '\t'), '\n');

    if(status == "status: associated")
    {
        if(ssid.contains(' '))
            return_ssid = ssid.split(" ").at(1);
        if(inet.contains(' '))
            connected_IP = inet.split(" ").at(1);
    }
    return return_ssid;
}

QString SwiMWindow::removeChar(const QString &s, const QChar& c)
{
    QString res = "";
    for(auto ch: s)
    {
        if(ch != c)
            res.push_back(ch);
    }
    return res;
}

void SwiMWindow::setActiveAP(const QString &ssid)
{
    for(auto apw: aps)
        apw->setConnected(apw->getSSID() == ssid);
}


bool SwiMWindow::runCommand(const QString& sCommand, const QString& grepArgs)
{
    QProcess process;
    QProcess grepProcess;
    s_err = "";
    s_output = "";
    QProcess *outputProcess = &process;

    if(grepArgs != "")
    {
        process.setStandardOutputProcess(&grepProcess);
        outputProcess = &grepProcess;
    }
    process.start(sCommand);
    process.waitForFinished();

    if(outputProcess != &process)
    {
        grepProcess.start("grep " + grepArgs);
        grepProcess.waitForFinished();
    }

    s_err = removeChar(process.readAllStandardError(), '\n');
    s_output = outputProcess->readAllStandardOutput();
    return (s_err == "" || s_err.isEmpty());
}

void SwiMWindow::setInterface(const QString& iface)
{
    APWidget *apwidget;
    QStringList sl;
    interface = iface;

    setError("");

    for(auto apw: aps)
        delete apw;
    aps.clear();
    tableAPs->setRowCount(0);

    tableAPs->setColumnCount(1);
    tableAPs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableAPs->verticalHeader()->hide();
    tableAPs->horizontalHeader()->hide();

    if(!runCommand("ifconfig -v " + interface + " list scan", "-va BSSID"))
        setError(s_err);
    else
    {
        sl = s_output.split("\n");
        connected_SSID = connectedSSID();
        for(QString s: sl)
        {
            if(s == "") continue;
            apwidget = new APWidget();
            apwidget->setAPInfo(s);
            if(apwidget->getSSID() == connected_SSID)
                aps.push_front(apwidget);
            else
                aps.push_back(apwidget);
        }
        tableAPs->setRowCount(aps.count());
        int i = 0;
        for(auto apw: aps)
        {
            tableAPs->setRowHeight(i, apw->height());
            tableAPs->setCellWidget(i++, 0, apw);
        }
        setActiveAP(connected_SSID);
        setError(tr("Connected to ") + connected_SSID + " (IP: " + connected_IP + ")");
    }
}

void SwiMWindow::resizeEvent (QResizeEvent *event)
{
    this->QWidget::resizeEvent(event);
    resizeTable();
}

void SwiMWindow::showEvent(QShowEvent *event)
{
    this->QWidget::showEvent(event);
    resizeTable();
}


void SwiMWindow::resizeTable()
{
    tableAPs->setColumnWidth(0, tableAPs->width());
    for(auto apw: aps)
        apw->resize(tableAPs->width(), apw->height());
}
