#ifndef HTTPSERVERHANDLER_H
#define HTTPSERVERHANDLER_H

#include <QObject>
#include <QHttpServer>

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
    QString processRequest(const QHttpServerRequest &request);


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
