#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>

#include <include/DatabaseConnector.h>
#include <include/HttpServerHandler.h>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThreadPool databasePool;
    databasePool.setMaxThreadCount(5);


    HttpServerHandler handler(&databasePool);

    handler.startServer();

    return a.exec();
}
