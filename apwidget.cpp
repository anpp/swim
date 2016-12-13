#include "apwidget.h"

APWidget::APWidget(QWidget *parent) : QFrame(parent)
{
    lMain = new QHBoxLayout(this);
    l0 = new QVBoxLayout();
    l1 = new QVBoxLayout();
    lSSID = new QHBoxLayout();
    lMAC = new QHBoxLayout();

    l0->setMargin(0);
    l0->setSpacing(0);
    l1->setMargin(0);
    l1->setSpacing(0);

    eSSID = new QTextEdit();
    eSSID->setReadOnly(true);
    eMAC = new QTextEdit();
    eMAC->setReadOnly(true);

    lblSSID = new QLabel("SSID:");
    lblMAC = new QLabel("MAC:");
    lblPercent = new QLabel("0%");

    cbxAuto = new QCheckBox(tr("Auto connect to this network"));
    lblIcon = new QLabel();
    lblConnect = new QLabel();
    lblConnect->setFixedWidth(32);

    lblSSID->setStyleSheet("font: bold");
    lblMAC->setStyleSheet("font: bold");
    //eSSID->setFrameStyle(QFrame::NoFrame);
    eSSID->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    eSSID->setStyleSheet("QTextEdit{background-color: transparent}");
    //eMAC->setFrameStyle(QFrame::NoFrame);
    eMAC->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    eMAC->setStyleSheet("QTextEdit{background-color: transparent}");

    lSSID->addWidget(lblSSID);
    lSSID->addWidget(eSSID);

    lMAC->addWidget(lblMAC);
    lMAC->addWidget(eMAC);

    l0->addWidget(lblIcon);
    l0->addWidget(lblPercent);

    l1->addLayout(lSSID);
    l1->addLayout(lMAC);
    l1->addWidget(cbxAuto);

    lMain->addLayout(l0);
    lMain->addWidget(lblConnect);
    lMain->addLayout(l1);
    lMain->addStretch();

    setIcon();
    setIconConnect();

    this->resize(AW_WIDTH, AW_HEIGHT);
}

APWidget::~APWidget()
{
  delete eSSID;
  delete lSSID;
  delete eMAC;
  delete lblMAC;
  delete lblPercent;
  delete lblConnect;
}

void APWidget::setAPInfo(const QString& sInfo)
{
    if(sInfo == "" || sInfo.isEmpty())
        return;
    int i = 0;
    QStringList sl;
    sl = sInfo.split(" ");

    for(QStringList::iterator isl = sl.begin(); isl != sl.end(); ++isl)
    {
        if(*isl == "")
        {
            sl.erase(isl);
            continue;
        }
        if((i++ == 1) && (*isl).length() != 17)//на случай, если точка доступа имеет название с пробелом
        {
            *(isl - 1) = *(isl - 1) + " " + *isl;
            sl.erase(isl);
        }
    }

    if(sl.count() > 0)
        SSID = sl.at(CSSID);

    //if(SSID == "ANP-ZyXEL")
    //    qDebug() << sl;

    eSSID->setText(SSID);
    eMAC->setText(sl.at(CMAC));

    setPercent(percentFromRAW(sl.at(CSN)));
}

void APWidget::setConnected(bool connected)
{
    this->connected = connected;
    setIconConnect();
}


void APWidget::setPercent(const int percent)
{
    this->percent = percent;
    lblPercent->setText("<CENTER>" + QString::number(percent) + "%" + "</CENTER>");
    setIcon();
}

int APWidget::percentFromRAW(const QString &raw)
{
    QStringList sl = raw.split(":");
    if(sl.length() != 2)
        return 0;
    return int(fmin((sl.at(0).toInt() - sl.at(1).toInt()) * 4.0, 100.0));
}

void APWidget::setIcon()
{
    QString sIcon = "ns-0.png";
    if(percent > 0)
    {
        if(percent >= 1)
            sIcon = "ns-0-24.png";
        if(percent >= 25)
            sIcon = "ns-25-49.png";
        if(percent >= 50)
            sIcon = "ns-50-89.png";
        if(percent >= 90)
            sIcon = "ns-90-100.png";
    }
    QPixmap pxm;
    pxm.load(":/icons/" + sIcon);
    lblIcon->setPixmap(pxm);
}

void APWidget::setIconConnect()
{
    delete lblConnect->pixmap();
    QPixmap pxm;
    if(!connected) return;
    pxm.load(":/icons/connected.png");
    lblConnect->setPixmap(pxm);
}



