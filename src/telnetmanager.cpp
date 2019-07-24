#include "telnetmanager.h"

TelnetManager::TelnetManager(QObject *parent) : QObject(parent)
{
    tcp_socket = new QTcpSocket(this);
    connect(tcp_socket, &QIODevice::readyRead, this, &TelnetManager::read_response);
    connect(tcp_socket, &QAbstractSocket::connected, this, &TelnetManager::connected);
    connect(tcp_socket, &QAbstractSocket::disconnected, this, &TelnetManager::disconnected);
    connect(tcp_socket, &QAbstractSocket::stateChanged, this, &TelnetManager::stateChanged);
}

void TelnetManager::set_address(QString addr_str)
{
    address.setAddress(addr_str);
}

void TelnetManager::set_port(QString port_str)
{
    port = port_str.toInt();
}

QHostAddress TelnetManager::get_address() const
{
    return address;
}

quint16 TelnetManager::get_port() const
{
    return port;
}

QAbstractSocket::SocketState TelnetManager::get_state() const
{
    return tcp_socket->state();
}

void TelnetManager::connect_tcp()
{
    tcp_socket->connectToHost(address, port);
}

void TelnetManager::disconnect_tcp()
{
    tcp_socket->disconnectFromHost();
}

bool TelnetManager::send_command(Command cmd, QString args)
{
    if(tcp_socket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;

        switch (cmd)
        {
        case Command::Reset: data = "reset\r\n"; break;
        case Command::Standby: data = "standby\r\n"; break;
        case Command::ColdStart: data = "coldstart\r\n"; break;
        case Command::WarmStart: data = QString("%1 %2 %3").arg("warmstart", args, "\r\n").toUtf8(); break;
        case Command::HotStart: data = QString("%1 %2 %3").arg("hotstart", args, "\r\n").toUtf8(); break;
        case Command::Status: data = "status\r\n"; break;
        case Command::Exit: data = "exit\r\n"; break;
        default: data = ""; break;
        }

        if (!data.isEmpty())
        {
            tcp_socket->write(data, data.size());

            if (tcp_socket->waitForBytesWritten())
            {
                emit tx_data(data);
                return true;
            }
        }
    }
    return false;
}

void TelnetManager::read_response()
{
    QByteArray data = tcp_socket->readAll();
    emit rx_data(data);
}
