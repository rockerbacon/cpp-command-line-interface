#!/bin/bash

SCRIPT_DIR=$(dirname $0)
PROJECT_ROOT=$(realpath "$SCRIPT_DIR/../../")
DEPENDENCIES_DIR="$PROJECT_ROOT/external_dependencies"
DEPENDENCIES_LIB_DIR="$DEPENDENCIES_DIR/lib"
DEPENDENCIES_INCLUDE_DIR="$DEPENDENCIES_DIR/include"

##################### Command Line Interface ##########################
DEPENDENCY_GIT_URL="$1"
if [ "$DEPENDENCY_GIT_URL" == "" ]; then
	echo "Error: broken dependency install configuration. Check './.assertions/dependency_manager/install_all.sh'"
	exit 1
fi
##################### Command Line Interface ##########################

rollback_installation () {
	if [ -d "$DEPENDENCY_GIT_CLONE_DIR/.git" ]; then
		echo "Rolling back: deleting '$DEPENDENCY_GIT_CLONE_DIR'"
		rm -rf "$DEPENDENCY_GIT_CLONE_DIR"
	fi
}

DEPENDENCIES_GIT_DIR="$DEPENDENCIES_DIR/git"
mkdir -p "$DEPENDENCIES_GIT_DIR"
cd "$DEPENDENCIES_GIT_DIR"

DEPENDENCY_RELATIVE_GIT_CLONE_DIR=$(echo "$DEPENDENCY_GIT_URL" | grep -oe "\/.*\.git" | sed 's/\///; s/\.git$//')
DEPENDENCY_GIT_CLONE_DIR="$DEPENDENCIES_GIT_DIR/$DEPENDENCY_RELATIVE_GIT_CLONE_DIR"

if [ -d "$DEPENDENCY_GIT_CLONE_DIR" ]; then
	echo "Dependency '$DEPENDENCY_GIT_URL' already downloaded"
else
	git clone "$DEPENDENCY_GIT_URL"
	GIT_EXECUTION_STATUS=$?
	if [ "$GIT_EXECUTION_STATUS" != "0" ]; then
		echo
		print_add_help
		exit 1
	fi
fi

if [ -f "$DEPENDENCY_GIT_CLONE_DIR/.assertions/language" ]; then
	DEPENDENCY_LANGUAGE=$(cat "$DEPENDENCY_GIT_CLONE_DIR/.assertions/language")
	if [ "$DEPENDENCY_LANGUAGE" == "cpp" ]; then
		if [ -d "$DEPENDENCY_GIT_CLONE_DIR/build" ]; then
			echo "Dependency '$DEPENDENCY_GIT_URL' already built"
		else
			"$DEPENDENCY_GIT_CLONE_DIR/build.sh"
			if [ ! -d "$DEPENDENCY_GIT_CLONE_DIR/build/release/lib" ]; then
				echo "Error: dependency is not a lib"
				rollback_installation
				exit 1
			fi
		fi
		ln -s "$DEPENDENCY_GIT_CLONE_DIR/build/release/lib" "$DEPENDENCIES_LIB_DIR/$DEPENDENCY_RELATIVE_GIT_CLONE_DIR"
		ln -s "$DEPENDENCY_GIT_CLONE_DIR/build/release/include" "$DEPENDENCIES_INCLUDE_DIR/$DEPENDENCY_RELATIVE_GIT_CLONE_DIR"
	else
		echo
		echo "Error: project uses Assertions, but it's not meant for this languague. Expected language: 'cpp', language reported by dependency: '${DEPENDENCY_LANGUAGE}'"
		rollback_installation
		exit 1
	fi
else
	echo
	echo "Error: dependencies can only be from projects using the Assertions C++ Framework for now. Support for other projects is still WIP"
	rollback_installation
	exit 1
fi

