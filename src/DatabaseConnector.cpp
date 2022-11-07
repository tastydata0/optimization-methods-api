#include "include/DatabaseConnector.h"

#include <QSqlDriver>
#include <QSqlError>



DatabaseConnector::DatabaseConnector(QObject *parent)
    : QObject{parent}
{
    connectionName = QString("%1").arg((qintptr)QThread::currentThread());
    if(!QSqlDatabase::connectionNames().contains(connectionName))
        connect();
    else
        qDebug() << "Running on established connection" << connectionName;
}

bool DatabaseConnector::connect()
{
    bool ok;

    const QSettings databaseSecrets("database_secrets.ini", QSettings::Format::IniFormat);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);

    db.setHostName(databaseSecrets.value("Database/DATABASE_HOST").toString());
    db.setDatabaseName(databaseSecrets.value("Database/DATABASE_NAME").toString());
    db.setUserName(databaseSecrets.value("Database/DATABASE_USER").toString());
    db.setPassword(databaseSecrets.value("Database/DATABASE_PASSWORD").toString());

    ok = db.open();

    if(ok)
        qDebug() << "Connected to database. Connection:" << connectionName;
    else
        qDebug() << "Failed to connect to database";

    return ok;
}

void DatabaseConnector::disconnect()
{
    qDebug() << "Disconnecting from database";
    {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        db.close();
    }

    QSqlDatabase::removeDatabase(connectionName);
    qDebug() << "Disconnected from database";
}


DatabaseConnector::REGISTER_USER_RESULT DatabaseConnector::registerUser(const QString &username, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

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

int DatabaseConnector::userQuota(const QString &token)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    if(!db.isOpen())
        return -1;

    const QString user_quota = "SELECT user_quota('%1');";

    QSqlQuery query(db);


    if(!query.exec(user_quota.arg(token))) {
        qDebug() << "Internal error";

        return -1;
    }

    query.next();
    return query.value(0).toInt();
}

bool DatabaseConnector::decreaseUserQuota(const QString &token)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    if(!db.isOpen())
        return false;

    const QString user_quota = "CALL decrease_user_quota('%1');";

    QSqlQuery query(db);


    if(!query.exec(user_quota.arg(token))) {
        qDebug() << "Internal error";

        return false;
    }

    return true;
}

int DatabaseConnector::userIdFromCredentials(const QString &username, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    if(!db.isOpen())
        return -1;

    const QString queryString = "SELECT user_id_from_credentials('%1', '%2');";

    QSqlQuery query(db);

    if(!query.exec(queryString.arg(username).arg(password))) {
        qDebug() << "Internal error:" << query.lastError();

        return -1;
    }

    query.next();
    return query.value(0).toInt();
}

QString DatabaseConnector::tokenFromUserId(int userId)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    if(!db.isOpen())
        return "";

    const QString queryString = "SELECT token FROM users WHERE id = %1;";

    QSqlQuery query(db);

    if(!query.exec(queryString.arg(userId))) {
        qDebug() << "Internal error:" << query.lastError();

        return "";
    }

    query.next();
    return query.value(0).toString();
}

DatabaseConnector::REGISTER_USER_RESULT DatabaseConnector::usernameNotTaken(const QString &username)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    if(!db.isOpen())
        return REGISTER_USER_RESULT::INTERNAL_ERROR;

    const QString queryString = "SELECT is_user_exists('%1');";

    QSqlQuery query(db);

    if(!query.exec(queryString.arg(username))) {
        qDebug() << "Internal error:" << query.lastError();

        return REGISTER_USER_RESULT::INTERNAL_ERROR;
    }

    query.next();
    if(!query.value(0).toBool()) {
        return REGISTER_USER_RESULT::OK;
    }
    else {
        return REGISTER_USER_RESULT::USER_EXISTS;
    }
}
