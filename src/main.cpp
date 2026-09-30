#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QTimer>

#include "app/window_state_manager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Luntar");
    QGuiApplication::setApplicationName("Image Importer");

    QQmlApplicationEngine engine;
    engine.loadFromModule("ImgImporter", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().constFirst());
    AR_ASSERT(window != nullptr);

    Window_State_Manager window_state_manager;
    window_state_manager.restore(window);

    QObject::connect(
        &app,
        &QGuiApplication::aboutToQuit,
        &app,
        [&window_state_manager, window]() {
            window_state_manager.save(window);
        });

    return app.exec();
}
