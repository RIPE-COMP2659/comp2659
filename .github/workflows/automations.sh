#!/bin/bash
set -e  # Exit on any error

# ==============================================================================
# GitHub Actions Automation Script
# Runs test coverage and memory leak detection for C project
# ==============================================================================

# ------------------------------------------------------------------------------
# 1. Run Tests with Coverage
# ------------------------------------------------------------------------------
echo "# AUTOMATIONS AND TESTS" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "## Test Execution" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo '```' >> $GITHUB_STEP_SUMMARY
cd ../../src && make coverage >> $GITHUB_STEP_SUMMARY 2>&1
echo '```' >> $GITHUB_STEP_SUMMARY

# ------------------------------------------------------------------------------
# 2. Generate Coverage Report
# ------------------------------------------------------------------------------

echo "" >> $GITHUB_STEP_SUMMARY
echo "---" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "## Code Coverage Analysis" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "### Coverage Report Files" >> $GITHUB_STEP_SUMMARY
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
echo "---" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "## Memory Leak Detection" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "### Valgrind Analysis Results" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo '```' >> $GITHUB_STEP_SUMMARY
make valgrind >> $GITHUB_STEP_SUMMARY 2>&1
echo '```' >> $GITHUB_STEP_SUMMARY

# ------------------------------------------------------------------------------
# 4. Generate SPECIFICATIONS.docx
# ------------------------------------------------------------------------------

repo_root="${GITHUB_WORKSPACE:-$(cd "$(dirname "$0")/../.." && pwd)}"
spec_source="$repo_root/SPECIFICATIONS.md"
spec_html="$repo_root/SPECIFICATIONS.html"
spec_docx="$repo_root/RIPE_SPECIFICATIONS.docx"

echo "" >> $GITHUB_STEP_SUMMARY
echo "---" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "## Specifications Document" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "### Generation Output" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo '```' >> $GITHUB_STEP_SUMMARY
echo "Repo root: $repo_root" >> $GITHUB_STEP_SUMMARY
if [ -f "$spec_source" ]; then
    pandoc "$spec_source" -o "$spec_html" >> $GITHUB_STEP_SUMMARY 2>&1
    pandoc "$spec_html" -o "$spec_docx" >> $GITHUB_STEP_SUMMARY 2>&1
else
    echo "SPECIFICATIONS.md not found at $spec_source" >> $GITHUB_STEP_SUMMARY
    exit 1
fi
echo '```' >> $GITHUB_STEP_SUMMARY
