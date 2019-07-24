#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->connect_btn->setText("Connect");
    ui->connect_btn->setEnabled(false);
    ui->textEdit->setPlaceholderText("The sent and received messages will be shown here...");
    ui->dateTimeEdit->setDate(QDate::currentDate());

    status_label = new QLabel(this);
    ui->statusBar->addPermanentWidget(status_label);

    connect(ui->address_edit, &QLineEdit::textChanged, this, &MainWindow::on_address_edit_textChanged);
    connect(ui->port_edit, &QLineEdit::textChanged, this, &MainWindow::on_port_edit_textChanged);
    connect(this, &MainWindow::inputs_changed, this, &MainWindow::handle_inputs_changed);

    connect(ui->reset_btn, &QPushButton::clicked, this, &MainWindow::reset_clicked);
    connect(ui->standby_btn, &QPushButton::clicked, this, &MainWindow::standby_clicked);
    connect(ui->coldstart_btn, &QPushButton::clicked, this, &MainWindow::coldstart_clicked);
    connect(ui->warmstart_btn, &QPushButton::clicked, this, &MainWindow::warmstart_clicked);
    connect(ui->hotstart_btn, &QPushButton::clicked, this, &MainWindow::hotstart_clicked);
    connect(ui->status_btn, &QPushButton::clicked, this, &MainWindow::status_clicked);
    connect(ui->connect_btn, &QPushButton::clicked, this, &MainWindow::connect_clicked);

    connect(&telnet_manager, &TelnetManager::tx_data, this, &MainWindow::print_text);
    connect(&telnet_manager, &TelnetManager::rx_data, this, &MainWindow::print_text);
    connect(&telnet_manager, &TelnetManager::connected, this, &MainWindow::status_connected);
    connect(&telnet_manager, &TelnetManager::disconnected, this, &MainWindow::status_disconnected);

    // Load settings from last session.
    load_settings();

    handle_inputs_changed();
}

MainWindow::~MainWindow()
{
    telnet_manager.disconnect_tcp();

    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();

    delete ui;
}

void MainWindow::set_address(QString addr_str)
{
    telnet_manager.set_address(addr_str);
    ui->address_edit->setText(addr_str);
}

void MainWindow::set_port(QString port_str)
{
    telnet_manager.set_port(port_str);
    ui->port_edit->setText(port_str);
}

void MainWindow::handle_inputs_changed()
{
    if (ui->address_edit->text().isEmpty() || ui->port_edit->text().isEmpty())
    {
        ui->connect_btn->setEnabled(false);
        ui->connect_btn->setStyleSheet(styleSheet());
    }
    else
    {
        ui->connect_btn->setEnabled(true);
        ui->connect_btn->setStyleSheet("background-color:#2ECC40;");
    }
}

void MainWindow::on_address_edit_textChanged()
{
    emit inputs_changed();
}

void MainWindow::on_port_edit_textChanged()
{
    emit inputs_changed();
}

void MainWindow::reset_clicked()
{
    telnet_manager.send_command(TelnetManager::Command::Reset);
}

void MainWindow::standby_clicked()
{
    telnet_manager.send_command(TelnetManager::Command::Standby);
}

void MainWindow::coldstart_clicked()
{
    telnet_manager.send_command(TelnetManager::Command::ColdStart);
}

void MainWindow::warmstart_clicked()
{
    telnet_manager.send_command(TelnetManager::Command::WarmStart, get_args());
}

void MainWindow::hotstart_clicked()
{
    telnet_manager.send_command(TelnetManager::Command::HotStart, get_args());
}

void MainWindow::status_clicked()
{
    telnet_manager.send_command(TelnetManager::Command::Status);
}

void MainWindow::connect_clicked()
{
    // What's the state of the socket?
    QAbstractSocket::SocketState state = telnet_manager.get_state();

    if (state == QAbstractSocket::ConnectedState)
    {
        // The socket was connected. So let's disconnect now.
        telnet_manager.send_command(TelnetManager::Command::Exit);
    }
    else if (state == QAbstractSocket::UnconnectedState)
    {
        // The socket was disconnected. So let's attempt a connection now.
        set_address(ui->address_edit->text());
        set_port(ui->port_edit->text());
        telnet_manager.connect_tcp();
    }
}

void MainWindow::status_connected()
{
    // The connection was successful so let's save the address and port number.
    QSettings settings;
    settings.beginGroup("tcp_socket");
    settings.setValue("address", telnet_manager.get_address().toString());
    settings.setValue("port", telnet_manager.get_port());
    settings.endGroup();

    qDebug() << "Settings Saved";

    // Disable the ip address and port number edit fields.
    ui->address_edit->setEnabled(false);
    ui->port_edit->setEnabled(false);

    ui->connect_btn->setText("exit");
    ui->connect_btn->setStyleSheet("background-color:#FF4136;");

    status_label->setText("Connected");
}

void MainWindow::status_disconnected()
{
    ui->address_edit->setEnabled(true);
    ui->port_edit->setEnabled(true);

    ui->connect_btn->setText("Connect");
    ui->connect_btn->setStyleSheet("background-color:#2ECC40;");

    status_label->setText("Disconnected");
}

void MainWindow::print_text(QByteArray data)
{
    QString text(data);
    ui->textEdit->insertPlainText(text);
    if(ui->action_auto_scroll->isChecked())
    {
        ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
    }
}

void MainWindow::load_settings()
{
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();

    settings.beginGroup("tcp_socket");
    set_address(settings.value("address", "").toString());
    set_port(settings.value("port", "").toString());
    settings.endGroup();

    qDebug() << "Settings Loaded";
}

QString MainWindow::get_args()
{
    QString args = QString("%1 %2 %3 %4").arg(
                ui->dateTimeEdit->text(),
                ui->latitude_spinBox->text(),
                ui->longitude_spinBox->text(),
                ui->height_spinBox->text());

    return args;
}

void MainWindow::save_settings()
{
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();

    settings.beginGroup("tcp_socket");
    settings.setValue("address", telnet_manager.get_address().toString());
    settings.setValue("port", telnet_manager.get_port());
    settings.endGroup();

    qDebug() << "Settings Saved";
}
