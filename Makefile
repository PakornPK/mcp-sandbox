.PHONY: clean build run

clean:
	rm -rf build && mkdir build && cd build && cmake ..

build:
	cd build && cmake --build .

run:
	./bin/mcp_sandbox
