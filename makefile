ifeq ($(OS),Windows_NT)
   CPP := C:\Program Files\mingw-w64\x86_64-7.3.0-posix-seh-rt_v5-rev0\mingw64\bin\c++
else
   CPP := c++
endif

CPPX := $(CPP) -std=c++17 -fconcepts -Ilibrary

.PHONY: run fail tests build docs 

test-compilation.exe: library/torsor.hpp tests/test-compilation.cpp
	$(CPPX) tests/test-compilation.cpp -o test-compilation.exe 

test-compilation-concepts.exe: library/torsor.hpp tests/test-compilation-concepts.cpp
	$(CPPX) tests/test-compilation-concepts.cpp -o test-compilation-concepts.exe 

test-runtime.exe: test/test-runtime.cpp library/quantity.hpp library/type_multiset.hpp
	$(CPPX) test/test-runtime.cpp -o test-runtime.exe 

build: 
	$(CPPX) test/test-error-messages.cpp -o test-compiler-messages.exe 

run: test-runtime.exe
	./test-runtime.exe

fail: test-compilation.exe test-compilation-concepts.exe
	./test-compilation.exe 
	./test-compilation-concepts.exe 
   
tests: run fail

docs: 
	Doxygen documentation/Doxyfile
	pandoc -V geometry:a4paper -s -o documentation/readme.pdf readme.md
