#pragma once

#include <QObject>

class QQuickWindow;

#ifndef AR_ASSERT
#include <cassert>
#define AR_ASSERT(EXP) assert(EXP)
#endif

class Window_State_Manager final : public QObject
{
    Q_OBJECT

public:
    explicit Window_State_Manager(QObject *parent = nullptr);

    /**
     * @brief Restores saved window geometry and constrains it to an available screen.
     * @param window Window to restore.
     */
    void restore(QQuickWindow *window);

    /**
     * @brief Saves normal window geometry, screen identity, and maximized state.
     * @param window Window to save.
     */
    void save(const QQuickWindow *window) const;

private:
    [[nodiscard]] QRect make_visible(const QRect &geometry) const;
};
