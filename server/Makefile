CXX = g++
cc = gcc
HDIRS = .

all: simple_server33

simple_server33: simple_server33.cpp $(HDIRS)/prereq.h $(HDIRS)/sqlsupp.h sqlite.o
	$(CXX) -o simple_server33 simple_server33.cpp sqlite.o -lpthread -ldl -I$(HDIRS)

sqlite.o: sqlite3.c $(HDIRS)/sqlite3.h $(HDIRS)/sqlite3ext.h
	$(cc) -c -o sqlite.o sqlite3.c -lpthread 
