#!/bin/bash

SCRIPT_PATH=$(realpath $(dirname $0))
BUILD_PATH="${SCRIPT_PATH}/build"

if [ "$1" != "clean" ]; then
	mkdir -p "$BUILD_PATH"
	cd "$BUILD_PATH"

	if [ "$1" != "--no-cmake" ]; then
		BUILD_TARGET=$1
		cmake "$SCRIPT_PATH" -G "Unix Makefiles"
	else
		BUILD_TARGET=$2
	fi

	if [ "$1" != "--cmake-only" ]; then
		make $BUILD_TARGET
	fi
else
	rm -rf "$BUILD_PATH"
fi
