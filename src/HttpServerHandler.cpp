#include "include/HttpServerHandler.h"

HttpServerHandler::HttpServerHandler(QObject *parent)
    : QObject{parent}
{
    server.route("/", [] () {
        return "Index";
    });
    server.route("/dichotomy", [] (const QHttpServerRequest &request) {
        QUrlQuery queryParams = request.query();

        return "Welcome";
   });
}


short HttpServerHandler::getServerPort() const
{
    return serverPort;
}

void HttpServerHandler::setServerPort(short newServerPort)
{
    serverPort = newServerPort;
}

void HttpServerHandler::startServer()
{
    short newPort = server.listen(QHostAddress::Any, serverPort);
    assert(newPort == serverPort);
    qDebug() << "Server started";
    emit serverStarted();
}
