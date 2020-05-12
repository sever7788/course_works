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
#include <string.h>

#include "ClientSocket"
#include "ClientWindow"

int main()
{	int pixel_size = DISPLAY_WIDTH * DISPLAY_HEIGHT * 4;

	ClientWindow wnd;
	wnd.createWindow();
	ClientSocket s;

    char buf[10000];
    int j;
    while(1)
    {
    	for(int i = 0; i<pixel_size; i+=10000){
    		recvfrom(s.sock, buf, 10000, 0, NULL, NULL);
    		j = atoi(buf);
    		memcpy(&(wnd.image->data[j*10000]), buf, 10000);
    		if(j>360&&j<=364)
    			break;
    	}
    	// Отображаем картинку в окне
    	wnd.displayScreen();
    }
    close(s.sock);
    // Закрываем соединение с X сервером
    XCloseDisplay(wnd.display);
    return 0;
}
