#ifndef SWIMWINDOW_H
#define SWIMWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QProcess>
#include <QStringList>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFrame>
#include <QHeaderView>
#include <QTableWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QVector>
#include <QStatusBar>
#include <QPushButton>
#include "apwidget.h"
#include "settings.h"

enum sizes: int {DEF_WIDTH = 650, DEF_HEIGHT = 360, SPACING = 5};
enum ifconsts: int {INET_STRING = 2, STATUS_STRING = 5, SSID_STRING = 6};

class SwiMWindow : public QMainWindow
{
    Q_OBJECT

    QComboBox *cbxInterfaces = nullptr;

    QFrame *mainFrame;
    QString interface = "wlan0";
    QString s_err = "";
    QString s_output = "";
    QString connected_IP = "";
    QString connected_SSID = "";

    QVBoxLayout *lMain;
    QHBoxLayout *lTop;
    QTableWidget *tableAPs;
    QLabel *lErr;
    QLabel *lInterface;
    QLabel *lblList;
    QPushButton *btnRefresh;

    QVector<APWidget*> aps;
    Settings settings{this, "swim", "swim"};

    void getInterfaces();
    void setInterface(const QString& iface);
    bool runCommand(const QString& sCommand, const QString& grepArgs = "");
    void setError(const QString& s) {lErr->setText(s);}
    void createControls();
    QString connectedSSID();
    QString removeChar(const QString& s, const QChar& c);
    void setActiveAP(const QString& ssid);

protected:
    void resizeEvent (QResizeEvent *event);
public:
    SwiMWindow(QWidget *parent = 0);
    ~SwiMWindow();

public slots:
    void slotInterfaceSelected(const QString& interface) { setInterface(interface);}
    void slotRefresh() {setInterface(interface);}
};

#endif // SWIMWINDOW_H
