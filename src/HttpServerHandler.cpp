#include "include/HttpServerHandler.h"



HttpServerHandler::HttpServerHandler(DatabaseConnector *databaseConnector, QObject *parent) : QObject(parent),
    databaseConnector(databaseConnector)
{
    assert(setSslCertificate(pathToCertificate));
    assert(setSslPrivateKey(pathToPrivateKey));
    server.sslSetup(sslCertificate, sslPrivateKey);

    server.route("/", [] () {
        return "Index";
    });

    server.route("/register", QHttpServerRequest::Method::Get, [&] (const QHttpServerRequest &request) {

        QFile registerPage(pathToRegisterPage);
        if (!registerPage.open(QIODevice::ReadOnly)) {
            return internalErrorResponse;
        }

        return QHttpServerResponse(QString::fromStdString(registerPage.readAll().toStdString()));
    });

    server.route("/register", QHttpServerRequest::Method::Post, [&] (const QHttpServerRequest &request) {

        QUrlQuery postParameters(request.body());

        QHash<QString, QString> parameters;

        foreach(const auto &item, postParameters.queryItems()) {
            parameters[item.first] = QUrl(item.second).path();
        }


        if(!parameters.contains("login")) {
            return QHttpServerResponse("Request must contain \"login\"", QHttpServerResponse::StatusCode::BadRequest);
        }
        else if(!parameters.contains("psw")) {
            return QHttpServerResponse("Request must contain \"psw\" (password)", QHttpServerResponse::StatusCode::BadRequest);
        }

        DatabaseConnector::REGISTER_USER_RESULT registerUserResult = databaseConnector->registerUser(parameters["login"], parameters["psw"]);

        switch (registerUserResult) {
            case DatabaseConnector::REGISTER_USER_RESULT::OK:
                return QHttpServerResponse("Success!", QHttpServerResponse::StatusCode::Ok);
                break;
            case DatabaseConnector::REGISTER_USER_RESULT::USER_EXISTS:
                return QHttpServerResponse("User exists.", QHttpServerResponse::StatusCode::BadRequest);
                break;
            case DatabaseConnector::REGISTER_USER_RESULT::INTERNAL_ERROR:
                return internalErrorResponse;
                break;
        }
    });


    server.route("/login", QHttpServerRequest::Method::Get, [&] (const QHttpServerRequest &request) {

        QFile registerPage(pathToLoginPage);
        if (!registerPage.open(QIODevice::ReadOnly)) {
            return internalErrorResponse;
        }

        return QHttpServerResponse(QString::fromStdString(registerPage.readAll().toStdString()));
    });



    server.route("/token", QHttpServerRequest::Method::Post, [&] (const QHttpServerRequest &request) {

        QUrlQuery postParameters(request.body());

        QHash<QString, QString> parameters;

        foreach(const auto &item, postParameters.queryItems()) {
            parameters[item.first] = QUrl(item.second).path();
        }


        if(!parameters.contains("login")) {
            return QHttpServerResponse("Request must contain \"login\"", QHttpServerResponse::StatusCode::BadRequest);
        }
        else if(!parameters.contains("psw")) {
            return QHttpServerResponse("Request must contain \"psw\" (password)", QHttpServerResponse::StatusCode::BadRequest);
        }

        int userId = databaseConnector->userIdFromCredentials(parameters["login"], parameters["psw"]);

        if(userId == -1) {
            return internalErrorResponse;
        }
        else if(userId == 0) {
            QFile userNotFoundPage(pathToUserNotFoundPage);
            if (!userNotFoundPage.open(QIODevice::ReadOnly)) {
                return internalErrorResponse;
            }

            return QHttpServerResponse(QString::fromStdString(userNotFoundPage.readAll().toStdString()),
                                       QHttpServerResponse::StatusCode::BadRequest);
        }
        else {
            QString userToken = databaseConnector->tokenFromUserId(userId);

            if(userToken == "") {
                return internalErrorResponse;
            }

            QFile tokenPage(pathToTokenPage);
            if (!tokenPage.open(QIODevice::ReadOnly)) {
                return internalErrorResponse;
            }

            return QHttpServerResponse(QString::fromStdString(tokenPage.readAll().toStdString()).arg(userToken));
        }

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



