#include "include/HttpServerHandler.h"


HttpServerHandler::HttpServerHandler(QObject *parent)
    : QObject{parent}
{

    assert(setSslCertificate(pathToCertificate));
    assert(setSslPrivateKey(pathToPrivateKey));
    server.sslSetup(sslCertificate, sslPrivateKey, QSsl::AnyProtocol);

    server.route("/", [] () {
        return "Index";
    });

    server.route("/register", QHttpServerRequest::Method::Get, [&] (const QHttpServerRequest &request) {
        QUrlQuery queryParameters = request.query();

        return "TODO";
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

bool HttpServerHandler::setSslCertificate(const QString& path, QSsl::EncodingFormat format)
{
    QFile certificateFile(path);
    if (!certificateFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    sslCertificate = QSslCertificate(certificateFile.readAll(), format);
    return true;
}

bool HttpServerHandler::setSslPrivateKey(const QString& path, const QByteArray& password, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format)
{
    QFile keyFile(path);
    if (!keyFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    sslPrivateKey = QSslKey(keyFile.readAll(), algorithm, format, QSsl::PrivateKey, password);
    return true;
}

void HttpServerHandler::startServer()
{
    short newPort = server.listen(QHostAddress::Any, serverPort);
    assert(newPort == serverPort);
    qDebug() << "Server started";
    emit serverStarted();
}
