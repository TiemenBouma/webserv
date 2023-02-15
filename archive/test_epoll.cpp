#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int fd) {
	int epfd;
	struct epoll_event ev, events[1];

	// Create an epoll instance
	epfd = epoll_create(1); 
	if (epfd == -1) {
		perror("epoll_create");
		return 1;
	}

	// Wait for incoming data
	ev.events = EPOLLIN; 
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl");
		return 1;
	}

	// Check for events with a 0 timeout
	int nfds = epoll_wait(epfd, events, 1, 0); 
	if (nfds == -1) {
		if (errno != EINTR) {
		// Interrupted by a signal, continue the loop
			perror("epoll_wait");
			return 1;
		}
	}

	if (nfds == 0) {
		// No events, do other work here
		return 0;
	}

	if (events[0].data.fd == fd) {
		// Incoming data on fd
		char buf[1024];
		ssize_t n = read(fd, buf, sizeof(buf));
		if (n == -1) {
			perror("read");
			return 1;
		} else if (n == 0) {
			printf("EOF on fd\n");
			return 0;
		} else {
			printf("Received %ld bytes on fd\n", n);
		}
	}
  return 0;
}
