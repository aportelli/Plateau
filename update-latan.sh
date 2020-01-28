#!/usr/bin/env bash

set -e

git submodule update --remote
INITDIR=$(pwd)
cd dependencies/LatAnalyze/ci-scripts
if [ "$(cat ${INITDIR}/dependencies/.latan 2>/dev/null)" != "$(git rev-parse HEAD)" ]; then
  MACOSX_DEPLOYMENT_TARGET=10.14 ./install-latan.sh ../../prefix
  git rev-parse HEAD > ${INITDIR}/dependencies/.latan
fi
cd ${INITDIR}
