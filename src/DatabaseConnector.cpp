#include "include/DatabaseConnector.h"

#include <QSqlDriver>
#include <QSqlError>



DatabaseConnector::DatabaseConnector(QObject *parent)
    : QObject{parent}
{

}

bool DatabaseConnector::connect()
{
    bool ok;

    const QSettings databaseSecrets("database_secrets.ini", QSettings::Format::IniFormat);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "main");

    db.setHostName(databaseSecrets.value("Database/DATABASE_HOST").toString());
    db.setDatabaseName(databaseSecrets.value("Database/DATABASE_NAME").toString());
    db.setUserName(databaseSecrets.value("Database/DATABASE_USER").toString());
    db.setPassword(databaseSecrets.value("Database/DATABASE_PASSWORD").toString());

    ok = db.open();

    if(ok)
        qDebug() << "Connected to database";
    else
        qDebug() << "Failed to connect to database";

    return ok;
}

void DatabaseConnector::disconnect()
{
    {
        QSqlDatabase db = QSqlDatabase::database("main");
        db.close();
    }

    QSqlDatabase::removeDatabase("main");
    qDebug() << "Disconnected from database";
}


DatabaseConnector::REGISTER_USER_RESULT DatabaseConnector::registerUser(const QString &username, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if(!db.isOpen()) {
        qDebug() << "Internal error: database is not open";

        return REGISTER_USER_RESULT::INTERNAL_ERROR;
    }

    QString queryString = "CALL register_user('" + username + "', '" + password + "');";

    QSqlQuery result(db);

    if(!result.exec(queryString)) {
        qDebug() << "Internal error";

        return REGISTER_USER_RESULT::INTERNAL_ERROR;
    }

}

DatabaseConnector::TOKEN_CHECK_RESULT DatabaseConnector::checkToken(const QString &token)
{

}

