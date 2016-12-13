#ifndef APWIDGET_H
#define APWIDGET_H

#include <QFrame>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPixmap>
#include <math.h>

#include <QDebug>

enum awsizes: int {AW_WIDTH = 200, AW_HEIGHT = 80};
enum APconsts: int {CSSID = 0, CMAC = 1, CSN = 4};

class APWidget : public QFrame
{
    Q_OBJECT

    QTextEdit *eSSID;
    QTextEdit *eMAC;
    QHBoxLayout *lMain;
    QVBoxLayout *l0;
    QVBoxLayout *l1;
    QHBoxLayout *lSSID;
    QHBoxLayout *lMAC;
    QLabel *lblSSID;
    QLabel *lblIcon;
    QLabel *lblMAC;
    QLabel *lblPercent;
    QCheckBox *cbxAuto;
    QLabel *lblConnect;

    QString SSID;
    int percent = 0;
    bool connected = false;

    void setPercent(const int percent);
    int percentFromRAW(const QString& raw);
    void setIcon();
    void setIconConnect();
public:
    explicit APWidget(QWidget *parent = 0);
    ~APWidget();

    void setAPInfo(const QString& sInfo);
    const QString& getSSID() {return SSID;}
    void setConnected(bool connected);
signals:

public slots:
};


#endif // APWIDGET_H
