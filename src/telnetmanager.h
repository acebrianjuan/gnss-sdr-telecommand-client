#ifndef TELNETMANAGER_H
#define TELNETMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>


class TelnetManager : public QObject
{
    Q_OBJECT

public:
    enum class Command
    {
        Reset,
        Standby,
        ColdStart,
        WarmStart,
        HotStart,
        Status,
        Exit
    };

    explicit TelnetManager(QObject *parent = nullptr);

    void set_address(QString addr_str);
    void set_port(QString port_str);

    QHostAddress get_address() const;
    quint16 get_port() const;
    QAbstractSocket::SocketState get_state() const;

signals:
    void tx_data(QByteArray data);
    void rx_data(QByteArray data);
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState socketState);

public slots:
    void connect_tcp();
    void disconnect_tcp();
    bool send_command(Command cmd, QString args = "");
    void read_response();

private:
    QTcpSocket *tcp_socket;
    QHostAddress address;
    quint16 port;
};

#endif // TELNETMANAGER_H
