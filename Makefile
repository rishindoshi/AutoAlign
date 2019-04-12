# designate which compiler to use
CXX			= g++
# list of objects used in project
OBJECTS 	= main.o board.o
# name of the executable produced by the top level
EXECUTABLE 	= main 

#Default Flags
CXXFLAGS = -std=c++11 \

# make release - will compile "all" with $(CXXFLAGS) and the -O3 flag
#				 also defines NDEBUG so that asserts will not check
release: CXXFLAGS += -O3 -DNDEBUG
release: all

# highest target; sews together all objects into executable
all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

board.o: board.h board.cpp
	$(CXX) $(CXXFLAGS) -c board.cpp
  
# make clean - remove .o files, executable, tarball
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(SUBMIT_FILE)

test1:
	./main < test/test1.txt

test2:
	./main < test/test2.txt

test3:
	./main < test/test3.txt

test4:
	./main < test/test4.txt

test5:
	./main < test/test5.txt

test6:
	./main < test/test6.txt

test7:
	./main < test/test7.txt

# these targets do not create any files
.PHONY: all release debug profile clean submit
# disable built-in rules
.SUFFIXES:
