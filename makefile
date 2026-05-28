run: app
	@echo "=============================================="
	@echo "||               Running App                ||"
	@echo "=============================================="
	@./bin/app


#Executables 
app: systemReloc.o workspaceReloc.o CLIReloc.o objDir binDir main.o 
	gcc obj/systemReloc.o obj/workspaceReloc.o obj/CLIReloc.o obj/main.o -o bin/app

apptest: systemReloc.o tests_main.o objDir binDir testscore.o 
	gcc obj/system.o obj/testscore.o obj/tests_main.o -o bin/apptest

#Important Directories
binDir: 
	@mkdir -p bin 

objDir: 
	@mkdir -p obj 

#Relocatable object files
#We hide linking 
testscore.o: file_storage_tests.o file_logic_tests.o queries_tests.o
	@gcc -r obj/file_storage_tests.o obj/file_logic_tests.o obj/queries_tests.o -o obj/testscore.o 

systemReloc.o: storage.o files.o file_storage.o file_logic.o folders.o vfs_entry_store.o path_utils.o queries.o entry_relocation.o
	@gcc -r obj/storage.o obj/files.o obj/file_storage.o obj/file_logic.o obj/folders.o obj/vfs_entry_store.o obj/path_utils.o obj/queries.o obj/entry_relocation.o -o obj/systemReloc.o

workspaceReloc.o: workspace.o navigation.o vfs_ops.o file_io.o 
	@gcc -r obj/workspace.o obj/navigation.o obj/vfs_ops.o obj/file_io.o -o obj/workspaceReloc.o 

CLIReloc.o: commands.o input_handler.o
	@gcc -r obj/commands.o obj/input_handler.o -o obj/CLIReloc.o

#Entry points
tests_main.o: tests/tests_main.c
	gcc -c tests/tests_main.c -o obj/tests_main.o 

main.o: main.c
	gcc -c main.c -I system/include -I . -I workspace -I cli -o obj/main.o

#CLI object files
commands.o: cli/commands.c
	gcc -c cli/commands.c -I system/include -I . -o obj/commands.o

input_handler.o: cli/input_handler.o 
	gcc -c cli/input_handler.c -I system/include -I . -o obj/input_handler.o

#Workspace object files
workspace.o: workspace/workspace.c
	gcc -c workspace/workspace.c -I system/include -I . -o obj/workspace.o 

navigation.o: workspace/navigation.c
	gcc -c workspace/navigation.c -I system/include -I . -o obj/navigation.o 

vfs_ops.o: workspace/vfs_ops.c
	gcc -c workspace/vfs_ops.c -I system/include -I . -o obj/vfs_ops.o

file_io.o: workspace/file_io.c 
	gcc -c workspace/file_io.c -I system/include -I . -o obj/file_io.o 

#Test object files
queries_tests.o: tests/queries_tests.c 
	gcc -c tests/queries_tests.c -o obj/queries_tests.o

file_storage_tests.o: tests/file_storage_tests.c 
	gcc -c tests/file_storage_tests.c -o obj/file_storage_tests.o 

file_logic_tests.o: tests/file_logic_tests.c
	gcc -c tests/file_logic_tests.c -o obj/file_logic_tests.o 

#Core object files
	
storage.o: system/storage.c
	gcc -c system/storage.c -I system/include -I . -o obj/storage.o

files.o: system/files.c
	gcc -c system/files.c -I system/include -I . -o obj/files.o

file_storage.o: system/file_storage.c
	gcc -c system/file_storage.c -I system/include -I . -o obj/file_storage.o

file_logic.o: system/file_logic.c
	gcc -c system/file_logic.c -I system/include -I . -o obj/file_logic.o

folders.o: system/folders.c
	gcc -c system/folders.c -I system/include -I . -o obj/folders.o 

vfs_entry_store.o: system/vfs_entry_store.c
	gcc -c system/vfs_entry_store.c -I system/include -I . -o obj/vfs_entry_store.o 

path_utils.o : system/path_utils.c
	gcc -c system/path_utils.c -I system/include -I . -o obj/path_utils.o

queries.o: system/queries.c
	gcc -c system/queries.c -I system/include -I . -o obj/queries.o

entry_relocation.o: system/entry_relocation.c
	gcc -c system/entry_relocation.c -I system/include -I . -o obj/entry_relocation.o