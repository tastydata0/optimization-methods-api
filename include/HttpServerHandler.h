#ifndef HTTPSERVERHANDLER_H
#define HTTPSERVERHANDLER_H

#include <QObject>
#include <QHttpServer>
#include "include/DichotomySolver.h"
#include "qjsonobject.h"

///
/// \class HttpServerHandler
/// \brief Класс, управляющий классом QHttpServer, обрабатывающий входящие запросы.
///
class HttpServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit HttpServerHandler(QObject *parent = nullptr);

    short getServerPort() const;
    void setServerPort(short newServerPort);

private:
    QHttpServer server;
    short serverPort = 8080;

    ///
    /// \brief Обработчик запроса, который вызывается из лямбда-функций, вызываемых QHttpServer::route.
    /// \param solver - указатель на любой класс, решающий задачу (наследник AbstractSolver).
    /// \param queryParameters - параметры, переданные пользователем через Http запрос.
    /// \return
    ///
    QHttpServerResponse processRequest(AbstractSolver *solver, const QUrlQuery &queryParameters);



signals:
    ///
    /// \brief Вызывается после успешного запуска сервера
    ///
    void serverStarted();

public slots:

    ///
    /// \brief Слот для запуска сервера
    ///
    void startServer();
};

#endif // HTTPSERVERHANDLER_H
