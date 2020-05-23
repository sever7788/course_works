#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


#include "ClientSocket"
#include "ServerSocket"
#include "ServerDisplay"
#include "ServerMouse"
#include "ServerButtons"

char IP[30];
bool END = false;

void* thread_func1(void *arg){
	ServerMouse mouse = *(ServerMouse*) arg;
		mouse.setCursorPos();
	return 0;
}

void* thread_func2(void *arg){
	ServerButtons buttons = *(ServerButtons*) arg;
		buttons.receiveEvent();
	return 0;
}

int main(int argc, char* argv[])
{
	if(argc!=2){
		printf("Wrong number of parameters!\nExample: ./server [ip_addres]\n");
		return 0;
	}
	sprintf(IP,"%s",argv[1]);
	ServerDisplay disp;
	ServerMouse mouse(disp.display, &END);
	int result;
	pthread_t thread1, thread2;
	ServerSocket s(IP,3425);
	int pixel_size = DISPLAY_WIDTH * DISPLAY_HEIGHT * 4;
	int j = 0;

	result = pthread_create(&thread1, NULL, thread_func1, &mouse);
	if(result != 0){
		perror("Create first thread!");
		return EXIT_FAILURE;
	}

	ServerButtons buttons(&END);

	result = pthread_create(&thread2, NULL, thread_func2, &buttons);
	if(result != 0){
		perror("Create 2 thread!");
		return EXIT_FAILURE;
	}


    while(true){
    	disp.getDisplay();
    	for(int i = 0, j=0; i<pixel_size; i+=32784, j++){
    		sprintf(&(disp.image->data[i]), "%d", j);
    	send(s.sock, &(disp.image->data[i]), 32784, 0);
    	usleep(1000);
    	}
    	//usleep(10000);
    }
    result = pthread_join(thread1, NULL);
    if(result !=0){
    	perror("Expectation 1 thread.");
    	return EXIT_FAILURE;
    }

    result = pthread_join(thread2, NULL);
    if(result !=0){
    	perror("Expectation 2 thread.");
        return EXIT_FAILURE;
    }
    close(s.sock);
	// Закрываем соединение с X сервером
    XCloseDisplay(disp.display);

    return 0;
}
