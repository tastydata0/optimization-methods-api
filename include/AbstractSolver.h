#ifndef ABSTRACTSOLVER_H
#define ABSTRACTSOLVER_H

#include <QJsonDocument>
#include <QHash>
#include <QObject>
#include <QUrlQuery>
#include <QVariant>


///
/// \class AbstractSolver
/// \brief Класс, абстрагирующий решение задачи.
///
class AbstractSolver : public QObject
{
    Q_OBJECT
public:

    explicit AbstractSolver(QObject *parent = nullptr);

    ///
    /// \brief Функция, преобразующая QUrlQuery в HashMap.
    /// \param query - данные Http запроса для преобразования.
    /// \return QHash, где название поля - ключ, значение - значение поля. Тип всех полей - QString.
    ///
    QHash<QString, QString> urlQueryToMap(const QUrlQuery &query);

    ///
    /// \brief Чистая виртуальная функция, решающая задачу.
    /// \param input - QHash с необходимыми ключами и корректными значениями в виде QString.
    /// \return Возвращает QJsonDocument с данными о решении задачи.
    /// В нем всегда есть поле "status", где отображено, было ли решение успешным или произошла ошибка.
    ///
    virtual QJsonDocument solve(const QHash<QString, QString> &input) = 0;

signals:

};

#endif // ABSTRACTSOLVER_H
