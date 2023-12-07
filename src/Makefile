FLAGS=-Wall -Wextra -Werror -std=c++17 -pedantic -pthread -fopenmp
FLAGSO3=-O3 -Wall -Wextra -Werror -std=c++17 -pedantic -pthread -fopenmp
GTEST=-lgtest -lgtest_main -pthread
COVER=-fprofile-arcs -ftest-coverage
ASAN=#-fsanitize=address

.PHONY: all
all:  build

.PHONY: build
build:
	g++ $(FLAGSO3) $(ASAN) main.cc Model/model.cc Model/adj_matrix.cc Model/ant.cc \
	Model/graph_algorithms.cc Model/ants_logic.cc ./Model/graph.cc View/view.cc \
	-o SimpleNavigator
	rm -f *.a *.o

.PHONY: debug
debug:
	g++ -g $(FLAGS) main.cc Model/model.cc Model/adj_matrix.cc Model/ant.cc \
	Model/graph_algorithms.cc Model/ants_logic.cc ./Model/graph.cc View/view.cc \
	-o SimpleNavigator
	rm -f *.a *.o

.PHONY: winograd
winograd:
	g++ $(FLAGSO3) Winograd/main.cc Winograd/Model/model.cc Winograd/View/view.cc \
    -o WinogradAlgo
	rm -f *.a *.o

.PHONY: clean
clean:
	rm -f SimpleNavigator
	rm -f test
	rm -f *.a *.o
	rm -f *.info *.gcda *.gcno *.gcov
	rm -rf coverage
	rm -rf doxygen/html doxygen/latex

.PHONY: tests
tests:
	rm -f *.gcda *.gcno
	g++ $(FLAGS) $(ASAN) $(GTEST) $(COVER) \
	./Tests/tests.cc ./Model/model.cc ./Model/adj_matrix.cc ./Model/ants_logic.cc \
	./Model/ant.cc ./Model/graph_algorithms.cc ./Model/graph.cc -o test
	./test
	geninfo . -b . -o ./coverage.info --no-external --ignore-errors mismatch,mismatch
	genhtml coverage.info -o ./coverage

.PHONY: gcov_report
gcov_report:
	open coverage/index.html

.PHONY: dvi
dvi:
	cd doxygen && doxygen Doxyfile && open html/index.html

.PHONY: dist
dist:
	tar czvf SimpleNavigator.tar.gz ./Model ./View ./Controller ./doxygen/Doxyfile \
	main.cc Makefile

.PHONY: lint
lint:
	cp ../materials/linters/.clang-format .
	clang-format -i *.cc ./Model/*.cc ./View/*.cc \
	./Model/*.h ./View/*.h ./Controller/*.h
	clang-format -n *.cc ./Model/*.cc ./View/*.cc \
	./Model/*.h ./View/*.h ./Controller/*.h
	rm -rf .clang-format

.PHONY: leaks
leaks:
	leaks -atExit -- ./test

.PHONY: cppcheck
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem \
	--suppress=unusedFunction --std=c++17 --language=c++ *.cc ./Model/*.cc ./View/*.cc \
	./Model/*.h ./View/*.h ./Controller/*.h

