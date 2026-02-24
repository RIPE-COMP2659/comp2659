echo 'COMP2659 Game Engine - Test Drivers Build'
echo ''
echo 'Cleaning object files...'
foreach i { *.o }
    Rm $i
endfor

echo 'Building test drivers...'
make test-drivers

echo ''
echo 'Build complete!'
