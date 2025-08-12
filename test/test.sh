#!/bin/bash

GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
MAGENTA="\033[0;35m"
YELLOW="\033[0;33m"
NC="\033[0m"

TEST_DIR="test"
OBJ_DIR="obj"
CC="gcc"
CC_FLAGS="-Wall -Wextra -Werror -fPIE -pie"

run_single_test() {
    local test_name="$1"
    local test_file="$TEST_DIR/$test_name.c"
    
    if [ ! -f "$test_file" ]; then
        echo -e "${RED}❌ The test called \"$test_name\" was not found!${NC}"
        echo -e "${BLUE}Available tests:${NC}"
        for test in $(find $TEST_DIR -name "*.c" -type f | xargs -I {} basename {} .c); do
            echo -e "${BLUE} => ${MAGENTA}make ${YELLOW}test$test${NC}"
        done
        return 1
    fi
    
    compile_output=$(${CC} ${CC_FLAGS} "$test_file" -L. -lasm -o "test_$test_name" 2>&1)
    
    if [ $? -eq 0 ]; then
        ./test_$test_name
        rm -f test_$test_name
        rm -f .test_log/$test_name.txt
    else
        echo -e "${RED}❌ ft_$test_name compilation failed. Look at .test_log/$test_name.txt${NC}"
        mkdir -p .test_log
        echo "$compile_output" > .test_log/$test_name.txt
        return 1
    fi
}

run_all_tests() {
    echo -e "${GREEN}🧪 Running all tests...${NC}"
    
    mkdir -p .test_log
    
    TEST_FILES=$(find $TEST_DIR -name "*.c" -type f 2>/dev/null)
    if [ -z "$TEST_FILES" ]; then
        echo -e "${RED}❌ No test files found in $TEST_DIR${NC}"
        exit 1
    fi
    
    passed=0
    failed=0
    
    for test_file in $TEST_FILES; do
        test_name=$(basename "$test_file" .c)
        
        compile_output=$(${CC} ${CC_FLAGS} "$test_file" -L. -lasm -o "test_$test_name" 2>&1)
        
        if [ $? -eq 0 ]; then
            test_output=$(./test_$test_name 2>&1 | sed 's/\x1b\[[0-9;]*m//g')
            
            if echo "$test_output" | grep -q "FAIL"; then
                echo -e "${RED}❌ FAIL${NC} $test_name (${MAGENTA}log file =>${NC} .test_log/$test_name.txt)"
                echo "$test_output" > .test_log/$test_name.txt
                failed=$((failed + 1))
            else
                echo -e "${GREEN}✅ PASS${NC} $test_name"
                rm -f .test_log/$test_name.txt
                passed=$((passed + 1))
            fi
            
            rm -f test_$test_name
        else
            echo -e "${RED}❌ FAIL${NC} $test_name (compilation failed)"
            echo "$compile_output" > .test_log/$test_name.txt
            failed=$((failed + 1))
        fi
    done
    
    total=$((passed + failed))
    echo ""
    echo -e "${BLUE}Summary: $total total, ${GREEN}$passed passed${NC}, ${RED}$failed failed${NC}"
}

if [ $# -eq 0 ]; then
    run_all_tests
else
    run_single_test "$1"
fi

if [ -d ".test_log" ] && [ -z "$(ls -A .test_log)" ]; then
    rmdir .test_log
fi