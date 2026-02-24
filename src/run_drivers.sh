#!/bin/bash
# Test Driver Execution Script
# Compiles and runs all test drivers with comprehensive output

echo "================================================================"
echo "  COMP2659 GAME ENGINE - TEST DRIVERS"
echo "================================================================"
echo ""

# Build all drivers (try to use make; continue if it fails)
echo "Building all test drivers..."
echo ""

build_ok=1
if command -v make >/dev/null 2>&1; then
    if make test-drivers; then
        echo ""
        echo "✓ All drivers compiled successfully"
    else
        echo ""
        echo "✗ Failed to compile drivers (make returned non-zero). Continuing to run any existing executables..."
        build_ok=0
    fi
else
    echo "" 
    echo "⚠️  'make' not found in PATH — skipping build. Ensure 'make' is installed to rebuild drivers." 
    build_ok=0
fi

echo ""
echo "================================================================"
echo "  RUNNING TEST DRIVERS"
echo "================================================================"
echo ""

# Array of drivers to run
drivers=("geo_driver" "spike_driver" "lava_driver" "level_driver" "world_driver")

# Run each driver (capture output per-driver and always print summary)
passed=0
failed=0
skipped=0
logs_dir=".driver_logs"
mkdir -p "$logs_dir"

# Ensure we always print a summary even on unexpected termination
print_summary() {
    echo ""
    echo "================================================================"
    echo "  TEST SUMMARY"
    echo "================================================================"
    total=${#drivers[@]}
    echo "Build success: $build_ok"
    echo "Passed: $passed / $total"
    echo "Failed: $failed / $total"
    echo "Skipped (missing exe): $skipped"
    echo ""
    if [ $failed -eq 0 ]; then
        echo "✓ All executed drivers passed (skipped: $skipped)."
        if [ $build_ok -eq 0 ]; then
            echo "Note: build failed or was skipped; results may be from previously-built executables." 
        fi
        exit 0
    else
        echo "✗ Some drivers failed. Check output above or in $logs_dir for details."
        exit 1
    fi
}
trap print_summary EXIT

for driver in "${drivers[@]}"; do
    driver_path="tests/drivers/$driver"
    log_file="$logs_dir/${driver}.log"

    echo "========================================"
    echo "Running: $driver"
    echo "Logs: $log_file"
    echo "========================================"

    # Choose executable name (Windows .exe or bare)
    if [ -f "$driver_path${EXE_EXT}" ]; then
        exec_path="$driver_path${EXE_EXT}"
    elif [ -f "$driver_path" ]; then
        exec_path="$driver_path"
    else
        echo "✗ $driver not found (skipping)" | tee "$log_file"
        ((skipped++))
        echo ""
        continue
    fi

    # Run driver, capture stdout+stderr to log, and preserve exit code
    echo "--- START OUTPUT ($driver) ---" > "$log_file"
    "$exec_path" >> "$log_file" 2>&1
    result=$?
    echo "--- END OUTPUT ($driver) ---" >> "$log_file"

    # Echo a short status to the terminal
    if [ $result -eq 0 ]; then
        echo "";
        echo "✓ $driver passed";
        ((passed++))
    else
        echo "";
        echo "✗ $driver failed (exit code: $result) — see $log_file";
        ((failed++))
    fi

    echo ""
done

# The trap will invoke print_summary on exit
exit 0

# Summary
echo "================================================================"
echo "  TEST SUMMARY"
echo "================================================================"
total=${#drivers[@]}
echo "Build success: $build_ok"
echo "Passed: $passed / $total"
echo "Failed: $failed / $total"
echo "Skipped (missing exe): $skipped"
echo ""

if [ $failed -eq 0 ]; then
    echo "✓ All executed drivers passed (skipped: $skipped)."
    if [ $build_ok -eq 0 ]; then
        echo "Note: build failed or was skipped; results may be from previously-built executables." 
    fi
    exit 0
else
    echo "✗ Some drivers failed. Check output above."
    exit 1
fi
