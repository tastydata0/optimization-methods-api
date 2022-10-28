#include "include/DatabaseConnector.h"



DatabaseConnector::DatabaseConnector(QObject *parent)
    : QObject{parent}
{

}

bool DatabaseConnector::connect()
{
    bool ok;

    const QSettings databaseSecrets("database_secrets.ini", QSettings::Format::IniFormat);

    db = QSqlDatabase::addDatabase("QPSQL", "main");

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
    db.close();
    QSqlDatabase::removeDatabase("main");
    qDebug() << "Disconnected from database";
}


DatabaseConnector::REGISTER_USER_RESULT DatabaseConnector::registerUser(const QString &username, const QString &password)
{

}

DatabaseConnector::TOKEN_CHECK_RESULT DatabaseConnector::checkToken(const QString &token)
{

}

