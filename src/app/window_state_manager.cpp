#include "window_state_manager.h"

#include <QGuiApplication>
#include <QQuickWindow>
#include <QScreen>
#include <QSettings>

namespace
{
constexpr auto k_geometry_key = "window/geometry";
constexpr auto k_screen_key = "window/screen";
constexpr auto k_maximized_key = "window/maximized";
constexpr int k_minimum_visible_width = 160;
constexpr int k_minimum_visible_height = 80;
}

Window_State_Manager::Window_State_Manager(QObject *parent)
    : QObject(parent)
{
}

void Window_State_Manager::restore(QQuickWindow *window)
{
    AR_ASSERT(window != nullptr);

    QSettings settings;
    const QRect saved_geometry = settings.value(k_geometry_key).toRect();
    const QString saved_screen_name = settings.value(k_screen_key).toString();
    const bool was_maximized = settings.value(k_maximized_key, false).toBool();

    if (!saved_geometry.isValid()) {
        return;
    }

    QScreen *saved_screen = nullptr;
    for (QScreen *screen : QGuiApplication::screens()) {
        if (screen->name() == saved_screen_name) {
            saved_screen = screen;
            break;
        }
    }

    QRect geometry = saved_geometry;
    if (saved_screen == nullptr) {
        geometry = make_visible(geometry);
    } else {
        const QRect available = saved_screen->availableGeometry();
        if (!available.intersects(geometry)) {
            geometry.moveCenter(available.center());
        }
        geometry = make_visible(geometry);
    }

    window->setGeometry(geometry);

    if (was_maximized) {
        window->showMaximized();
    }
}

void Window_State_Manager::save(const QQuickWindow *window) const
{
    AR_ASSERT(window != nullptr);

    QSettings settings;

    const bool maximized = window->visibility() == QWindow::Maximized;
    settings.setValue(k_maximized_key, maximized);

    if (!maximized) {
        settings.setValue(k_geometry_key, window->geometry());
    }

    if (window->screen() != nullptr) {
        settings.setValue(k_screen_key, window->screen()->name());
    }
}

QRect Window_State_Manager::make_visible(const QRect &geometry) const
{
    const QList<QScreen *> screens = QGuiApplication::screens();
    AR_ASSERT(!screens.isEmpty());

    for (QScreen *screen : screens) {
        const QRect intersection = screen->availableGeometry().intersected(geometry);
        if (intersection.width() >= k_minimum_visible_width
            && intersection.height() >= k_minimum_visible_height) {
            return geometry;
        }
    }

    QScreen *primary_screen = QGuiApplication::primaryScreen();
    AR_ASSERT(primary_screen != nullptr);

    const QRect available = primary_screen->availableGeometry();
    QRect adjusted = geometry;

    adjusted.setWidth(qMin(adjusted.width(), available.width()));
    adjusted.setHeight(qMin(adjusted.height(), available.height()));
    adjusted.moveCenter(available.center());

    return adjusted;
}
