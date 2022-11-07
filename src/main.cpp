#include <QCoreApplication>
#include <QHttpServer>
#include <QDebug>

#include <include/DatabaseConnector.h>
#include <include/HttpServerHandler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThreadPool databasePool;
    databasePool.setMaxThreadCount(DatabaseConnector::MAX_DATABASE_CONNECTIONS_COUNT);

    for(int i = 0; i < databasePool.maxThreadCount(); i++) {
        (void) QtConcurrent::run(&databasePool, [] () {
            DatabaseConnector();
        });
    }

    HttpServerHandler handler(&databasePool);

    handler.startServer();

    return a.exec();
}
