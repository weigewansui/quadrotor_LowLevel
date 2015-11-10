#include "llp_port.h"

class LLP_CTRL : public LLP_PORT {

public:
	using LLP_PORT::LLP_PORT;
	LLP_CTRL();
	LLP_CTRL(termios, string, int, int);
	void send_command(CTRL_INPUT*);
	void switch_motor();
};