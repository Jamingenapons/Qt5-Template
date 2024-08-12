#include <QFile>
#include <QTextStream>
#include <QDateTime>


#pragma once

class Logger {
public:
    Logger(const QString& filename);
    ~Logger();

    static void staticLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    QFile* file;
    QTextStream* stream;
};
