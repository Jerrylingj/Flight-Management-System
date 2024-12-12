#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>  // 这个头文件是必需的
#include "api/networkhandler.h"
#include "storage/userinfo.h"

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

    UserInfo userInfo;
    userInfo.setUserName("John Doe");
    userInfo.setUserPersonalInfo("Some personal info about John");
    userInfo.setMyMoney(1000);

    // 将 UserInfo 对象暴露给 QML
    engine.rootContext()->setContextProperty("userInfo", &userInfo);

    return app.exec();
}
