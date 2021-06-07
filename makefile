CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER 
# Data Structures

Cache_write.o: data_structures/Cache_write.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/Cache_write.cpp -o build/Cache_write.o

CPU_Request.o: data_structures/CPU_Request.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/CPU_Request.cpp -o build/CPU_Request.o

Memory_write.o: data_structures/Memory_write.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/Memory_write.cpp -o build/Memory_write.o

Parsed_Request.o: data_structures/Parsed_Request.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/Parsed_Request.cpp -o build/Parsed_Request.o

Tag_change.o: data_structures/Tag_change.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/Tag_change.cpp -o build/Tag_change.o

# Tests
RequestManager_test.o: test/RequestManager_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/RequestManager_test.cpp -o build/RequestManager_test.o

MUX_test.o: test/MUX_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/MUX_test.cpp -o build/MUX_test.o

hit_miss_test.o: test/hit_miss_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/hit_miss_test.cpp -o build/hit_miss_test.o

Tag_test.o: test/Tag_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Tag_test.cpp -o build/Tag_test.o

Block_test.o: test/Block_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Block_test.cpp -o build/Block_test.o

Hit_read_test.o: test/Hit_read_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Hit_read_test.cpp -o build/Hit_read_test.o

Hit_write_test.o: test/Hit_write_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Hit_write_test.cpp -o build/Hit_write_test.o

Miss_write_test.o: test/Miss_write_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Miss_write_test.cpp -o build/Miss_write_test.o

Miss_read_test.o: test/Miss_read_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Miss_read_test.cpp -o build/Miss_read_test.o

Memory_request_manager_test.o: test/Memory_request_manager_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Memory_request_manager_test.cpp -o build/Memory_request_manager_test.o

Top_model.o: top_model/Top_model.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/Top_model.cpp -o build/Top_model.o

tests: Top_model.o RequestManager_test.o MUX_test.o hit_miss_test.o Tag_test.o Block_test.o Hit_read_test.o Hit_write_test.o Miss_write_test.o Miss_read_test.o Memory_request_manager_test.o CPU_Request.o Parsed_Request.o Cache_write.o Memory_write.o Tag_change.o
	$(CC) -g -o bin/REQUESTMANAGER_TEST build/RequestManager_test.o build/CPU_Request.o build/Parsed_Request.o
	$(CC) -g -o bin/MUX_TEST build/MUX_test.o build/CPU_Request.o build/Parsed_Request.o
	$(CC) -g -o bin/HIT_MISS_TEST build/hit_miss_test.o build/Parsed_Request.o build/Tag_change.o
	$(CC) -g -o bin/TAG_TEST build/Tag_test.o build/Tag_change.o
	$(CC) -g -o bin/BLOCK_TEST build/Block_test.o build/Tag_change.o build/Cache_write.o
	$(CC) -g -o bin/HIT_READ_TEST build/Hit_read_test.o build/Parsed_Request.o
	$(CC) -g -o bin/HIT_WRITE_TEST build/Hit_write_test.o build/Parsed_Request.o build/Cache_write.o
	$(CC) -g -o bin/MISS_WRITE_TEST build/Miss_write_test.o build/Parsed_Request.o build/Cache_write.o
	$(CC) -g -o bin/MISS_READ_TEST build/Miss_read_test.o build/Parsed_Request.o build/Cache_write.o
	$(CC) -g -o bin/MEM_REQUEST_MANAGER_TEST build/Memory_request_manager_test.o build/Cache_write.o

#Compile top model sim only
sim: Top_model.o CPU_Request.o Parsed_Request.o Cache_write.o Tag_change.o
	$(CC) -g -o bin/TOP_TEST build/Top_model.o build/CPU_Request.o build/Parsed_Request.o build/Cache_write.o build/Tag_change.o
#TARGET TO COMPILE EVERYTHING
all: tests

#CLEAN COMMANDS
clean:
	rm -f bin/* build/*




