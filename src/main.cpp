#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("gnss-sdr");
    app.setOrganizationDomain("gnss-sdr.org");
    app.setApplicationName("gnss-sdr-telecommand-client");

    MainWindow w;

    QStringList args = app.arguments();
    if (args.count() == 3)
    {
        w.set_address(args[1]);
        w.set_port(args[2]);

        // Attempt connection.
        w.connect_clicked();
    }

    w.show();

    return app.exec();
}
