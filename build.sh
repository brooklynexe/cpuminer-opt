#!/usr/bin/env bash
set -e

echo "==========================================="
echo "   cpuminer-opt-cpupower - Multi-Arch Build"
echo "==========================================="

ROOT_DIR="$(pwd)"
OS="$(uname -s)"
ARCH="$(uname -m)"
CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu)

# -------- Linux Native --------
echo "[*] Building native Linux ($ARCH)..."
./autogen.sh || true
./configure CFLAGS="-O3 -march=native"
make -j${CORES}
cp cpuminer $ROOT_DIR/cpuminer-linux-$ARCH

# -------- macOS --------
if [[ "$OS" == "Darwin" ]]; then
    echo "[*] Building for macOS..."
    brew install automake autoconf pkg-config gmp jansson openssl curl || true
    ./autogen.sh || true
    ./configure CFLAGS="-O3 -march=native" \
        CPPFLAGS="-I$(brew --prefix openssl)/include" \
        LDFLAGS="-L$(brew --prefix openssl)/lib"
    make clean && make -j${CORES}
    cp cpuminer $ROOT_DIR/cpuminer-macos
fi

# -------- Windows Cross (mingw-w64) --------
if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    echo "[*] Cross-compiling for Windows x86_64..."
    make clean || true
    ./configure --host=x86_64-w64-mingw32 CFLAGS="-O3"
    make -j${CORES}
    cp cpuminer.exe $ROOT_DIR/cpuminer-win64.exe
fi

if command -v i686-w64-mingw32-gcc >/dev/null 2>&1; then
    echo "[*] Cross-compiling for Windows x86..."
    make clean || true
    ./configure --host=i686-w64-mingw32 CFLAGS="-O3"
    make -j${CORES}
    cp cpuminer.exe $ROOT_DIR/cpuminer-win32.exe
fi

# -------- Android (NDK) --------
if [[ -n "$NDK" ]]; then
    echo "[*] Building for Android (arm64-v8a + armeabi-v7a)..."
    $NDK/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk APP_ABI="arm64-v8a armeabi-v7a"
    mkdir -p $ROOT_DIR/android-build
    cp -r libs/* $ROOT_DIR/android-build/
fi

echo "==========================================="
echo " Build finished!"
echo " Outputs in: "
ls -1 cpuminer* 2>/dev/null || true
ls -1 *.exe 2>/dev/null || true
ls -1 android-build/*/cpuminer 2>/dev/null || true
echo "==========================================="
