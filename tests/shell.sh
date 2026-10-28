#!/bin/bash

YELLOW="\033[33m"
GREEN="\033[32m"
BOLD="\033[1m"
RESET="\033[0m"
RED="\033[31m"
MAGENTA="\033[35m"


print_usage() {
    echo -e "${BOLD}C Utility Diff Tester${RESET}"
    echo -e "Description: Automates testing of custom C binaries (via './cunix') against system utilities using 'diff -u'."
    echo -e ""
    echo -e "${BOLD}Usage:${RESET} $0 [options]"
    echo -e "  -h, --help     Display this help message"
    echo -e ""
    echo -e "${BOLD}How it works:${RESET}"
    echo -e "1. You will be prompted for a test command (e.g., 'cat', 'cat -n', 'grep -i')."
    echo -e "2. You will be prompted for a source directory containing test files."
    echo -e "3. The script compares './cunix [command]' output against the system '[command]'."
    echo -e "4. Results are logged into 'test_res/', 'actual_res/', and 'diff_res/'."
    exit 0
}

# Check for help flag
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    print_usage
fi


echo "Enter test command example cat, cat -flags, grep, grep -flags, tail, tail -flags"
read command

echo "Enter source test directory"
read src

if [ ! -d "$src" ]; then
    echo -e "${RED}${BOLD}invalid directory${RESET}"
    exit
fi

mkdir -p test_res
mkdir -p actual_res
mkdir -p diff_res

for items in "$src"/*; do
    if [ -f "$items" ]; then
        file_name=$(basename "$items")

        #Run custom cunix
        ./cunix $command "$items" > "test_res/$file_name" 2>&1

        #Run the system utility using $command
        $command "$items" > "actual_res/$file_name" 2>&1

        # 3. Compare them immediately with diff inside the loop
        if diff -u "actual_res/$file_name" "test_res/$file_name" >> diff_res/diff.txt; then
            echo -e "${GREEN}\"$items\" Passed${RESET}"
        else
            echo -e "${RED}\"$items\" Diff found! Logged to diff_res/diff.txt${RESET}" 
        fi
    else
        echo "${RED}invalid files${RESET}"
        exit
    fi
done

# --- End of Script Post-Action Prompt ---
echo ""
echo -e "${MAGENTA}Testing completed! What would you like to do next?${RESET}"
echo "Type 'REMOVE' to delete result folders, or 'exit' to close:"
read input

if [[ "$input" == "exit" ]]; then
    echo -e "${YELLOW} Exiting...${RESET}"
    exit     

elif [[ "$input" == "REMOVE" ]]; then
    rm -r test_res actual_res diff_res
    echo -e "${RED}result folders and files deleted${EXIT}"
    exit
fi