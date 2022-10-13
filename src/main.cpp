#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>
#include <QtConcurrent>

#include <include/HttpServerHandler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpServerHandler handler;
    handler.startServer();

    return a.exec();
}
