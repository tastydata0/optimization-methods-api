#include "include/DichotomySolver.h"

DichotomySolver::DichotomySolver(QObject *parent)
    : AbstractSolver{parent}
{

}

QJsonDocument DichotomySolver::solve(const QHash<QString, QString> input)
{
    QString program = "python";

    QStringList arguments;
    arguments << "scripts/dichotomy.py"
              << input["left_bound"]
              << input["right_bound"]
              << input["function"]
              << input["e"]
              << input["l0"]
              << input["target"];

    QProcess process;
    process.start(program, arguments);

    QByteArray data;
    if (process.waitForFinished()) {
        data = process.readAllStandardOutput();
    }
    process.close();

    QJsonParseError error;
    QJsonDocument output = QJsonDocument::fromJson(data, &error);

    if(error.error == QJsonParseError::NoError) {
        return output;
    }
    else {
        return QJsonDocument::fromJson("{\"success\": false, \"error\": \"Internal error\"}");
    }



}
