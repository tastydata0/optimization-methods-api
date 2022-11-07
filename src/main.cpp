#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>

#include <include/DatabaseConnector.h>
#include <include/HttpServerHandler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    DatabaseConnector connector;

    HttpServerHandler handler(&connector);

    handler.startServer();
    assert(connector.connect());

    QObject::connect(&a, &QCoreApplication::aboutToQuit, &connector, &DatabaseConnector::disconnect);

    return a.exec();
}
