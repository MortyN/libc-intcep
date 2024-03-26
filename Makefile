compile: example_compile
	gcc -Wall -fPIC -shared -o intercept.so intercept.c -ldl

example_compile:
	gcc exampleprogram/main.c -o exampleprogram/main.o

macos_compile: intercept_macos_compile example_compile

macos_run: macos_compile
	DYLD_INSERT_LIBRARIES=intercept.dylib exampleprogram/main.o

intercept_macos_compile: 
	gcc -dynamiclib intercept.c -o intercept.dylib

run_java:
	docker build -f java/Dockerfile . --tag interceptjava:latest
	docker run interceptjava

run_c:
	docker build -f c/Dockerfile . --tag interceptc:latest
	docker run interceptc

docker_prune:
	docker system prune -a

docker_copy_files:
	docker cp 56829c3950ea:/app/filenamejava.txt file.txt

intercept_compile:
	gcc -Wall -fPIC -shared -o intercept.so intercept.c -ldl