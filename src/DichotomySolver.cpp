#include "include/DichotomySolver.h"

DichotomySolver::DichotomySolver(QObject *parent)
    : AbstractSolver{parent}
{

}

QJsonDocument DichotomySolver::solve(const QHash<QString, QString> &input)
{
    QString program = "python";

    QStringList arguments;
    arguments << "scripts/dichotomy.py"
              << input["left-bound"]
              << input["right-bound"]
              << input["function"]
              << input["e"]
              << input["l0"]
              << input["target"];

    QProcess process;
    process.start(program, arguments);

    // TODO timeout
    process.waitForFinished();

    return QJsonDocument::fromJson(process.readAll());
}
