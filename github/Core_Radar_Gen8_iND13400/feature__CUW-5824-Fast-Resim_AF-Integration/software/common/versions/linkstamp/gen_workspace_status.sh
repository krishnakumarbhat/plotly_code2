#!/bin/sh -
echo "STABLE_BUILD_SCM_REVISION $(git rev-parse HEAD)"
if git diff --quiet; then
  echo "STABLE_BUILD_SCM_STATUS clean"
else
  echo "STABLE_BUILD_SCM_STATUS dirty"
fi
