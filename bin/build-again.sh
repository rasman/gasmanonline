rm -rf ./build

mkdir build

(cd build;$QT5_REPO/qtbase/bin/qmake $GASMAN_REPO/src/gasman.pro;emmake make -j4)

