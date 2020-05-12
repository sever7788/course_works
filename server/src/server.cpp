#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ServerSocket"
#include "ServerDisplay"

int main()
{
	ServerDisplay disp;
	ServerSocket s;
	int pixel_size = DISPLAY_WIDTH * DISPLAY_HEIGHT * 4;
    char str[11];
    int j = 0;
    while(true){
    	disp.getDisplay();
    	//send(sock, (char*)"12345", 5, 0);
    	for(int i = 0, j=0; i<pixel_size; i+=10000, j++){
    		sprintf(&(disp.image->data[i]), "%d", j);
    	send(s.sock, &(disp.image->data[i]), 10000, 0);
    	usleep(1000);
    	}
    	//usleep(500);
    }
    close(s.sock);

	// Закрываем соединение с X сервером
    XCloseDisplay(disp.display);

    return 0;
}
