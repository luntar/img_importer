#include "import_controller.h"

#include "image_processor.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

Import_Controller::Import_Controller(QObject *parent)
    : QObject(parent)
{
}

bool Import_Controller::busy() const
{
    return _busy;
}

int Import_Controller::completed_count() const
{
    return _completed_count;
}

int Import_Controller::total_count() const
{
    return _total_count;
}

QString Import_Controller::status() const
{
    return _status;
}

void Import_Controller::clean_directory(const QUrl &source_url)
{
    if (_busy) {
        return;
    }

    const QString source_path = source_url.toLocalFile();
    QDir source_directory(source_path);
    if (!source_directory.exists()) {
        _status = "Source directory does not exist.";
        emit status_changed();
        return;
    }

    const QStringList filters = {
        "*.jpg", "*.jpeg", "*.png", "*.bmp", "*.tif", "*.tiff"
    };
    const QFileInfoList input_files = source_directory.entryInfoList(
        filters,
        QDir::Files | QDir::Readable,
        QDir::Time | QDir::Reversed);

    _busy = true;
    _completed_count = 0;
    _total_count = input_files.size();
    _status = QString("Preparing %1 image(s)...").arg(_total_count);
    emit busy_changed();
    emit progress_changed();
    emit status_changed();

    const QString source_copy_path = source_directory.filePath("source");
    const QString page_path = source_directory.filePath("pages");
    QDir().mkpath(source_copy_path);
    QDir().mkpath(page_path);

    Image_Processor image_processor;
    int failure_count = 0;

    for (int index = 0; index < input_files.size(); ++index) {
        const QFileInfo &input_file = input_files.at(index);
        const QString source_copy = QDir(source_copy_path).filePath(input_file.fileName());
        if (!QFileInfo::exists(source_copy)) {
            QFile::copy(input_file.absoluteFilePath(), source_copy);
        }

        const QString output_file = QDir(page_path).filePath(
            QString("page_%1.png").arg(index + 1, 4, 10, QLatin1Char('0')));

        QString error;
        if (!image_processor.clean_page(input_file.absoluteFilePath(), output_file, &error)) {
            ++failure_count;
            _status = QString("Page %1 failed: %2").arg(index + 1).arg(error);
        } else {
            _status = QString("Prepared page %1 of %2").arg(index + 1).arg(_total_count);
        }

        ++_completed_count;
        emit progress_changed();
        emit status_changed();
    }

    _busy = false;
    _status = failure_count == 0
        ? QString("Prepared %1 page(s).").arg(_total_count)
        : QString("Prepared %1 page(s); %2 failed.").arg(_total_count - failure_count).arg(failure_count);

    emit busy_changed();
    emit status_changed();
    emit finished(page_path);
}
