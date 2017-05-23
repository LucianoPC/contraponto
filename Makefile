CC = g++
prefix = /usr/local


all: generate_binary


generate_binary: compile_mum
	mkdir -p bin/
	$(CC) -o bin/contraponto src/main.cpp obj/*.o -I lib/MuM


compile_mum:
	mkdir -p obj/
	$(CC) -c lib/MuM/MuUtil.cpp -o obj/MuUtil.o
	$(CC) -c lib/MuM/MuError.cpp -o obj/MuError.o
	$(CC) -c lib/MuM/MuParamBlock.cpp -o obj/MuParamBlock.o
	$(CC) -c lib/MuM/MuNote.cpp -o obj/MuNote.o
	$(CC) -c lib/MuM/MuVoice.cpp -o obj/MuVoice.o
	$(CC) -c lib/MuM/MuMaterial.cpp -o obj/MuMaterial.o


clean:
	-rm -rf bin/
	-rm -rf obj/
	-rm -rf *.sco
	-rm -rf *.orc
