#pragma once

#include <QObject>
#include <QString>

class Import_Controller final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy NOTIFY busy_changed)
    Q_PROPERTY(int completed_count READ completed_count NOTIFY progress_changed)
    Q_PROPERTY(int total_count READ total_count NOTIFY progress_changed)
    Q_PROPERTY(QString status READ status NOTIFY status_changed)

public:
    explicit Import_Controller(QObject *parent = nullptr);

    [[nodiscard]] bool busy() const;
    [[nodiscard]] int completed_count() const;
    [[nodiscard]] int total_count() const;
    [[nodiscard]] QString status() const;

    /**
     * @brief Cleans all supported images in a source directory.
     * @param source_url Local directory URL selected by the user.
     */
    Q_INVOKABLE void clean_directory(const QUrl &source_url);

signals:
    void busy_changed();
    void progress_changed();
    void status_changed();
    void finished(const QString &output_directory);

private:
    bool _busy = false;
    int _completed_count = 0;
    int _total_count = 0;
    QString _status;
};
