#!/bin/bash

SUCCESSFUL_TESTS=0
FAILED_TESTS=0

default_text_color=7	#color used to reset terminal text to default color. 0 will reset to black and 7 to white. See the tput command documentation for other colors
red_color=`tput setaf 1`
green_color=`tput setaf 2`
reset_color=`tput setaf $default_text_color`
up_line=`tput cuu 1`
clear_line=`tput el 1`

SCRIPT_DIR=$(realpath $(dirname $0))
TESTS_DIR="$SCRIPT_DIR/tests"
BUILD_DIR="$SCRIPT_DIR/build"

determine_current_test_full_name () {
	ESCAPED_TESTS_DIR=$(echo $TESTS_DIR | sed 's/\//\\\//g; s/\./\\\./g')
	ESCAPED_BUILD_DIR=$(echo $TEST_BUILD_DIR | sed 's/\//\\\//g; s/\./\\\./g')
	TEST_NAME=$(echo $CURRENT_TEST | sed "s/^${ESCAPED_TESTS_DIR}\///; s/^${ESCAPED_BUILD_DIR}\///;" | sed "s/^test_//; s/.cpp$//")
	TEST_FULL_NAME="test_${TEST_NAME}"
}

determine_current_test_source_file () {
	TEST_SOURCE_FILE="${TESTS_DIR}/${TEST_FULL_NAME}.cpp"
}

determine_current_test_binary_file () {
	TEST_BINARY_FILE="${BUILD_DIR}/${TEST_FULL_NAME}"
}

if [ "$#" -gt 0 ] && [ "$1" != "all" ]; then
	TESTS=$1
	shift
	until [ -z "$1" ]
	do
		TESTS="${TESTS} ${1}"
		shift
	done
else
	TESTS="${TESTS_DIR}/*.cpp"
fi

FAILED_TESTS=0
SUCCESSFUL_TESTS=0
IGNORED_TESTS=0

"$SCRIPT_DIR/build.sh" --cmake-only

echo	# line feed
echo "-------------------INDIVIDUAL TESTS-------------------"
for CURRENT_TEST in $TESTS
do

	determine_current_test_full_name
	echo "Testing ${TEST_NAME}..."
	if [ -d "${TESTS_DIR}/${CURRENT_TEST}" ]; then
		echo "Executing tests from specific folder not yet supported"
	else
		determine_current_test_source_file
		determine_current_test_binary_file
		if [ -f "$TEST_SOURCE_FILE" ]; then

			"$SCRIPT_DIR/build.sh" --no-cmake  $TEST_FULL_NAME

			BUILD_STATUS=$?
			if [ $BUILD_STATUS -eq 0 ]; then
				TEST_OUTPUT=$($TEST_BINARY_FILE)
				TEST_SUCCESSES=$(echo $TEST_OUTPUT | grep -o "' OK" | wc -l)
				TEST_FAILURES=$(echo $TEST_OUTPUT | grep -o "' failed:" | wc -l)
				SUCCESSFUL_TESTS=`expr $SUCCESSFUL_TESTS + $TEST_SUCCESSES`
				FAILED_TESTS=`expr $FAILED_TESTS + $TEST_FAILURES`
				echo "	${TEST_OUTPUT}" | tr '\n' '\032' | sed -e $(echo -e 's/\032/\\n\\t/g')
			else
				echo "	${red_color}build failed for ${TEST_SOURCE_FILE}${reset_color}"
				FAILED_TESTS=`expr $FAILED_TESTS + 1`
			fi
		else
			echo "${red_color}no source file ${TEST_SOURCE_FILE}${reset_color}"
		fi

	fi

	echo	# line feed

done
echo "-------------------INDIVIDUAL TESTS-------------------"

echo	# line feed
echo "-------------------TESTS SUMMARY-------------------"
if [ $SUCCESSFUL_TESTS -gt 0 ]; then
	echo "${green_color}$SUCCESSFUL_TESTS passed${reset_color}"
else
	echo "$SUCCESSFUL_TESTS passed${reset_color}"
fi
if [ $FAILED_TESTS -gt 0 ]; then
	echo "${red_color}$FAILED_TESTS failed${reset_color}"
else
	echo "$FAILED_TESTS failed${reset_color}"
fi
echo "-------------------TESTS SUMMARY-------------------"

