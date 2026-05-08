rm -rf ./build

mkdir build

# Qt6 WASM build — requires:
#   QT6_WASM_REPO : path to Qt6 built for wasm-emscripten
#   GASMAN_REPO   : path to this repository
#   EMSDK_REPO    : path to emsdk (activate version 3.1.56 before running)
#
# One-time Qt6 WASM build of Qt itself:
#   cd $EMSDK_REPO && ./emsdk install 3.1.56 && ./emsdk activate 3.1.56
#   source $EMSDK_REPO/emsdk_env.sh
#   cd $QT6_WASM_REPO && ./configure -xplatform wasm-emscripten \
#       -feature-thread -no-dbus -nomake examples -nomake tests \
#       -prefix $QT6_WASM_REPO/qtbase && cmake --build . --parallel

(cd build; \
  source $EMSDK_REPO/emsdk_env.sh; \
  $QT6_WASM_REPO/qtbase/bin/qmake $GASMAN_REPO/src/gasman.pro; \
  emmake make -j$(nproc))

$GASMAN_REPO/bin/gasmanonline-run.sh
