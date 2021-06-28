The purpose of this project is to create a proof-of-concept for implementing a concurrent graph class the same way a
linked list can be used as a proof-of-concept for a tree class. This project uses hand-over-hand locking to avoid
race conditions. Both building the tree and searching for a node in the tree are blocking functions.

Tree.cpp:
Full class containing both searching and building functions

TreeSearch.cpp:
Used for separate testing

TreeBuild.cpp:
Used for separate testing

execute.sh:
Used to execute a program some number of times. This is used to look at node values

columns.sh:
Used to find the maximum and minimum number of columns for every row from a text file. This can be used with
execute.sh as follows:
    bash execute.sh > output.txt
    bash columns.sh
