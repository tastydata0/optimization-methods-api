#include "include/HttpServerHandler.h"


HttpServerHandler::HttpServerHandler(QObject *parent)
    : QObject{parent}
{
    server.route("/", [] () {
        return "Index";
    });

    server.route("/dichotomy", QHttpServerRequest::Method::Get, [&] (const QHttpServerRequest &request) {
        QUrlQuery queryParameters = request.query();

        DichotomySolver solver;

        return processRequest(&solver, queryParameters);
    });
}

QHttpServerResponse HttpServerHandler::processRequest(AbstractSolver *solver, const QUrlQuery &queryParameters)
{
    QJsonDocument output = solver->solve(solver->urlQueryToMap(queryParameters));

    return QHttpServerResponse(output.object());
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
