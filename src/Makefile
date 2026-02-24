prog.tos: block.o spike.o lava.o geo.o level.o world.o main.o
	cc68x -g block.o spike.o lava.o geo.o level.o world.o main.o -o prog.tos

dtypes.h:
	echo 'ERROR: dtypes.h is missing. Copy dtypes.h into the Atari build folder.'
	exit 1

block.o: block.c block.h dtypes.h
	cc68x -g -c block.c

lava.o: lava.c lava.h dtypes.h
	cc68x -g -c lava.c

spike.o: spike.c spike.h dtypes.h
	cc68x -g -c spike.c

geo.o: geo.c geo.h dtypes.h
	cc68x -g -c geo.c

level.o: level.c level.h dtypes.h
	cc68x -g -c level.c

world.o: world.c world.h dtypes.h
	cc68x -g -c world.c

main.o: main.c block.h spike.h lava.h geo.h level.h world.h dtypes.h
	cc68x -g -c main.c

# Raster driver test (Checkpoint 2)
rastdrv.tos: rastdrv.o renderer.o
	cc68x -g rastdrv.o renderer.o -o rastdrv.tos

rastdrv.o: raster_lib/rastdrv.c raster_lib/raster.h dtypes.h
	cc68x -g -c raster_lib/rastdrv.c

<<<<<<< Updated upstream
# Build test executables (pattern rule)
# Each test compiles with its corresponding source file: test_NAME.c compiles with NAME.c
#%$(EXE_EXT): tests/%.c $(UNITY_FILES) $(TEST_SUPPORT_SRC)
%$(EXE_EXT): tests/%.c $(UNITY_FILES) $(TEST_SUPPORT_SRC)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) $(INCLUDES) -DTESTING $< $(TEST_SUPPORT_SRC) $(UNITY_FILES) -o $@

# Build the main program
main: clean $(MAIN_EXE)

$(MAIN_EXE): $(MAIN_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(MAIN_SRC) -o $(MAIN_EXE)

# Clean up unncessary files
coverage: generate-coverage clean-coverage

# Generate coverage report using gcov for all test executables
# Each test only reports coverage for its corresponding source file
generate-coverage: clean test
	$(foreach exec,$(TEST_EXECS:$(EXE_EXT)=),$(GCOV) $(exec)-$(patsubst test_%,%,$(exec)).gcno && ) $(NOOP)
	@echo Coverage report generated. Check *.gcov files for details.

# Clean only coverage files (used before coverage generation)
clean-coverage:
	-$(RM) *.gcda *.gcno

# Clean up build files
clean:
	-$(RM) *.exe *.o *.gcda *.gcno *.gcov *.ut $(TEST_EXECS)

# Run tests with Valgrind memory checking
valgrind: $(TEST_EXECS)
	$(MAKE) test RUN_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(RUN_PREFIX)"

# Commands to run tests, otherwise make may assume these are files.
.PHONY: all main test clean clean-coverage coverage generate-coverage valgrind
=======
renderer.o: raster_lib/renderer.c raster_lib/raster.h dtypes.h
	cc68x -g -c raster_lib/renderer.c
>>>>>>> Stashed changes
