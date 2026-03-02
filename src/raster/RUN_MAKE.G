echo 'Cleaning object files...'
foreach i { *.o }
    Rm $i
endfor

echo 'Cleaning executables...'
foreach i { *.tos }
    Rm $i
endfor

echo 'Complete, running make...'
make -f tmake

echo 'Build finished! Run rtest.tos to start the test.'

echo 'Cleaning object files...'
CLEAN.G

echo 'Cleaning object files...'
foreach i { *.o }
    Rm $i
endfor
