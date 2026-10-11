BUILD_DIR ?= build

.PHONY: configure build build-clean test fresh clean

configure: $(BUILD_DIR)/CMakeCache.txt

$(BUILD_DIR)/CMakeCache.txt:
	cmake -S . -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR) 

build-clean: configure
	cmake --build $(BUILD_DIR) --target clean

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

clean:
	rm -rf $(BUILD_DIR)

fresh:
	$(MAKE) clean
	$(MAKE) build

