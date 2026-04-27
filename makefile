app: main.o storage.o files.o file_storage.o
	gcc obj/storage.o obj/files.o obj/file_storage.o obj/main.o -o bin/app


main.o: src/main.c
	gcc -c src/main.c -o obj/main.o

storage.o: src/storage.c
	gcc -c src/storage.c -o obj/storage.o

files.o: src/files.c
	gcc -c src/files.c -o obj/files.o

file_storage.o: src/file_storage.c
	gcc -c src/file_storage.c -o obj/file_storage.o
