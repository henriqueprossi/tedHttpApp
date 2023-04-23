#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "TedManager.h"
#include "ConnectedDeviceModel.h"

void bindQmlTypes() {

    // Register the type ConnectedDeviceModel under the url "org.example" in version 1.0
    // under the name "DataEntryModel"
    qmlRegisterType<ConnectedDeviceModel>("br.com.colleter.ted", 1, 0, "ConnectedDeviceModel");

    qmlRegisterSingletonType<TedManager>(
        "br.com.colleter.ted", 1, 0, "TedManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)

            TedManager *instance = &TedManager::instance();
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

    TedManager::instance();

    return app.exec();
}
