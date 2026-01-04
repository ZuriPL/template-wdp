run: $(SOURCE_CODE) $(EXE)
	make $(EXE)
	make test
	echo "\n----------\n"
	make valgrind

test: $(SOURCE_CODE) $(EXE)
	echo "Valgrind = false"
	echo ""
	@for dir in $(TESTS_LIST); do \
  		echo Paczka $$dir:; \
		echo; \
  		$(TESTER) $$dir $(TESTS_MAX) false;  \
	done;

valgrind: $(SOURCE_CODE) $(EXE)
	echo "Valgrind = true"
	echo ""
	@for dir in $(TESTS_LIST); do \
  		echo Paczka $$dir:; \
		echo; \
  		$(TESTER) $$dir $(TESTS_MAX_VALGRIND) true;  \
	done;

$(EXE): $(SOURCE_CODE)
	$(COMPILER) @opcje $(SOURCE_CODE) -o $(EXE)