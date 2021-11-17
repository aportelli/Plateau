#!/usr/bin/env bash

if (( $# != 1 )); then
  echo "usage: `basename $0` <ntasks>" 1>&2
  exit 1
fi
NTASKS=$1

set -e

git submodule update --remote || echo "warning: LatAnalyze git update failed" 1>&2
INITDIR=$(pwd)
cd dependencies/LatAnalyze/ci-scripts
if [ "$(cat ${INITDIR}/dependencies/.latan 2>/dev/null)" != "$(git rev-parse HEAD)" ]; then
  MACOSX_DEPLOYMENT_TARGET=11 ./install-latan.sh ../../prefix ${NTASKS} 2>&1
  git rev-parse HEAD > ${INITDIR}/dependencies/.latan
fi
cd ${INITDIR}
