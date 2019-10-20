#!/bin/bash

################ Command Line Interface ##################
print_add_help () {
	echo "Help:"
	echo "Add new dependencies to the project. Dependency information is stored inside the project so that other developers are able to easily get them"
	echo
	echo "Usage: ./dependency.sh add TYPE"
	echo
	echo "TYPE:"
	echo "	git [GIT REPOSITORY]: add a git dependency to the project, where GIT REPOSITORY is the same URL you'd use to clone a project"
}

if [ "$2" == "--help" ]; then
	print_add_help
	exit 0
elif [ "$2" == "git" ]; then
	DEPENDENCY_TYPE="$2"
	if [ "$3" != "" ]; then
		DEPENDENCY_GIT_URL="$3"
	else
		echo "Error: unspecified git repository"
		echo
		print_add_help
		exit 1
	fi
else
	echo "Error: unknown dependency type"
	echo
	print_add_help
	exit 1
fi
################ Command Line Interface ##################

if [ "$DEPENDENCY_TYPE" == "git" ]; then
	if [ -f "$PROJECT_ROOT/.assertions/dependency_manager/install_all.sh" ]; then
		DEPENDENCY_ALREADY_EXISTS=$(cat "$PROJECT_ROOT/.assertions/dependency_manager/install_all.sh" | grep -o "${DEPENDENCY_GIT_URL}")
	fi
	if [ "$DEPENDENCY_ALREADY_EXISTS" == "" ]; then
		echo "./.assertions/dependency_manager/install_from_git.sh ${DEPENDENCY_GIT_URL}" >> "./.assertions/dependency_manager/install_all.sh"
	else
		echo "Error: dependency already added"
		exit 1
	fi
fi

cd "$PROJECT_ROOT"
bash "$PROJECT_ROOT/.assertions/dependency_manager/install_all.sh"

