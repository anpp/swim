#include "swimwindow.h"

SwiMWindow::SwiMWindow(QWidget *parent)
    : QFrame(parent)
{
    process = new QProcess(this);
    createControls();
    getInterfaces();

    connect(cbxInterfaces, SIGNAL(currentIndexChanged (const QString &)), SLOT(slotInterfaceSelected(const QString &)));

    cbxInterfaces->setCurrentIndex(cbxInterfaces->findText(interface));

    this->resize(DEF_WIDTH, DEF_HEIGHT);
    this->setWindowIcon(QIcon(":/icons/wifi_manager.png"));
    QApplication::setStyle("plastique");
}

SwiMWindow::~SwiMWindow()
{
  delete cbxInterfaces;
  delete tableAPs;
  delete lErr;
  delete lInterface;
  delete process;
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
    lTop = new QHBoxLayout();
    lMain = new QVBoxLayout(this);

    tableAPs = new QTableWidget();
    tableAPs->setFrameStyle(QFrame::NoFrame);
    tableAPs->setStyleSheet("QTableWidget{selection-background-color: transparent; background-color: transparent}");


    cbxInterfaces = new QComboBox();
    lErr = new QLabel();
    lInterface = new QLabel(tr("Interface: "));
    lblList = new QLabel(tr("Networks list"));

    lTop->addWidget(lInterface);
    lTop->addWidget(cbxInterfaces);
    lTop->addStretch();

    lMain->addLayout(lTop);
    lMain->addSpacing(SPACING);
    lMain->addWidget(lblList);
    lMain->addWidget(tableAPs);
    lMain->addWidget(lErr);
}

QString SwiMWindow::connectedSSID()
{
    QStringList sl;
    QString ssid, status, inet, return_ssid = "";

    if(!runCommand("ifconfig " + interface))
    {
        setError(s_err);
        return return_ssid;
    }

    sl = s_output.split("\n");

    ssid = removeChar(sl.at(SSID_STRING), '\t');
    status = removeChar(sl.at(STATUS_STRING), '\t');
    inet = removeChar(sl.at(INET_STRING), '\t');

    sl = ssid.split(" ");

    if(status == "status: associated")
    {
        return_ssid = sl.at(1);
        sl = inet.split(" ");
        connected_IP = sl.at(1);
    }
    return return_ssid;
}

QString SwiMWindow::removeChar(const QString &s, const QChar& c)
{
    QString res = "";
    for(QChar ch: s)
    {
        if(ch != c)
            res.push_back(ch);
    }
    return res;
}

void SwiMWindow::setActiveAP(const QString &ssid)
{
    for(APWidget* apw: aps)
        apw->setConnected(apw->getSSID() == ssid);
}


bool SwiMWindow::runCommand(const QString& sCommand)
{
    s_err = "";
    s_output = "";
    if(process == nullptr)
        return false;
    process->start(sCommand);
    process->waitForFinished();

    s_err = process->readAllStandardError();
    s_output = process->readAllStandardOutput();
    return (s_err == "" || s_err.isEmpty());
}

void SwiMWindow::setInterface(const QString& iface)
{
    APWidget *apwidget;
    QStringList sl;
    interface = iface;

    setError("");

    for(APWidget* apw: aps)
        delete apw;
    aps.clear();
    tableAPs->setRowCount(0);

    tableAPs->setColumnCount(1);
    tableAPs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableAPs->verticalHeader()->hide();
    tableAPs->horizontalHeader()->hide();

    if(!runCommand("ifconfig -v " + interface + " list scan"))
        setError(s_err);
    else
    {
        sl = s_output.split("\n");
        sl.pop_front(); //первую строку убираем, в ней заголовок

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
        for(APWidget* apw: aps)
        {
            tableAPs->setColumnWidth(0, tableAPs->width());
            tableAPs->setRowHeight(i, apw->height());

            apw->resize(tableAPs->width(), apw->height());

            tableAPs->setCellWidget(i++, 0, apw);

        }
        setActiveAP(connected_SSID);
        setError(tr("Connected to ") + connected_SSID + " (IP: " + connected_IP + ")");
    }
}

void SwiMWindow::resizeEvent (QResizeEvent *event)
{
    this->QWidget::resizeEvent(event);
    tableAPs->setColumnWidth(0, tableAPs->width());
    for(int i = 0; i < tableAPs->rowCount(); ++i)
        tableAPs->cellWidget(i, 0)->resize(tableAPs->width(), tableAPs->cellWidget(i, 0)->height());
}

