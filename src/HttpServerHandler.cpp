#include "include/HttpServerHandler.h"


HttpServerHandler::HttpServerHandler(QObject *parent)
    : QObject{parent}
{

    assert(setSslCertificate(pathToCertificate));
    assert(setSslPrivateKey(pathToPrivateKey));
    server.sslSetup(sslCertificate, sslPrivateKey);

    server.route("/", [] () {
        return "Index";
    });

    server.route("/register", QHttpServerRequest::Method::Get, [&] (const QHttpServerRequest &request) {
        QUrlQuery queryParameters = request.query();

        QFile registerPage(pathToRegisterPage);
        if (!registerPage.open(QIODevice::ReadOnly)) {
            return QHttpServerResponse("Internal error", QHttpServerResponse::StatusCode::InternalServerError);
        }

        return QHttpServerResponse(QString::fromStdString(registerPage.readAll().toStdString()));
    });

    server.route("/register", QHttpServerRequest::Method::Post, [&] (const QHttpServerRequest &request) {
        QUrlQuery queryParameters = request.query();

        QUrlQuery postParameters(request.body());

        foreach(const auto &item, postParameters.queryItems()) {
            qDebug() << item.first << QUrl(item.second).path();
        }

        return "Спасибо!";
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
