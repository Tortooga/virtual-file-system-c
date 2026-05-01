app: core.o main.o 
	gcc obj/main.o obj/core.o -o bin/app

apptest: core.o tests_main.o testscore.o 
	gcc obj/core.o obj/testscore.o obj/tests_main.o -o bin/apptest


#Relocatable object files
testscore.o: file_storage_tests.o 
	gcc -r obj/file_storage_tests.o -o obj/testscore.o 

core.o: storage.o files.o file_storage.o file_logic.o
	gcc -r obj/storage.o obj/files.o obj/file_storage.o obj/file_logic.o -o obj/core.o


#Entry points
tests_main.o: tests/tests_main.c
	gcc -c tests/tests_main.c -o obj/tests_main.o 

main.o: src/main.c
	gcc -c src/main.c -o obj/main.o



#Test object files
file_storage_tests.o: tests/file_storage_tests.c 
	gcc -c tests/file_storage_tests.c -o obj/file_storage_tests.o 

#Core object files
storage.o: src/storage.c
	gcc -c src/storage.c -o obj/storage.o

files.o: src/files.c
	gcc -c src/files.c -o obj/files.o

file_storage.o: src/file_storage.c
	gcc -c src/file_storage.c -o obj/file_storage.o

file_logic.o: src/file_logic.c
	gcc -c src/file_logic.c -o obj/file_logic.o