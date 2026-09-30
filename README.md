# Image Importer

A Qt 6 / QML score-image preparation tool.

The importer will turn arbitrary score photographs or rendered PDF pages into a canonical score package that a performance viewer can consume.

## Initial architecture

- Qt 6 + QML for the desktop UI.
- OpenCV for page detection, perspective correction, deskewing, cropping, and image normalization.
- A score manifest will map stable logical page IDs to canonical image files.
- Annotations will reference stable page IDs and normalized page coordinates.
- The importer and viewer remain separate applications connected by the score-package format.

## First milestone

1. Select a directory of images.
2. Discover and order the images.
3. Display page thumbnails.
4. Process a selected image through `Image_Processor`.
5. Show original and canonical page images for review.
6. Save the score manifest.

Automatic page/song inference comes after the basic score-package workflow is solid.
