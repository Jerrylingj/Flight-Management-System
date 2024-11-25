#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "api/networkhandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    qmlRegisterType<NetworkHandler>("NetworkHandler", 1, 0, "NetworkHandler");
    engine.loadFromModule("untitled7", "Main");

    return app.exec();
}
