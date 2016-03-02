#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>

using namespace std;

int main() {

	int x = open("/sys/class/gpio/gpio21/value", O_RDONLY);
	if (x == -1) {
		cerr << "no good" << endl;
		exit(1);	
	}

	int y = open("/sys/class/gpio/gpio20/value", O_RDONLY);
	if (y == -1) {
		cerr << "no good" << endl;
		exit(1);	
	}

	struct pollfd pfds[2];
	pfds[0].fd = x;
	pfds[0].events = POLLPRI | POLLERR;
	pfds[1].fd = y;
	pfds[1].events = POLLPRI | POLLERR;

	char val;
	read(x, &val, 1);
	read(y, &val, 1);

	string bits_rec;

	while(1) {
		int ret = poll(pfds, 2, 100);
		if (ret == -1) {
			cerr << "no good" << endl;
			exit(1);
		} else if (ret == 0) {
			if (bits_rec.length() != 0) {
				cout << bits_rec << endl;
				bits_rec = "";
			}
		}
		
		for (int i = 0; i < 2; i++) {
			if (pfds[i].revents & POLLPRI) {
				lseek(pfds[i].fd, 0, SEEK_SET);
				read(pfds[i].fd, &val, 1);
				bits_rec += (i == 0) ? "0" : "1";
				//cout << "falling: " << i << endl;
			}
		}
	}
}

