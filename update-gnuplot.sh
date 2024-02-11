#!/usr/bin/env bash

set -e

root_dir=$(pwd -P)
git clone --depth 1 --branch master https://github.com/gnuplot/gnuplot.git
cd gnuplot/src/qtterminal
cp QtGnuplotEvent.* QtGnuplotInstance.* QtGnuplotItems.* QtGnuplotScene.* QtGnuplotWidget.* ${root_dir}/src/gnuplot/
cd ..
cp mousecmn.h ${root_dir}/src
cd ${root_dir}
rm -rf gnuplot
