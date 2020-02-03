#!/usr/bin/env bash

set -e

git submodule update --remote || echo "warning: LatAnalyze git update failed" 1>&2
INITDIR=$(pwd)
cd dependencies/LatAnalyze/ci-scripts
if [ "$(cat ${INITDIR}/dependencies/.latan 2>/dev/null)" != "$(git rev-parse HEAD)" ]; then
  MACOSX_DEPLOYMENT_TARGET=10.14 ./install-latan.sh ../../prefix 2>&1
  git rev-parse HEAD > ${INITDIR}/dependencies/.latan
fi
cd ${INITDIR}
