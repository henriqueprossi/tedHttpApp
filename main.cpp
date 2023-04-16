#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "src/network/NetworkManager.h"
#include "src/network/DeviceModel.h"

void bindQmlTypes() {

    // Register the type DeviceModel under the url "org.example" in version 1.0
    // under the name "DataEntryModel"
    qmlRegisterType<DeviceModel>("br.com.colleter.ted", 1, 0, "DeviceModel");

    qmlRegisterSingletonType<NetworkManager>(
        "br.com.colleter.ted", 1, 0, "NetworkManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)

            NetworkManager *instance = &NetworkManager::instance();
            return instance;
        });
}

int main(int argc, char *argv[]) {

    QGuiApplication app(argc, argv);

    bindQmlTypes();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/tedHttpApp/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    NetworkManager::instance();

    return app.exec();
}
