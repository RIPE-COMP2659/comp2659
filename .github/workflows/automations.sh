#!/bin/bash
set -e  # Exit on any error

# ==============================================================================
# GitHub Actions Automation Script
# Runs test coverage and memory leak detection for C project
# ==============================================================================

# ------------------------------------------------------------------------------
# 1. Run Tests with Coverage
# ------------------------------------------------------------------------------
echo "# AUTOMATIONS AND TESTS"
echo "" >> $GITHUB_STEP_SUMMARY
echo '```' >> $GITHUB_STEP_SUMMARY
cd ../../src && make coverage >> $GITHUB_STEP_SUMMARY 2>&1
echo '```' >> $GITHUB_STEP_SUMMARY

# ------------------------------------------------------------------------------
# 2. Generate Coverage Report
# ------------------------------------------------------------------------------

echo "" >> $GITHUB_STEP_SUMMARY
echo "## Code Coverage Details" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY

if ls *.gcov 1> /dev/null 2>&1; then
    # Coverage files found - display first 20 lines of each
    for file in *.gcov; do
        echo "<details>" >> $GITHUB_STEP_SUMMARY
        echo "<summary><b>$file</b></summary>" >> $GITHUB_STEP_SUMMARY
        echo "" >> $GITHUB_STEP_SUMMARY
        echo '```' >> $GITHUB_STEP_SUMMARY
        cat "$file" | head -20 >> $GITHUB_STEP_SUMMARY
        echo '```' >> $GITHUB_STEP_SUMMARY
        echo "</details>" >> $GITHUB_STEP_SUMMARY
        echo "" >> $GITHUB_STEP_SUMMARY
    done
else
    # No coverage files generated
    echo " No coverage files found" >> $GITHUB_STEP_SUMMARY
fi

# ------------------------------------------------------------------------------
# 3. Run Valgrind Memory Check
# ------------------------------------------------------------------------------

echo "" >> $GITHUB_STEP_SUMMARY
echo "## Valgrind Memory Check" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo '```' >> $GITHUB_STEP_SUMMARY
make valgrind >> $GITHUB_STEP_SUMMARY 2>&1
echo '```' >> $GITHUB_STEP_SUMMARY
