echo "CLEANING BUILD FILES..."

echo 'Cleaning object files...'
foreach i { *.o }
    Rm $i
endfor

echo 'Cleaning executables...'
foreach i { *.tos }
    Rm $i
endfor

