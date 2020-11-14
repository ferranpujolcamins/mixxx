- [Building On Linux](#building-on-linux)
- [Building on Windows](#building-on-windows)
- [Building on macOS](#building-on-macos)

# Building On Linux

Compiling software is the process of turning human-readable source code
into machine code a computer can execute. Compiling Mixxx is fairly
straightforward on Linux. The steps below outline what to do.

## Install build dependencies

Mixxx relies on several external libraries for various features.

### Debian / Ubuntu / Raspbian

First of all install cmake, the tool that we need to build the project, running:

```sh
sudo apt-get install cmake
```

If your distribution is Debian 9/10 based (such as Ubuntu 16.04/18.04), you can install
them by running:

```sh
sudo apt-get install g++ git scons libportmidi-dev libopusfile-dev \
  libshout-dev libtag1-dev libprotobuf-dev protobuf-compiler \
  libusb-1.0-0-dev libfftw3-dev libmad0-dev portaudio19-dev \
  libchromaprint-dev librubberband-dev libsqlite3-dev \
  libid3tag0-dev libflac-dev libsndfile-dev libupower-glib-dev \
  libavcodec-dev libavformat-dev \
  libgl-dev liblilv-dev \
  libjack-dev libjack0 portaudio19-dev \
  libfaad-dev libmp4v2-dev \
  libmp3lame-dev libebur128-dev
```

Note: `libfaad-dev libmp4v2-dev` are required for M4A support. The
installation order is [important](https://bugs.launchpad.net/mixxx/+bug/1464120).

Beginning with Debian 10 (Ubuntu 20.04) support for libmp4v2 has been dropped.
Instead install additional FFmpeg packages to build Mixxx with FFmpeg enabled:

```sh
sudo apt-get install g++ git scons libportmidi-dev libopusfile-dev \
  libshout-dev libtag1-dev libprotobuf-dev protobuf-compiler \
  libusb-1.0-0-dev libfftw3-dev libmad0-dev portaudio19-dev \
  libchromaprint-dev librubberband-dev libsqlite3-dev \
  libid3tag0-dev libflac-dev libsndfile-dev libupower-glib-dev \
  libavcodec-dev libavformat-dev libavutil-dev libswresample-dev \
  libgl-dev liblilv-dev \
  libjack-dev libjack0 portaudio19-dev \
  libmp3lame-dev libebur128-dev
```

And build with:

```
scons faad=0 ffmpeg=1
```

If you are building the Mixxx **master** or **2.3** git branch, additionally run:

``` sh
sudo apt-get install qt5-default qt5keychain-dev qtdeclarative5-dev libqt5opengl5-dev qtscript5-dev libqt5svg5-dev libqt5x11extras5-dev libvamp-sdk2v5 libhidapi-libusb0 libqt5sql5-sqlite libmodplug-dev
```

If you are building Mixxx **2.2**, additionally run:
``` sh
sudo apt-get install qt5-default qtdeclarative5-dev libqt5opengl5-dev qtscript5-dev libqt5svg5-dev libqt5x11extras5-dev libqt5sql5-sqlite libmodplug-dev
```

If you are building Mixxx **2.1**, additionally run:
``` sh
sudo apt-get install libqt4-dev libqt4-sql-sqlite libqt4-opengl-dev libqt4-svg libqt4-xmlpatterns libqt4-sql
```

### Fedora

On Fedora, [enable the RPMFusion package repository](http://rpmfusion.org/Configuration).
You only need to enable the *free* repository; the *nonfree* repository is not necessary for Mixxx.
Then run:
``` sh
sudo dnf groupinstall "Development Tools"
sudo dnf install gcc-c++ lame-devel
sudo dnf builddep mixxx
```

if you are building the master branch before the 2.4 release, you also need to install qt5-declarative:
```sh
sudo dnf install qt5-declarative-devel
```

### Arch & Derivatives

If you are developing in Arch, you should have the
[base-devel group](https://www.archlinux.org/groups/x86_64/base-devel/)
installed.

The tools you are going to need for working with Mixxx are:
``` sh
sudo pacman -S --needed git gcc cmake
```
Alternatively, you can substitute gcc with clang.

Then install the dependencies:
``` sh
sudo pacman -S --needed libid3tag libmad portaudio qt libogg \
    libvorbis libsndfile portmidi libmp4v2 faad2 libshout \
    taglib protobuf vamp-plugin-sdk rubberband \
    chromaprint sqlite upower lilv lame \
    soundtouch opusfile
```

### Nix & NixOS

If you have the nix package manager installed (can be done on any linux),
or you are running [NixOS](https://nixos.org/), getting started is the easiest.

#### Creating a release

```
nix-build shell.nix --arg releaseMode true --arg defaultLv2Plugins true
```
This will build a fully functional mixxx derivate to run at any time.

#### Development Environment

To get a working development environment start a nix-shell like this:

```
nix-shell --arg enableKeyfinder true --arg defaultLv2Plugins true
```
You can then use the commands `configure`, `build`, `run`, `debug` for your workcycle.
The result will be placed in the folder cbuild.
ccache is used inside the development shell to speed up your recompile times.

#### VSCode support

You can use following task for building mixxx inside the nix development environment and
have proper error parsing:

```json
{
    "label": "build",
    "type": "shell",
    "command": "nix-shell --arg enableKeyfinder true --arg defaultLv2Plugins true --command 'build;'",
    "options": {
        "cwd": "${workspaceFolder}"
    },
    "problemMatcher": {
        "base": "$gcc",
        "fileLocation": ["absolute"]
    },
    "group": {
        "kind": "build",
        "isDefault": true
    },
    "runOptions": { "instances": 1}
}
```

### Non-system Qt

If your distribution's Qt package is older than the version required by
Mixxx, download the latest [Qt source
code](https://download.qt.io/archive/qt/). For each Qt version, it is
available at that link in a directory called "single" and has a filename
like `qt-everywhere-src-VERSION.tar.xz`. Extract that archive and
compile the source code:

    tar xf qt-everywhere-src-VERSION.tar.xz
    cd qt-everywhere-src-VERSION
    ./configure -prefix /path/to/qt/install -system-sqlite -sql-sqlite -qt-zlib -opensource -confirm-license -nomake examples -nomake tests -skip qt3d -skip qtwebengine
    make -j4 # replace 4 with however many threads your CPU can run. This will take a long time.
    make install

### Other

For other distributions, you will need to install the following through
your distribution's package manager. On most distributions, you will
also need the corresponding -dev or -devel packages for each package. If
you cannot find a package listed here that starts with "lib", try
searching for the package name without the "lib" prefix.

  - scons
  - libid3tag
  - libmad
  - [PortAudio-v19](http://www.portaudio.com)
  - Qt 5
  - libogg, libvorbis, libvorbisfile
  - libsndfile
  - [PortMidi & PortTime](http://portmedia.sourceforge.net/portmidi)
  - [libmp4](http://www.mpeg4ip.net/) (or
    [libmp4v2](http://code.google.com/p/mp4v2/)) (optional, for M4A file
    support)
  - [faad2](http://www.audiocoding.com/faad2.html) (optional, for M4A
    file support)
  - libshout
  - taglib
  - [protobuf](http://code.google.com/p/protobuf/)
  - [vamp-plugin-sdk](http://www.vamp-plugins.org/develop.html)
    (**optional** if not installed, Mixxx uses an internal version)
  - [librubberband](http://breakfastquay.com/rubberband/)
  - libchromaprint
  - libsqlite3
  - libupower-glib-dev
  - [lilv](http://drobilla.net/software/lilv)
  - [libmodplug](http://modplug-xmms.sourceforge.net/) (optional, for
    MOD file playback support)

## Get Mixxx

If you are still running as root from installing packages, return to
using your normal unprivileged user account (press Ctrl + D or run
`exit`).

If you want to compile Mixxx, you'll need to download the source code.
Either grab the source for the latest release from the
[downloads page on the website](https://www.mixxx.org/download),
or checkout a snapshot from our git repository:

  - For the latest development (master) branch: `git clone https://github.com/mixxxdj/mixxx.git`
  - For the latest stable branch: `git clone -b <version> https://github.com/mixxxdj/mixxx.git`

To update to the latest version of a git branch, enter (`cd` into) the
directory you cloned the git repository into and run `git pull`. Refer
to [Using Git](Using%20Git) for more details.

## Compile and install

### CMake

Mixxx uses the CMake build system as of Mixxx 2.3 (currently the master
branch). Building and installing Mixxx follows the standard CMake
procedures.

To build with CMake, first create a new directory and enter it.
This is typically under the project root, but it can be anywhere you want.
```sh
mkdir cmake_build
cd cmake_build
```

Now configure CMake. This only needs to be done once; you don't need to
repeat it when you compile Mixxx again. This step checks if you have all
the dependencies installed, similar to the configure script of GNU
autotools. `/usr` is used as the installation path in this example, but
you can set this to anywhere as long as your `$PATH` environment
variable includes a `bin` directory under the installation path
(`/usr/bin` if the installation path is `/usr`).
```sh
cmake -DCMAKE_INSTALL_PREFIX=/usr .. # Replace .. with the path to the project root if you are outside it
```

##### Compile Mixxx
Set the `--parallel` option to the number of CPU cores
you have. This will take a while, depending on the speed of your
computer.
```sh
cmake --build . --parallel `nproc`
```

Contrary to the behavior of SCons, CMake does not move the produced
binaries into the root folder of the project.

##### Install Mixxx
If you want to compile and install in one step, you can
skip the compilation step above and just run this command.
```sh
cmake --build . --target install --parallel `nproc`
```
#### Debug build

If you want to make a debug build, add `-DCMAKE_BUILD_TYPE=Debug
-DDEBUG_ASSERTIONS_FATAL=ON` to the end of the cmake configure command.
Debug builds should be started with the command line option
`--debugAssertBreak` to trigger a breakpoint in the debugger if debug
assertions are violated or to abort Mixxx immediately. This ensures that
messages about violated debug assertions are not missed between various
other debug log messages. We recommend this if you are working on Mixxx
code, but not if you are performing with Mixxx.

#### ccache

We highly recommend installing [CCache](https://ccache.dev/) if you will
be contributing code to Mixxx. If you won't be writing or testing code
and are just building Mixxx to use it for yourself, you can skip
installing CCache. CCache drastically speeds up the time to recompile
Mixxx, especially when switching between git branches. CMake works with
CCache automatically.

You will probably want to increase the default ccache size of 5.0GB to
something much larger to accommodate Mixxx's large build sizes. You can
adjust the cache size with the --set-config flag:
```sh
ccache --set-config=max_size=20.0G
```

#### Non-System-Qt

Append `-DCMAKE_PREFIX_PATH=/path/to/qt/install` (where
`/path/to/qt/install` is the path you used when [building
Qt](compiling_on_linux#non-system_qt)) to the cmake configure command to
instruct cmake to prefer the Qt version from that path.

### SCons

Mixxx 2.2 and earlier use the SCons build system. Mixxx 2.3 also
supports SCons, but SCons support will be removed in Mixxx 2.4.

Once you have the source code, change to the newly created "mixxx"
directory (run `cd mixxx`). Mixxx uses the [SCons](http://scons.org/)
build system rather than the more common GNU autotools and GNU make.
Running `scons -h` in the "mixxx" directory shows a complete list of
build flags if you'd like to customize. To compile without any special
options, as a regular user, run:
```sh
scons prefix=INSTALLATION_DIRECTORY -j NUMBER_OF_CPU_CORES optimize=native
```
Change INSTALLATION\_DIRECTORY to the location you want to install Mixxx
to. If you want to install Mixxx for all users of the OS, you do not
need to specify a prefix and can leave it as the default, which is
/usr/local. If you only want to install Mixxx for your user, you can
specify a location in your home directory such as \~/local

Running `scons` will take some time, depending on the speed of your
computer. Specifying NUMBER\_OF\_CPU\_CORES will tell scons to run that
many threads at a time while compiling. This speeds up compilation on
multi-core CPUs. You can check how many threads your CPU can run
simultaneously with the `lscpu` command (look for the `CPU(s)` field in
the output). Setting more threads than your CPU can handle will decrease
performance.

Once Mixxx has compiled, if you set the prefix options for scons to a
directory that your normal user does not have write access to, run
```sh
sudo scons prefix=INSTALLATION_DIRECTORY install
```
to install it. If you set the prefix to a directory your user does have
write access to, then you do not need `sudo` before `scons`. The prefix
option must be the same as before or scons will recompile Mixxx before
installing it.

If you want to be able to run Mixxx on different types of CPUs, change
`optimize=native` to `optimize=portable`. If you want to contribute code
to Mixxx and use a debugger, use `optimize=off`.

To compile on a Raspberry Pi (only compatible on Rapsberry Pi 3 and
later), use the arguments: `optimize=native machine=armhf` with scons.

#### Debug build

To catch bugs early during development build and run Mixxx with the
following options.

    build=debug debug_assertions_fatal=1

Debug builds should be started with the command line option
`--debugAssertBreak` to trigger a breakpoint in the debugger if debug
assertions are violated or to abort Mixxx immediately. This ensures that
messages about violated debug assertions are not missed between various
other debug log messages. We recommend this if you are working on Mixxx
code, but not if you are performing with Mixxx.

#### Optional: Build with m4a/AAC file support

If you want to play m4a files, add `faad=1` to your scons commands
above. This requires the libraries faad2 and libmp4v2 (or libmp4) to be
installed.

#### Optional: Compile with Clang

[Clang](http://clang.llvm.org) is a C/C++ compiler based on [LLVM](http://llvm.org).
Using Clang has various benefits:

  - [Better error messages](http://clang.llvm.org/diagnostics.html)
  - Colorized compilation output
  - Better tools for analyzing problems in your program (
    [Address Sanitizer](http://clang.llvm.org/docs/AddressSanitizer.html),
    [Thread Sanitizer](http://clang.llvm.org/docs/ThreadSanitizer.html),
    [MemorySanitizer](http://clang.llvm.org/docs/MemorySanitizer.html),
    etc.)

On Debian, Clang is provided as a package with a version number
attached. Using 6.0 as an example, install it like this:

    sudo apt-get install clang-6.0

To compile Mixxx using Clang 6.0, before running `scons`:

    export CC=clang-6.0
    export CXX=clang++-6.0

You can now use clang-specific SCons options.

  - To enable colorized output, use the `color=1` scons flag.
  - To enable Address Sanitizer, use the `asan=1` scons flag.

#### Troubleshooting scons

If scons can't find installed dependencies, try

    scons --config=force

#### Uninstall

To uninstall a copy of Mixxx that you compiled with SCons, `cd` into the
directory where you ran `scons` before, then run:

    scons -c prefix=INSTALLATION_DIRECTORY install

INSTALLATION\_DIRECTORY must be the same as that used when compiling and
installing. If you needed to use `sudo` to install, also use `sudo` to
uninstall.

#### Clean up

If `scons` fails with mysterious errors about not finding dependencies
that you know are installed, it may be using outdated cached information
to look for the dependencies. This can happen after upgrading your
GNU/Linux distribution. To resolve this, try running `scons -c` and
recompiling Mixxx.

## Run without installing

If you want to run your compiled version without installing,
from the same directory, run:

    ./mixxx --resourcePath res/ --settingsPath <folder>

You can omit the `--settingsPath` argument, but then mixxxx will
use and potentially overwrite your user-wide configs.

*(If you're looking to make an installable release, [go to this
page](Build%20Windows%20installer).)*

# Building on Windows

*FIXME Mixxx is fading out SCons in favor of [cmake](https://cmake.org)
starting in 2.3 and [will completely switch
in 2.4](https://github.com/mixxxdj/mixxx/pull/2777). This page isn't
updated with the appropriate instructions yet.*

## Programs to install

  - Visual Studio 2017 Community Edition or the Visual Studio 2017 Build Tools.
    (Visual Studio 2015 may work, but will likely stop working once we
    start using C++ features that are not implemented by VS2015).
  - [Python](http://python.org/download/) 2.7.x
    Ensure Python is on
    your system PATH (there is an option in the installer to do this).
    Don't use Python 3.x as [Scons is not compatible with it](https://github.com/SCons/scons/wiki/FrequentlyAskedQuestions#what-version-of-python-do-i-need).
  - [Latest version of SCONS](http://scons.org/pages/download.html)
  - A Git client like [Git for Windows](https://git-scm.com/download/win),
    [TortoiseGit](https://code.google.com/p/tortoisegit/) or the
	[github windows client](http://github-windows.s3.amazonaws.com/GitHubSetup.exe)
    (featuring a unix like command line)

## Acquire Mixxx dependencies

To build Mixxx, you need built copies of its dependencies.
You may download pre-built versions of them from the Mixxx team (recommended)
or build them from source.

### Option 1: Download pre-built Mixxx dependencies

You can find pre-built Mixxx dependencies at the bottom of this [Github
Page](https://github.com/mixxxdj/buildserver).

The folder `2.?.x-windows` contains build environments for the `2.?`
release of Mixxx. If you are working on the master branch, always pick
the latest version. If you want to work on a specific branch, pick the
corresponding folder. Check the
[build/windows/golden\_environment](https://github.com/mixxxdj/mixxx/blob/master/build/windows/golden_environment)
file in the Mixxx codebase to see what version of the build environment
is currently used for builds.

You may choose between the "release-fastbuild" and "release" variants.

  - release-fastbuild is built with link-time code-generation (LTCG)
    disabled. This leads to faster builds but potentially results in a
    slower version of Mixxx (we haven't measured it so we don't know).
    When we build Mixxx on AppVeyor for continuous integration, this is
    the version we use.
  - release is the version used to produce Mixxx releases. It is
    compiled with LTCG enabled and requires at least 8 GB of memory (or
    equivalent free space on the disk with your [paging
    file](https://www.howtogeek.com/126430/htg-explains-what-is-the-windows-page-file-and-should-you-disable-it/))
    to link Mixxx.

If you want to build a 32-bit version of Mixxx, choose an "x86" variant.
For 64-bit, choose an "x64" variant.

Download and unzip one of these environments. Remember the folder to
which the repository was saved. We will refer to that folder as
**WINLIB\_PATH** later.

### Option 2: Compile Mixxx dependencies from source

If you want to build the Mixxx dependencies from source instead of
downloading pre-built ones:

1.  Clone the [Mixxx buildserver repository](https://github.com/mixxxdj/buildserver).
    Remember the folder to which the repository was saved. We will refer
    to that folder as **WINLIB\_PATH** later.
2.  In the buildserver repository, checkout the **2.?.x-windows**
    branch, depending on which version of Mixxx you are building the
    dependencies for.
3.  Start a Windows command prompt (you do not need a Windows SDK
    command prompt). Open the Start Menu and type "cmd" into the search
    box and hit enter.
4.  Change into the **WINLIB\_PATH** directory.
    `cd WINLIB_PATH_GOES_HERE`
5.  Build the environment:
    - 32-bit: `build_environment x86 Release`
    - 64-bit: `build_environment x64 Release`

This step will take 2-3 hours depending on how many CPU cores you have.
Go have lunch.

## Download Mixxx's source code

1.  Clone the [Mixxx](https://github.com/mixxxdj/mixxx.git) repository.
    Remember the folder to which the repository was saved. We will refer
    to that folder as **MIXXX\_REPO** later.
2.  Start a command prompt (it doesn't need to be a Windows SDK prompt)
    and change into the **MIXXX\_REPO** directory.
3.  If you are building Mixxx 2.1 or 2.2, create a file called
    `build.bat` with the following content:

    ```bat
	@echo off
    SETLOCAL
    REM Clean up after old builds.
    del /q /f *.exe
    rmdir /s /q dist32
    rmdir /s /q dist64

    REM this can be either release or debug. For development you want to use debug
    set BUILD_TYPE=release

    REM This determines if you build a 32bit or 64bit version of mixxx.
    REM 32bit = i386, 64bit = amd64
    set ARCHITECTURE=i386

    REM set this to the folder where you built the dependencies
    set WINLIB_PATH="**Enter Path to WINLIB_PATH**"
    SET BIN_DIR=%WINLIB_PATH%\bin
    REM make sure the Qt version matches the version in WINLIB_PATH.
    set QT_VERSION=5.11.2
    SET QTDIR=%WINLIB_PATH%\Qt-%QT_VERSION%

    if "%ARCHITECTURE%" == "i386" (
            set TARGET_MACHINE=x86
            set VCVARS_ARCH=x86
    ) else (
            set TARGET_MACHINE=amd64
            set VCVARS_ARCH=x86_amd64
    )

    REM Adjust to your environment
    call "C:\Program Files (x86)\Microsoft Visual
    Studio 14.0\VC\vcvarsall.bat" %VCVARS_ARCH%

    rem /MP Use all CPU cores.
    rem /FS force synchronous PDB writes (prevents PDB corruption with
    /MP)
    rem /EHsc Do not handle SEH in try / except blocks.
    set CL=/MP /FS /EHsc

    set PATH=%BIN_DIR%;%PATH%
    REM Set the -j value to the number of CPU cores (not HT "virtual" cores but physical cores) you have
    scons -j2 toolchain=msvs winlib=%WINLIB_PATH% build=%BUILD_TYPE%
    staticlibs=1 staticqt=1 verbose=0 machine=%TARGET_MACHINE%
    qtdir=%QTDIR% hss1394=1 mediafoundation=1 opus=1 localecompare=1
    optimize=portable virtualize=0 test=1 qt_sqlite_plugin=0
    build_number_in_title_bar=0 bundle_pdbs=1
    ENDLOCAL
    ```

	This script will setup the build environment and call scons with
    the appropriate flags. You have to edit the **WINLIB\_PATH**
    variable and set it to the absolute path of the folder where you
    compiled the dependencies for mixxx. If you build the environment
    yourself instead of using the precompiled environment, you will need
    to adjust the **QTDIR** variable too.

    If you are building Mixxx 2.3, create a file called `build.bat` with
    the following content

	```bat
	@echo off
    SETLOCAL enableDelayedExpansion

    if "%1" == "clean" (
    REM Clean up after old builds.
    del /q /f *.exe
    del /q /f .sconsign.dblite
    del config.h
    rmdir /s /q dist32
    rmdir /s /q dist64
    rmdir /s /q .sconf_temp
    rmdir /s /q win32_build
    rmdir /s /q win64_build
    rmdir /s /q cache
    )

    REM x86 or x64
    SET LOCAL_PROCESSOR=x64

    REM debug, release or release-fastbuild. Warning: The debug buildenv is not prebuilt so you need to build it yourself.
    SET LOCAL_BUILDMODE=release-fastbuild

    REM The name of the directory where the libraries are located (and the name of the file to download).
    REM For the precompiled ones, it is automatically filled from ./build/windows/golden_environment
    REM You can replace this detection with the correct name if it does not apply to your setup.
    set /P LOCAL_WINLIB_NAME=<build/windows/golden_environment
    set LOCAL_WINLIB_NAME=!LOCAL_WINLIB_NAME:PLATFORM=%LOCAL_PROCESSOR%!
    set
    LOCAL_WINLIB_NAME=!LOCAL_WINLIB_NAME:CONFIGURATION=%LOCAL_BUILDMODE%!

    REM %~dp0.. means the parent folder of your mixxx source code.
    SET LOCAL_WINLIB_PARENT=%~dp0..

    REM This sets the number of processors to half or what Windows reports.
    REM This is done because most processors have some sort of hyperthreading and we want only real cores.
    IF %NUMBER_OF_PROCESSORS% GTR 1 (
            SET /A LOCAL_NUMBER_PROCS=%NUMBER_OF_PROCESSORS%/2
    ) ELSE (
            SET /A LOCAL_NUMBER_PROCS=%NUMBER_OF_PROCESSORS%
    )
    SET SCONS_NUMBER_PROCESSORS=-j%LOCAL_NUMBER_PROCS%

    call ./build/windows/install_buildenv.bat
    http://downloads.mixxx.org/builds/buildserver/2.3.x-windows/
    %LOCAL_WINLIB_NAME% %LOCAL_WINLIB_PARENT%
    if ERRORLEVEL 1 ENDLOCAL && EXIT /b

    REM skiptest means that we don't want to build and execute the mixxx-test.
    REM skipinstaller means that we don't want to generate the installer after the build.
    ./build/appveyor/build_mixxx.bat %LOCAL_PROCESSOR% %LOCAL_BUILDMODE% %LOCAL_WINLIB_PARENT%\%LOCAL_WINLIB_NAME% skiptest skipinstaller

    ENDLOCAL
    ```

4.  When you are ready, type: `build.bat`

## Build 64bit version of Mixxx

You have to follow the above guide with two changes.

1.  Build the dependencies with: `build_environment x64 Release` or make
    sure you have downloaded the x64 version of the pre-built
    dependencies.
2.  Use **set ARCHITECTURE=amd64** in **build.bat**

**WARNING**: DO NOT mix 32 and 64 bits build in the same CMD Shell
window or you will have undetermined results. If you need 32 and 64 bits
environments together, use different terminal window for 32 bits and 64
bits build\_environment and mixxx compilation

## Build debug version of Mixxx

You have to follow the above guide with two changes.

1.  Build the dependencies with: `build_environment xxx Debug`
2.  Use **set BUILD\_TYPE=debug** in **build.bat**

# Building on macOS

Compiling Mixxx for macOS is simple once you have all the dependencies
installed. This guide assumes you have basic knowledge about using the
command line.

1. [Install Xcode Command Line Tools](#1-install-xcode-command-line-tools)
1. [Install build dependencies](#2-install-build-dependencies)
    1. [Method 1: Homebrew](#method-1-homebrew)
    1. [Method 2: Use a pre-built environment](#method-2-use-a-pre-built-environment)
    1. [Method 3: Manual](#method-3-manual)
1. [Get Mixxx](#3-get-mixxx)
1. [Compile and install](#4-compile-and-install)
    1. [Configure the build for Homebrew dependencies](#configure-the-build-for-homebrew-dependencies)
    1. [Configure the build for pre-built environment](#configure-the-build-for-pre-built-environment)
1. [Configure your development tools](#5-configure-your-development-tools)

## 1. Install Xcode Command Line Tools

Launch the Terminal application, and type the following command string:

```shell
xcode-select --install
```

Click *Install* on the software update popup window that will appear,
and wait for the download and installation to finish (about 150 MB). It
gets placed in the following directory:
`/Library/Developer/CommandLineTools/`

<span class="underline">Note</span>: If Xcode is already installed in
your system, then Command Line Tools are installed as well (you can
check this by trying to run `clang` or `make` from the terminal). To
install the latest available version of Xcode for your macOS release, go
to <https://developer.apple.com/download/>. Downloading it requires a
free registration at Apple's developer site.

<a name="installDependencies"/>

## 2. Install build dependencies

### Method 1: Homebrew

**There is currently a major performance problem with Qt 5.14 and Mixxx
on macOS. We recommend [using our prebuilt
dependencies](#method-2use-a-pre-built-environment) until this is
[fixed](https://github.com/mixxxdj/mixxx/pull/1974).**

[Homebrew](https://github.com/Homebrew/brew) is yet another package
manager for macOS. It is growing quickly in popularity. Assuming you
have already installed Homebrew and gotten it working:

  - Open the
    [Terminal](http://www.apple.com/macosx/apps/all.html#terminal)
    application and use the following command to install the necessary
    libraries:

<!-- end list -->

```shell
brew install scons pkg-config portaudio libsndfile libogg libvorbis portmidi git taglib libshout protobuf flac libjpeg qt5 chromaprint rubberband fftw vamp-plugin-sdk opusfile lilv lame qtkeychain
```

#### Optional: ModPlug support

To enable [libmodplug](http://modplug-xmms.sourceforge.net/) based
module tracker support.

```shell
brew install libmodplug
```

If you get the error `No available formula for libmodplug` , enter the
following:

```shell
brew create http://sourceforge.net/projects/modplug-xmms/files/latest/download
```

Enter Formula name `libmodplug` if asked for, then enter:

```shell
brew install libmodplug
```

#### Optional: Alternative MP3/AAC support

Mixxx supports using macOS-provided versions of the MP3 and AAC codec, so you do not need this step for MP3/AAC support. If you don't want to
use the macOS versions of these codecs you can build the codecs into Mixxx directly. To do this, you have to install the MP3 and AAC codecs using Homebrew:

```shell
brew install libid3tag libmad mp4v2 faad2
```

### Method 2: Use a pre-built environment

**These instructions only work for Mixxx 2.3 and later.**

The Mixxx build server produces pre-built macOS build environments.

See
[downloads.mixxxx.org](http://downloads.mixxx.org/builds/buildserver/)
and select the series of Mixxx you would like to develop for (e.g.
`2.3.x-macosx`). Check
[build/osx/golden\_environment](https://github.com/mixxxdj/mixxx/blob/master/build/osx/golden_environment)
to see the current official version. Download the archive and extract it wherever.

There is currently an issue with a hardcoded path in the PkgConfig file for taglib in the build environment, so you must fix it before building or your build will fail to link at the end.
```shell
tar xf 2.3-j00004-497fe02e-osx10.11-x86_64-release.tar.gz
export PREBUILT_ENVIRONMENT=/home/YOUR-USERNAME/2.3-j00004-497fe02e-osx10.11-x86_64-release # or wherever you extracted the archive
find "${PREBUILT_ENVIRONMENT}" -name "*.pc" -or -path "*/bin/taglib-config" -exec sed -i".orig" -e "s|/Users/mixxx/bs-2.3-mac/amd64/environment/2.3-j00004-497fe02e-osx10.11-x86_64-release|${PREBUILT_ENVIRONMENT}|g" {} \;
```

### Method 3: Manual

You can of course install all of [Mixxx's dependencies](dependencies) by
hand. We don't recommend it.

<a name="getMixxx"/>

## 3. Get Mixxx

If you want to compile Mixxx, you'll need to download the source code. Either grab the source for the latest release from our [downloads
page](https://www.mixxx.org/download), or checkout a snapshot from our git repository:

  - For the latest development (main) branch: `git clone https://github.com/mixxxdj/mixxx.git`
  - For the latest beta branch: `git clone -b 2.3 https://github.com/mixxxdj/mixxx.git`
  - For the latest stable branch: `git clone -b 2.2 https://github.com/mixxxdj/mixxx.git`

To update to the latest version of a git branch, enter (`cd` into) the
directory you cloned the git repository into and run `git pull`. Refer
to [Using Git](Using%20Git) for more details.

<a name="compile"/>

## 4. Compile and install

Change to the newly created `mixxx` directory:

```shell
cd mixxx
```

Create the folder where the build files will be written and navigate into it:

```shell
mkdir cmake_build && cd cmake_build
```

The next steps you need to follow depend on whether you are using a pre-built environment or you installed dependencies with Homebrew:

### Configure the build for Homebrew dependencies

Run the following cmake command to configure the project with the recommended default settings for development. You don't need to run this command each time you want to build Mixxx, you only need to run this command again whenever you want to change the build settings.

```shell
cmake -DCOREAUDIO=ON -DCMAKE_BUILD_TYPE=Debug -DDEBUG_ASSERTIONS_FATAL=ON -DQt5_DIR=/usr/local/opt/qt5/cmake/Qt5/ -DCMAKE_PREFIX_PATH=/usr/local/opt/ ..
```

### Configure the build for pre-built environment
You don't need to follow this steps each time you want to build Mixxx, you only need to run again the commands on this section whenever you want to change the build settings.

Before configuring the build, make sure to disable macOS Gatekeeper as described in [this article](https://www.imore.com/how-open-apps-anywhere-macos-catalina-and-mojave). Otherwise, macOS will prevent the pre-built environment bundled binaries to execute.

Run the following cmake command to configure the project with the recommended default settings for development.

```shell
export PREBUILT_ENVIRONMENT=/home/YOUR-USERNAME/2.3-j00004-497fe02e-osx10.11-x86_64-release # path where you extracted the build environment archive
cmake -DCOREAUDIO=ON -DCMAKE_BUILD_TYPE=Debug -DDEBUG_ASSERTIONS_FATAL=ON -DQt5_DIR=${PREBUILT_ENVIRONMENT}/Qt-5.12.3/lib/cmake/Qt5 -DCMAKE_PREFIX_PATH=${PREBUILT_ENVIRONMENT} ..
```

Now you can enable Gatekeeper again as described in this [article](https://www.imore.com/how-open-apps-anywhere-macos-catalina-and-mojave).

### Build Mixxx
Now you are ready to build Mixxx. To build Mixxx simply run the following command. Note that this has to be run inside the `cmake_build` folder:

```shell
cmake --build .
```

If the build succeeds, there will be a `run-mixxx.sh` script in the current directory that you can run:

```shell
./run-mixxx.sh
```

You can pass arguments to this as if you were running the `mixxx` binary directly. For example:

```shell
./run-mixxx.sh --logLevel debug
```

You can run the `mixxx` binary directly, but you would need to set the `QT_QPA_PLATFORM_PLUGIN_PATH` environment variable to point to the `plugins` directory under the Qt directory in the build environment.

### Building a DMG image with an .app bundle inside
Generating the .app has some expensive scanning and relinking steps. So, for development, we recommend using the bare binary instead of creating a bundle. Generally you would only need to build a bundle locally if you are working on the bundle building process.

Add `-DMACOS_BUNDLE=ON` to the first `cmake` command above when configuring the build.

To sign the `.app` bundle inside the DMG image, add `-DAPPLE_CODESIGN_IDENTITY=<your signing identity>` to the `cmake` command. This must be done at the initial `cmake` configure step, not when running `cpack` later. You can run `security find-identity -p codesigning` to find what identities you have installed on your keychain.

To create the DMG image with the .app bundle inside, run
```shell
cpack -G DragNDrop
```
You can run the bundle by double clicking the DMG image in Finder then dragging and dropping the Mixxx.app file inside to /Applications or wherever you would like.

## 5. Configure your development tools

Now that you can build Mixxx, learn about [developer
tools](https://github.com/mixxxdj/mixxx/wiki/Developer-Tools) that make Mixxx development easier.
