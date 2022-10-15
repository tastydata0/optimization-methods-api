#include "include/AbstractSolver.h"

AbstractSolver::AbstractSolver(QObject *parent)
    : QObject{parent}
{

}

QHash<QString, QString> AbstractSolver::urlQueryToMap(const QUrlQuery &query)
{
    QHash<QString, QString> rawInput;
    foreach(auto item, query.queryItems()) {
        rawInput.insert(item.first, item.second);
    }
    return rawInput;
}
