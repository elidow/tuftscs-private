echo -----------------------------------
echo -----------------------------------
echo Running Unit Tests
echo -----------------------------------
echo -----------------------------------

echo Testing halt
echo -----------------------------------
./um halt.um
echo -----------------------------------

echo Testing output
echo -----------------------------------
./um output.um > output.1
echo -----------------------------------

echo Testing load-value
echo -----------------------------------
./um load-value.um > load-value.1
echo -----------------------------------

echo Testing halt-verbose
echo -----------------------------------
./um halt-verbose.um
echo -----------------------------------

echo Testing add
echo -----------------------------------
./um add.um > add.1
echo -----------------------------------

echo Testing print-six
echo -----------------------------------
./um print-six.um > print-six.1
echo -----------------------------------

echo Testing Multiply
echo -----------------------------------
./um multiply.um > multiply.1
echo -----------------------------------

echo Testing Divide
echo -----------------------------------
./um divide.um > divide.1
echo -----------------------------------

echo Testing nand
echo -----------------------------------
./um nand.um > nand.1
echo -----------------------------------

echo Testing conditional-move
echo -----------------------------------
./um conditional-move.um > conditional-move.1
echo -----------------------------------

echo Testing input
echo -----------------------------------
./um input.um < input.0 > input.1
echo -----------------------------------

echo Testing adv-output
echo -----------------------------------
./um adv-output.um < adv-output.0 > adv-output.1
echo -----------------------------------

echo Testing map-segment
echo -----------------------------------
./um map-segment.um > map-segment.1
echo -----------------------------------

echo Testing segmented-store
echo -----------------------------------
./um segmented-store.um
echo -----------------------------------

echo Testing segmented-load
echo -----------------------------------
./um segmented-load.um > segmented-load.1
echo -----------------------------------

echo Testing unmap-segment
echo -----------------------------------
./um unmap-segment.um > unmap-segment.1
echo -----------------------------------

echo Testing load-program
echo -----------------------------------
./um load-program.um
echo -----------------------------------

echo -----------------------------------
echo TESTING COMPLETE
echo -----------------------------------
echo -----------------------------------