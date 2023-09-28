build: source/source.cpp source/glad.c source/std_image.c
	g++ -Wall -o build/source $^ -I include -lglfw
	./build/source

clean:
	rm -f build/source

