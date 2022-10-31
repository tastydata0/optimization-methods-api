#ifndef DICHOTOMYSOLVER_H
#define DICHOTOMYSOLVER_H

#include "AbstractSolver.h"
#include <QProcess>

///
/// \class DichotomySolver
/// \brief Наследник AbstractSolver, решающий задачу поиска минимума/максимума методом дихотомии.
///
class DichotomySolver : public AbstractSolver
{
public:
    explicit DichotomySolver(QObject *parent = nullptr);


    ///
    /// \brief Реализация функции, унаследованной от AbstractSolver
    /// \param input - QHash с необходимыми ключами и корректными значениями в виде QString.
    /// \return Возвращает QJsonDocument с данными о решении задачи.
    /// В нем всегда есть поле типа bool "success", где отображено, было ли решение успешным или произошла ошибка.
    ///
    virtual QJsonDocument solve(const QHash<QString, QString> &input) override;

};

#endif // DICHOTOMYSOLVER_H
