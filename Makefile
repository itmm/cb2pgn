HXs := $(shell hx-srcs.sh)
SRCs := $(shell hx-files.sh $(HXs))
APPs := $(SRCs:.cpp=)
CXXFLAGS += -Wall -std=c++17 -g

.PHONY: all clean

all: .hx-run

.hx-run: $(HXs)
	@echo "HX"
	@hx
	@date >$@
	@make --no-print-directory $(APPs)

$(APPs): $(SRCs)
	@echo "C++ $@"
	@$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@echo "RM"
	@rm -f $(SRCs) $(APPs) .hx-run

