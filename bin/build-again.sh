rm -rf ./build

mkdir build

# Qt6 WASM build (see build-and-run.sh for full setup notes)
(cd build; \
  source $EMSDK_REPO/emsdk_env.sh; \
  $QT6_WASM_REPO/qtbase/bin/qmake $GASMAN_REPO/src/gasman.pro; \
  emmake make -j$(nproc))

