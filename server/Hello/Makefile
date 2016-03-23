CXX = g++
   cc = gcc
   
   LIB = -lpthread -ldl
   BIN = sqlite simple_server33
   
   all : $(BIN)
   sqlite : sqlite3.c shell.c
       $(cc) -o $@ $^ $(LIB) 
  apiTest : simple_server33.cpp sqlite3.o
      $(CXX) -o $@ $^ $(LIB) 
  sqlite3.o : sqlite3.c
      $(cc) -o $@ -c $^
  
  clean :
      rm -f $(BIN)
  
  .PHONY: all, clean