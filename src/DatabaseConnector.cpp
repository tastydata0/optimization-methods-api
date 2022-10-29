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

    REGISTER_USER_RESULT exists = usernameNotTaken(username);
    if(exists == REGISTER_USER_RESULT::USER_EXISTS || exists == REGISTER_USER_RESULT::INTERNAL_ERROR) {
        return exists;
    }

    QString queryString = "CALL register_user('" + username + "', '" + password + "');";

    QSqlQuery query(db);

    if(!query.exec(queryString)) {
        qDebug() << "Internal error";

        return REGISTER_USER_RESULT::INTERNAL_ERROR;
    }

    return REGISTER_USER_RESULT::OK;

}

DatabaseConnector::TOKEN_CHECK_RESULT DatabaseConnector::doesUserHaveQuota(const QString &token)
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if(!db.isOpen())
        return TOKEN_CHECK_RESULT::INTERNAL_ERROR;

    const QString does_user_have_quota = "SELECT does_user_have_quota('%1');";

    QSqlQuery query(db);


    if(!query.exec(does_user_have_quota.arg(token))) {
        qDebug() << "Internal error";

        return TOKEN_CHECK_RESULT::INTERNAL_ERROR;
    }

    query.next();
    if(query.value(0).toBool()) {
        return TOKEN_CHECK_RESULT::OK;
    }
    else {
        return TOKEN_CHECK_RESULT::OUT_OF_QUOTA;
    }
}

DatabaseConnector::REGISTER_USER_RESULT DatabaseConnector::usernameNotTaken(const QString &username)
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if(!db.isOpen())
        return REGISTER_USER_RESULT::INTERNAL_ERROR;

    const QString queryString = "SELECT is_user_exists('%1');";

    QSqlQuery query(db);

    if(!query.exec(queryString.arg(username))) {
        qDebug() << "Internal error:" << query.lastError();

        return REGISTER_USER_RESULT::INTERNAL_ERROR;
    }

    query.next();
    if(query.value(0).toBool()) {
        return REGISTER_USER_RESULT::OK;
    }
    else {
        return REGISTER_USER_RESULT::USER_EXISTS;
    }
}
