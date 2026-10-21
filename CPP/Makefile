CXX = g++
CXXFLAGS = -std=c++17 -Wall

SOURCES = main.cpp commands/commands.cpp cpp/array.cpp cpp/flist.cpp cpp/dlist.cpp cpp/stack.cpp cpp/queue.cpp cpp/cbt.cpp

dbms: $(SOURCES) headers/*.h commands/*.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o dbms

clean:
	rm -f dbms
