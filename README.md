# Image Importer

A Qt 6 / QML score-image preparation tool.

The importer turns arbitrary score photographs or rendered PDF pages into a canonical score package that a performance viewer can consume.

## Initial architecture

- Qt 6 + QML for the desktop UI.
- OpenCV for page detection, perspective correction, deskewing, cropping, and image normalization.
- A score manifest maps stable logical page IDs to canonical image files.
- Annotations reference stable page IDs and normalized page coordinates.
- The importer and viewer remain separate applications connected by the score-package format.

## WSL / Linux development environment

WSL/Linux is the primary development path for the score application. Keep the repository in the WSL Linux filesystem (for example, `~/src/img_importer`) rather than under `/mnt/c`.

Install the required packages:

```bash
chmod +x scripts/install_deps_ubuntu.sh
./scripts/install_deps_ubuntu.sh
```

Configure and build with Ninja:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Run:

```bash
./build/img_importer
```

The same CMake project is intended to build natively on WSL/x86-64 and Raspberry Pi OS/ARM64. Initially, build natively on the Raspberry Pi rather than cross-compiling.

## Windows development environment

The recommended Windows setup is:

- Windows 10 or 11, 64-bit.
- Visual Studio 2022 with **Desktop development with C++** installed.
- CMake 3.21 or newer.
- Qt 6.5 or newer, installed with the Qt Online Installer.
- OpenCV 4.x installed through vcpkg.
- Git.

Keep the compiler architecture consistent: use 64-bit Qt, the Visual Studio x64 compiler, and the vcpkg `x64-windows` triplet.

### 1. Install Visual Studio

Install Visual Studio 2022 and select the **Desktop development with C++** workload. Make sure the MSVC compiler, Windows SDK, and CMake tools are installed.

Open a fresh **x64 Native Tools Command Prompt for VS 2022** for the command-line examples below.

Verify:

```bat
cl
cmake --version
git --version
```

### 2. Install Qt 6

Install Qt using the Qt Online Installer.

Install a Qt 6.5-or-newer desktop kit that matches Visual Studio 2022. The project currently requires:

- Qt Core
- Qt Gui
- Qt Quick
- Qt QML

For example, if Qt 6.8.x is installed under:

```text
C:\Qt\6.8.3\msvc2022_64
```

that directory is the Qt prefix used during CMake configuration.

Qt Creator is optional. The project is a normal CMake project and can also be built from Visual Studio or the command line.

### 3. Install vcpkg and OpenCV

Choose a permanent location for vcpkg. For example:

```bat
cd C:\dev
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
```

Install 64-bit OpenCV:

```bat
vcpkg install opencv4:x64-windows
```

The project currently uses the OpenCV `core`, `imgproc`, and `imgcodecs` components.

You do not need to install a second copy of Qt through vcpkg. Qt comes from the Qt installation above; OpenCV comes from vcpkg.

### 4. Clone this repository

```bat
cd C:\dev
git clone https://github.com/luntar/img_importer.git
cd img_importer
git switch feature/qt-opencv-importer
```

### 5. Configure with CMake

The two important CMake inputs are:

- `CMAKE_PREFIX_PATH`: location of the Qt desktop kit.
- `CMAKE_TOOLCHAIN_FILE`: the vcpkg CMake toolchain.

Example:

```bat
cmake -S . -B build ^
  -G "Visual Studio 17 2022" ^
  -A x64 ^
  -DCMAKE_PREFIX_PATH=C:\Qt\6.8.3\msvc2022_64 ^
  -DCMAKE_TOOLCHAIN_FILE=C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake
```

Change the Qt and vcpkg paths to match the local machine.

A successful configure should report that Qt and OpenCV were found and should generate a Visual Studio solution in `build`.

### 6. Build

From the command line:

```bat
cmake --build build --config Debug
```

Or open the generated solution:

```text
build\img_importer.sln
```

and build the `img_importer` target in Visual Studio.

### 7. Run

For a Debug build, the executable will normally be under the generated build tree. CMake can build and Visual Studio can launch it directly.

If Windows reports that Qt DLLs are missing when launching the executable outside the development environment, run Qt's `windeployqt` on the executable or add the Qt kit's `bin` directory to the development PATH. Deployment packaging will be added later; it is not part of the initial development milestone.

### Troubleshooting

#### CMake cannot find Qt6

Confirm that `CMAKE_PREFIX_PATH` points at the compiler-specific Qt directory, not merely `C:\Qt`.

For example:

```text
C:\Qt\6.8.3\msvc2022_64
```

If the build directory was configured with the wrong Qt installation, delete `build` and configure again.

#### CMake cannot find OpenCV

Confirm that the vcpkg toolchain was supplied when the build directory was first configured:

```text
-DCMAKE_TOOLCHAIN_FILE=C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake
```

and confirm:

```bat
C:\dev\vcpkg\vcpkg.exe list
```

contains `opencv4:x64-windows`.

If the build directory was originally configured without the vcpkg toolchain, delete `build` and configure again.

#### Architecture mismatch

Everything should be x64:

- Visual Studio generator: `-A x64`
- Qt kit: `msvc2022_64`
- vcpkg triplet: `x64-windows`

Mixing x86 and x64 libraries will cause link failures.

## First milestone

1. Select a directory of images.
2. Discover and order the images.
3. Display page thumbnails.
4. Process a selected image through `Image_Processor`.
5. Show original and canonical page images for review.
6. Save the score manifest.

Automatic page/song inference comes after the basic score-package workflow is solid.
