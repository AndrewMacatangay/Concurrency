all: TreeSearch TreeBuild Tree

Tree: Tree.cpp
	g++ -g -Wall -o Tree Tree.cpp -pthread

TreeSearch: TreeSearch.cpp
	g++ -g -Wall -o TreeSearch TreeSearch.cpp -pthread

TreeBuild: TreeBuild.cpp
	g++ -g -Wall -o TreeBuild TreeBuild.cpp -pthread

clean:
	rm TreeSearch TreeBuild Tree
