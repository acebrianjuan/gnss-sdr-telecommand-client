#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTcpSocket>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include "telnetmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void inputs_changed();
    void received_data(QByteArray data);

public slots:
    void on_address_edit_textChanged();
    void on_port_edit_textChanged();
    void handle_inputs_changed();

    void reset_clicked();
    void standby_clicked();
    void coldstart_clicked();
    void warmstart_clicked();
    void hotstart_clicked();
    void status_clicked();
    void connect_clicked();

    void print_text(QByteArray data);

    void set_address(QString ip);
    void set_port(QString port);

    void status_connected();
    void status_disconnected();

    void save_settings();
    void load_settings();

    QString get_args();

private:
    Ui::MainWindow *ui;
    TelnetManager telnet_manager;
    QLabel *status_label;
    QSettings settings;
};

#endif // MAINWINDOW_H
