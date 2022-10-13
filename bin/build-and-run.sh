$QT5_REPO/qt5-build/qtbase/bin/qmake $GASMAN_REPO/src/gasman.pro

emmake make -j4

$GASMAN_REPO/bin/gasmanonline-run.sh