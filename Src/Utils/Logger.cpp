#include "Logger.h"

Logger::Logger(const QString& filename)
    : file(new QFile(filename)),
      stream(nullptr)
{
    if (file->open(QIODevice::WriteOnly | QIODevice::Append)) {
        stream = new QTextStream(file);
    }
}

Logger::~Logger()
{
    delete stream;
    delete file;
}

void Logger::log(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!stream) return;

    *stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " ";

    switch (type) {
    case QtDebugMsg:
        *stream << "Debug";
        break;
    case QtInfoMsg:
        *stream << "Info";
        break;
    case QtWarningMsg:
        *stream << "Warning";
        break;
    case QtCriticalMsg:
        *stream << "Critical";
        break;
    case QtFatalMsg:
        *stream << "Fatal";
        abort(); // You might want to handle this differently in a real application.
    }

    *stream << ": " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
    stream->flush();
}

void Logger::staticLog(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    // 可能需要一个单例或全局实例
    static Logger globalLogger("logfile.log"); // 假设这是构造Logger的方式
    globalLogger.log(type, context, msg);
}