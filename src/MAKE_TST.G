echo 'Cleaning object files...'
foreach i { *.o }
    Rm $i
endfor

echo 'Cleaning executables...'
foreach i { *.tos }
    Rm $i
endfor

echo 'Cleaning test files...'
foreach i { *.ut }
    Rm $i
endfor

echo 'Complete, running make_atr test...'
make -f make_atr test

