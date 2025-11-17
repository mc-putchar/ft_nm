#!/bin/bash

set -euo pipefail

export LC_COLLATE=C

RED='\033[31m'
GRN='\033[32m'
YLW='\033[33m'
MAG='\033[35m'
MAB='\033[1;35m'
CYA='\033[36m'
CYB='\033[1;36m'
NC='\033[0m'
LOG_FILE="ft_nm_test_fails_$(date +%Y%mYd_%H%M%S).log"

usage() {
	echo -e "$${YLW}Usage:${NC} $0 [${CYA}NM_FLAGS${NC}] ${YLW}--${NC} <${CYA}PATH_1${NC}> [${CYA}PATH_2${NC}]..."
	echo -e "  ${CYA}NM_FLAGS${NC}: Optional flags to pass to both nm commands."
	echo -e "  ${YLW}--${NC}: Mandatory separator between flags and paths."
	echo -e "  ${CYA}PATH${NC}: File or directory to test."
	echo -e "\n${YLW}Log file will be created at:${NC} $LOG_FILE"
}

test_file() {
	local file_path="$1"
	local flags="$2"
	local temp_ft="/tmp/ft_nm_out_$$.txt"
	local temp_std="/tmp/std_nm_out_$$.txt"

	if ! [[ -x "./ft_nm" ]]; then
		echo -e "${RED}Error: ${NC} Cannot find or execute './ft_nm'. Ensure the binary exists and is executable."
		exit 1
	fi

	./ft_nm ${flags} "${file_path}" > "${temp_ft}" 2>&1
	local ft_nm_exit=$?
	nm ${flags} "${file_path}" > "${temp_std}" 2>&1
	local nm_exit=$?

	if diff -u "${temp_ft}" "${temp_std}" > /dev/null && [[ $ft_nm_exit -eq $nm_exit ]]; then
		echo -e "${file_path}: [${GRN}OK${NC}]"
	else
		echo -e "${file_path}: [${RED}KO${NC}]"
		echo "================================================================================" >> "$LOG_FILE"
		echo "Test failed for file: ${file_path}" >> "$LOG_FILE"
		echo "Flags used: ${flags:-<none>}" >> "$LOG_FILE"
		echo "ft_nm Exit Code: ${ft_nm_exit}, nm Exit Code: ${nm_exit}" >> "$LOG_FILE"
		echo "----------------- Unified Diff -----------------" >> "$LOG_FILE"
		diff -u "${temp_std}" "${temp_ft}" >> "$LOG_FILE" || true
		echo "================================================================================" >> "$LOG_FILE"
    fi

	rm -f "${temp_ft}" "${temp_std}"
}

process_path() {
	local path="$1"
	local flags="$2"

	if [[ -f "$path" ]]; then
		test_file "$path" "$flags"
	elif [[ -d "$path" ]]; then
		echo -e "${YLW}Processing dir: ${path}${NC}"
		find "$path" -type f -print0 | while IFS= read -r -d $'\0' file; do
			if [[ "$file" =~ /.\w+ ]]; then
				continue
			fi
			test_file "$file" "$flags"
		done
	else
		echo -e "${path}: [${YLW}SKIP${NC}] (Path is not a regular file or directory)"
	fi
}

if [[ $# -eq 0 ]]; then
	usage
	exit 1
fi

FLAGS=''
PATHS=()
MODE="FLAGS"

for arg in "$@"; do
	if [[ "$arg" == "--" ]]; then
		MODE="PATHS"
		continue
	fi
	if [[ "$MODE" == "FLAGS" ]]; then
		FLAGS+="$arg "
	else
		PATHS+=("$arg")
	fi
done

FLAGS=$(echo "$FLAGS"|xargs)

echo -e "${CYA}--- ft_nm AutoTester ---${NC}"
echo -e "${CYA}Flags used:${NC} ${FLAGS:-<none>}"
echo -e "${CYA}Paths to process:${NC} ${PATHS[*]}"
echo "--------------------------------"

echo "ft_nm Test Log - $(date)" > "$LOG_FILE"
echo "Flags: ${FLAGS:-<none>}" >> "$LOG_FILE"
echo "--------------------------------" >> "$LOG_FILE"

for path in "${PATHS[@]}"; do
    process_path "$path" "$FLAGS"
done

echo "--------------------------------"
if grep -q "Test failed" "$LOG_FILE"; then
    echo -e "${RED}TESTS COMPLETED WITH FAILURES.${NC}"
    echo -e "${YLW}Detailed diffs logged to:${NC} ${LOG_FILE}"
else
    echo -e "${GRN}ALL TESTS PASSED SUCCESSFULLY.${NC}"
    rm -f "$LOG_FILE" || true
fi

trap 'rm -f /tmp/ft_nm_out_$$* /tmp/std_nm_out_$$*' EXIT

exit 0
