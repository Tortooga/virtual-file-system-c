app: core.o main.o 
	gcc obj/main.o obj/core.o -o bin/app

apptest: core.o tests_main.o testscore.o 
	gcc obj/core.o obj/testscore.o obj/tests_main.o -o bin/apptest


#Relocatable object files
testscore.o: file_storage_tests.o file_logic_tests.o queries_tests.o
	gcc -r obj/file_storage_tests.o obj/file_logic_tests.o obj/queries_tests.o -o obj/testscore.o 

core.o: storage.o files.o file_storage.o file_logic.o folders.o path_parser.o queries.o
	gcc -r obj/storage.o obj/files.o obj/file_storage.o obj/file_logic.o obj/folders.o obj/path_parser.o obj/queries.o -o obj/core.o


#Entry points
tests_main.o: tests/tests_main.c
	gcc -c tests/tests_main.c -o obj/tests_main.o 

main.o: src/main.c
	gcc -c src/main.c -o obj/main.o



#Test object files
queries_tests.o: tests/queries_tests.c 
	gcc -c tests/queries_tests.c -o obj/queries_tests.o

file_storage_tests.o: tests/file_storage_tests.c 
	gcc -c tests/file_storage_tests.c -o obj/file_storage_tests.o 

file_logic_tests.o: tests/file_logic_tests.c
	gcc -c tests/file_logic_tests.c -o obj/file_logic_tests.o 

#Core object files
	
storage.o: src/storage.c
	gcc -c src/storage.c -o obj/storage.o

files.o: src/files.c
	gcc -c src/files.c -o obj/files.o

file_storage.o: src/file_storage.c
	gcc -c src/file_storage.c -o obj/file_storage.o

file_logic.o: src/file_logic.c
	gcc -c src/file_logic.c -o obj/file_logic.o

folders.o: src/folders.c
	gcc -c src/folders.c -o obj/folders.o 

path_parser.o : src/path_parser.c
	gcc -c src/path_parser.c -o obj/path_parser.o

queries.o: src/queries.c
	gcc -c src/queries.c -o obj/queries.o