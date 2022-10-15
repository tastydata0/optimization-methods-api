#ifndef ABSTRACTSOLVER_H
#define ABSTRACTSOLVER_H

#include <QJsonDocument>
#include <QHash>
#include <QObject>
#include <QUrlQuery>
#include <QVariant>

class AbstractSolver : public QObject
{
    Q_OBJECT
public:

    explicit AbstractSolver(QObject *parent = nullptr);

    QHash<QString, QString> urlQueryToMap(const QUrlQuery &query);

    virtual QJsonDocument solve(const QHash<QString, QString> &input) = 0;

signals:

};

#endif // ABSTRACTSOLVER_H
