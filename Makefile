CC = g++
DEPS = asctec.h llp_ctrl.h
BIN_DIR = bin
CFLAGS = `pkg-config --cflags lcm`
LDFLAGS = `pkg-config --libs lcm`

.PHONY: all clean Vicon lcmtypes
all: $(BIN_DIR)/llp_test lcmtypes Vicon $(BIN_DIR)/navigation lcmtypes/state_t.hpp $(BIN_DIR)/control_test 

# include/llp_ctrl.o: include/llp_ctrl.cpp include/llp_ctrl.h
# 	$(CC) -c include/llp_ctrl.cpp

# include/llp_port.o: include/llp_port.cpp include/llp_port.h
# 	$(CC) -c include/llp_port.cpp

lcmtypes:
	@$(MAKE) -C lcmtypes

Vicon: include/filter_util.o include/util.o
	@$(MAKE) -C Vicon

include/%.o: include/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

# include/filter_util.o: include/filter_util.c
# 	gcc -c -o $@ $^

main.o:
	$(CC) $(CFLAGS) -c main.cpp

navigation/%.o: navigation/%.cpp
	$(CC) -c -o $@ $^ $(LDFLAGS)

control/%.o: control/%.cpp
	$(CC) -pthread -std=c++0x -c -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/llp_test: main.o include/llp_ctrl.o include/llp_nav.o include/llp_port.o
	$(CC) -o $@ $^

$(BIN_DIR)/control_test: include/llp_ctrl.o include/llp_port.o control/control.o include/util.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/navigation: navigation/nav.o include/llp_nav.o include/llp_port.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *~ $(BIN_DIR)/* include/*.o navigation/*.o control/*.o
	@$(MAKE) -C lcmtypes clean