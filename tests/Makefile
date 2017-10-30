all: clean tests


install:
	g++ -isystem googletest/include -Igoogletest -pthread -c googletest/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

tests:
	g++ -isystem googletest/include -pthread test_golosa.cpp googletest/src/gtest_main.cc libgtest.a -o tests
	# aca habria que leer todos los archivos del directorio


clean:
	rm -f *.o
	rm -f tests
	rm -f *.gch
