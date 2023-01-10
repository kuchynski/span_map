SRC_FILES_CPP = ./main.cpp

cpp: $(SRC_FILES_CPP)
	g++ -std=c++2a -fconcepts -O2 -o test_app $^

clean: rm -f *.o
