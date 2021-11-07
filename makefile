all:
	mkdir -p bin
	$(CC) -I. engine/*.cpp ui/*.cpp -lc -lm -lstdc++ -ldl -lpthread ./ext/libraylib_linux.a -o comfyengine

