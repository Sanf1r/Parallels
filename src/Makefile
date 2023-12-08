FLAGSO3=-O3 -Wall -Wextra -Werror -std=c++17 -pedantic -pthread -fopenmp

.PHONY: all
all:  unzip_ex ant gauss winograd

.PHONY: unzip_ex
unzip_ex:
	unzip -o Gauss/Examples/Archive.zip -d ./Gauss/Examples
	unzip -o Ant/Examples/Archive.zip -d ./Ant/Examples
	unzip -o Winograd/MatrixExamples/Archive.zip -d ./Winograd/MatrixExamples

.PHONY: ant
ant:
	g++ $(FLAGSO3) $(ASAN) Ant/main.cc Ant/Model/model.cc Ant/Model/adj_matrix.cc \
	Ant/Model/ant.cc Ant/Model/graph_algorithms.cc Ant/Model/ants_logic.cc \
	Ant/Model/graph.cc Ant/View/view.cc \
	-o TSP
	
.PHONY: winograd
winograd:
	g++ $(FLAGSO3) $(ASAN) Winograd/main.cc Winograd/Model/model.cc Winograd/View/view.cc \
    -o WinogradAlgo
	
.PHONY: gauss
gauss:
	g++ $(FLAGSO3) $(ASAN) ./Gauss/main.cc ./Gauss/Model/model.cc ./Gauss/View/view.cc \
    -o SSLE

.PHONY: clean
clean:
	rm -f TSP
	rm -f WinogradAlgo

.PHONY: lint_ant
lint_ant:
	cp ../materials/linters/.clang-format .
	clang-format -i ./Ant/*.cc ./Ant/Model/*.cc ./Ant/View/*.cc \
	./Ant/Model/*.h ./Ant/View/*.h ./Ant/Controller/*.h
	clang-format -n ./Ant/*.cc ./Ant/Model/*.cc ./Ant/View/*.cc \
	./Ant/Model/*.h ./Ant/View/*.h ./Ant/Controller/*.h
	rm -rf .clang-format

.PHONY: lint_gauss
lint_gauss:
	cp ../materials/linters/.clang-format .
	clang-format -i ./Gauss/*.cc ./Gauss/Model/*.cc ./Gauss/View/*.cc \
	./Gauss/Model/*.h ./Gauss/View/*.h ./Gauss/Controller/*.h
	clang-format -n ./Gauss/*.cc ./Gauss/Model/*.cc ./Gauss/View/*.cc \
	./Gauss/Model/*.h ./Gauss/View/*.h ./Gauss/Controller/*.h
	rm -rf .clang-format

.PHONY: lint_winograd
lint_winograd:
	cp ../materials/linters/.clang-format .
	clang-format -i ./Winograd/*.cc ./Winograd/Model/*.cc ./Winograd/View/*.cc \
	./Winograd/Model/*.h ./Winograd/View/*.h ./Winograd/Controller/*.h
	clang-format -n ./Winograd/*.cc ./Winograd/Model/*.cc ./Winograd/View/*.cc \
	./Winograd/Model/*.h ./Winograd/View/*.h ./Winograd/Controller/*.h
	rm -rf .clang-format

.PHONY: cppcheck_ant
cppcheck_ant:
	cppcheck --enable=all --suppress=missingIncludeSystem \
	--suppress=unusedFunction --std=c++17 --language=c++ ./Ant/*.cc ./Ant/Model/*.cc ./Ant/View/*.cc \
	./Ant/Model/*.h ./Ant/View/*.h ./Ant/Controller/*.h

.PHONY: cppcheck_gauss
cppcheck_gauss:
	cppcheck --enable=all --suppress=missingIncludeSystem \
	--suppress=unusedFunction --std=c++17 --language=c++ ./Gauss/*.cc ./Gauss/Model/*.cc ./Gauss/View/*.cc \
	./Gauss/Model/*.h ./Gauss/View/*.h ./Gauss/Controller/*.h

.PHONY: cppcheck_winograd
cppcheck_winograd:
	cppcheck --enable=all --suppress=missingIncludeSystem \
	--suppress=unusedFunction --std=c++17 --language=c++ ./Winograd/*.cc ./Winograd/Model/*.cc \
	 ./Winograd/View/*.cc ./Winograd/Model/*.h ./Winograd/View/*.h ./Winograd/Controller/*.h

