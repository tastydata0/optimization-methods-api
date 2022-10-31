#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>

class DatabaseConnector : public QObject
{
    Q_OBJECT



public:

    enum class REGISTER_USER_RESULT {OK, USER_EXISTS, INTERNAL_ERROR};
    enum class TOKEN_CHECK_RESULT {OK, OUT_OF_QUOTA, TOKEN_NOT_FOUND, INTERNAL_ERROR};

    explicit DatabaseConnector(QObject *parent = nullptr);

    ///
    /// \brief Регистрация пользователя.
    /// \param username - имя пользователя
    /// \param password - пароль
    /// \return REGISTER_USER_RESULT::OK, если всё нормально, REGISTER_USER_RESULT::USER_EXISTS, если имя пользователя занято, USER_EXISTS::INTERNAL_ERROR при внутренней ошибке.
    ///
    REGISTER_USER_RESULT registerUser(const QString& username, const QString& password);

    ///
    /// \brief Проверка, осталась ли квота у пользователя.
    /// \param token - токен вида 11111f11-d518-4652-bb62-891c529f02a7
    /// \return TOKEN_CHECK_RESULT::OK, если квота есть, OUT_OF_QUOTA
    ///
    TOKEN_CHECK_RESULT doesUserHaveQuota(const QString& token);

public:

    ///
    /// \brief Проверка, существует ли пользователь с таким именем
    /// \param username - имя пользователя
    /// \return REGISTER_USER_RESULT::OK, если имя не занято, REGISTER_USER_RESULT::USER_EXISTS, если имя пользователя занято, USER_EXISTS::INTERNAL_ERROR при внутренней ошибке.
    ///
    REGISTER_USER_RESULT usernameNotTaken(const QString& username);

public slots:

    ///
    /// \brief Подключение к БД, используя данные из database_secrets.ini.
    /// \return true, если подключение удалось, иначе false.
    ///
    bool connect();

    ///
    /// \brief Отключение от БД.
    ///
    void disconnect();

};

#endif // DATABASECONNECTOR_H
