# Color support
if [ -t 1 ] && [ -n "$(tput colors)" ]; then
    # stdout is a terminal with colors
    RED="$(tput setaf 1)"
    GREEN="$(tput setaf 2)"
    YELLOW="$(tput setaf 3)"
    BLUE="$(tput setaf 4)"
    MAGENTA="$(tput setaf 5)"
    CYAN="$(tput setaf 6)"
    WHITE="$(tput setaf 7)"
    BOLD="$(tput bold)"
    RESET="$(tput sgr0)"
else
    # stdout is not a terminal or does not support colors
    RED=""
    GREEN=""
    YELLOW=""
    BLUE=""
    MAGENTA=""
    CYAN=""
    WHITE=""
    BOLD=""
    RESET=""
fi

TESTS=tests

# Check to see if a file exists.
exists() {
    [ -e "$1" ]
}

if ! make > /dev/null; then
    echo "Error: Failed to compile 'life.c'."
    exit 1
fi

if [ ! -x "./life" ]; then
    echo "Error: game executable './life' not found."
    exit 1
fi

if [ ! -d "${TESTS}" ]; then
    echo "Error: '${TESTS}' directory not found."
    exit 1
fi

# Run the the game on each of the testcases
passed=0
failed=0
for testcase in "${TESTS}"/*/; do
    if ! exists "${testcase}input.txt"; then
        echo "Error: Input file 'input.txt' does not exist for '${testcase}'";
        echo -e "Skipping...\n"
        continue
    fi
    if ! exists "${testcase}expected"; then
        echo "Error: Expected output 'expected' does not exist for '${testcase}'"
        echo -e "Skipping...\n"
        continue
    fi
    if ! exists "${testcase}run"; then
        echo "Error: No procedure 'run' to execute the test was found for '${testcase}'";
        echo -e "Skipping...\n"
        continue
    fi

    command=$(head -n 1 "${testcase}/run")
    eval "${command}" > "${testcase}output"
    
    if diff -q "${testcase}output" "${testcase}expected"; then
        pmsg="PASS"
        passed=$((passed + 1))
    else
        pmsg="FAIL"
        failed=$((failed + 1))
    fi

    rm "${testcase}output"

    [ "$pmsg" = "PASS" ] && rowcolor=$GREEN || rowcolor=$RED
    printf "${rowcolor}%-50s %-5s ${RESET}\n" "$(basename ${testcase})" "${pmsg}"
done

echo
echo "TESTING SUMMARY:"
echo "${GREEN}Passed: ${passed}${RESET}"
echo "${RED}Failed: ${failed}${RESET}"
echo "Total:  $(($passed+$failed))"

# Exit with 0 if all tests passed, 1 if any failed.
if [ $failed -eq 0 ]; then
    exit 0
else
    exit 1
fi
