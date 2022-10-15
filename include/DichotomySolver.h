#ifndef DICHOTOMYSOLVER_H
#define DICHOTOMYSOLVER_H

#include "AbstractSolver.h"
#include <QProcess>

class DichotomySolver : public AbstractSolver
{
public:
    explicit DichotomySolver(QObject *parent = nullptr);

    // AbstractSolver interface
    virtual QJsonDocument solve(const QHash<QString, QString> &input) override;

};

#endif // DICHOTOMYSOLVER_H
