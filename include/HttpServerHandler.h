#ifndef HTTPSERVERHANDLER_H
#define HTTPSERVERHANDLER_H

#include <QObject>
#include <QHttpServer>
#include <QFile>
#include <QSslKey>
#include <QSslConfiguration>
#include "include/DichotomySolver.h"
#include "qjsonobject.h"

#define pathToCertificate ":/certificate.pem"
#define pathToPrivateKey ":/privatekey.key"
#define pathToRegisterPage ":/register.html"

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

    ///
    /// \brief Установка сертификата из файла.
    /// \param path - Путь к файлу приватного ключа.
    /// \param format - Формат кодирования ключа.
    /// \return true при успешном присваивании, false при ошибке.
    ///
    bool setSslCertificate(const QString& path, QSsl::EncodingFormat format = QSsl::Pem);

    ///
    /// \brief Установка приватного ключа из файла.
    /// \param path - Путь к файлу приватного ключа.
    /// \param password - Пароль приватного ключа.
    /// \param algorithm - Алгоритм приватного ключа.
    /// \param format - Формат кодирования ключа.
    /// \return true при успешном присваивании, false при ошибке.
    ///
    bool setSslPrivateKey(const QString& path, const QByteArray& password = QByteArray(), QSsl::KeyAlgorithm algorithm = QSsl::Rsa, QSsl::EncodingFormat format = QSsl::Pem);

private:
    QHttpServer server;
    QSslCertificate sslCertificate;
    QSslKey sslPrivateKey;
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
