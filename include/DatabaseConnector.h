#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>

class DatabaseConnector : public QObject
{
    Q_OBJECT

    enum class REGISTER_USER_RESULT {OK, USERNAME_TAKEN, INTERNAL_ERROR};
    enum class TOKEN_CHECK_RESULT {OK, PERMISSION_DENIED, TOKEN_NOT_FOUND, INTERNAL_ERROR};

public:
    explicit DatabaseConnector(QObject *parent = nullptr);
    bool connect();
    void disconnect();
    REGISTER_USER_RESULT registerUser(const QString& username, const QString& password);
    TOKEN_CHECK_RESULT checkToken(const QString& token);

private:
    QSqlDatabase db;

signals:

};

#endif // DATABASECONNECTOR_H
