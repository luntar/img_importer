#include <QGuiApplication>
#include <QImage>
#include <QPdfDocument>
#include <QPdfDocumentRenderOptions>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>

namespace {
constexpr qreal kDefaultDpi = 300.0;

int printUsage(const QString &appName)
{
    QTextStream err(stderr);
    err << "Usage: " << appName << " <pdf-file>\n";
    return 1;
}

int printLoadError(QPdfDocument::Error error, QTextStream &err)
{
    switch (error) {
    case QPdfDocument::Error::NoError:
        return 0;
    case QPdfDocument::Error::Unknown:
        err << "Failed to load PDF: unknown error.\n";
        break;
    case QPdfDocument::Error::DataNotYetAvailable:
        err << "Failed to load PDF: data not yet available.\n";
        break;
    case QPdfDocument::Error::FileNotFound:
        err << "Failed to load PDF: file not found.\n";
        break;
    case QPdfDocument::Error::InvalidFileFormat:
        err << "Failed to load PDF: invalid file format.\n";
        break;
    case QPdfDocument::Error::IncorrectPassword:
        err << "Failed to load PDF: incorrect password.\n";
        break;
    case QPdfDocument::Error::UnsupportedSecurityScheme:
        err << "Failed to load PDF: unsupported security scheme.\n";
        break;
    }
    return 1;
}
} // namespace

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QTextStream out(stdout);
    QTextStream err(stderr);

    const QStringList args = app.arguments();
    if (args.size() != 2) {
        return printUsage(args.value(0, "pdf_to_images"));
    }

    const QString pdfPath = args.at(1);
    QFileInfo pdfInfo(pdfPath);
    if (!pdfInfo.exists() || !pdfInfo.isFile()) {
        err << "PDF file does not exist: " << pdfPath << "\n";
        return 1;
    }

    QPdfDocument document;
    const QPdfDocument::Error loadResult = document.load(pdfInfo.absoluteFilePath());
    if (loadResult != QPdfDocument::Error::NoError) {
        return printLoadError(loadResult, err);
    }

    const int pageCount = document.pageCount();
    if (pageCount <= 0) {
        err << "PDF has no pages to render.\n";
        return 1;
    }

    const QDir outputDir(pdfInfo.absolutePath());
    const QString baseName = pdfInfo.completeBaseName();

    QPdfDocumentRenderOptions renderOptions;
    renderOptions.setRenderFlags(QPdf::RenderFlag::Antialiasing | QPdf::RenderFlag::TextAntialiasing);

    for (int page = 0; page < pageCount; ++page) {
        const QSizeF pagePointSize = document.pagePointSize(page);
        if (pagePointSize.isEmpty()) {
            err << "Skipping page " << (page + 1) << ": invalid page size.\n";
            continue;
        }

        const QSize imageSize = (pagePointSize * kDefaultDpi / 72.0).toSize();
        const QImage image = document.render(page, imageSize, renderOptions);
        if (image.isNull()) {
            err << "Failed to render page " << (page + 1) << ".\n";
            continue;
        }

        const QString fileName = QString("%1_%2.png")
                                     .arg(baseName)
                                     .arg(page + 1, 3, 10, QChar('0'));
        const QString outputPath = outputDir.filePath(fileName);
        if (!image.save(outputPath, "PNG")) {
            err << "Failed to save image: " << outputPath << "\n";
            continue;
        }

        out << outputPath << "\n";
    }

    return 0;
}
