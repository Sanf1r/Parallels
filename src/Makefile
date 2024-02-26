FLAGSO3=-O3 -Wall -Wextra -Werror -std=c++17 -pedantic -pthread -fopenmp

.PHONY: all
all:  unzip_ex ant winograd

.PHONY: unzip_ex
unzip_ex:
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
	
.PHONY: clean
clean:
	rm -f TSP
	rm -f WinogradAlgo
