rebuild:
	rm -rf ./build/assets
	cmake -B build
	make -C build
	make run

run:
	./build/featherGL

clean:
	rm -rf ./build

rebuild0:
	make clean
	make build
