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
#include <arpa/inet.h>
#include <pthread.h>

#include "ServerSocket"
#include "ClientSocket"
#include "ClientWindow"
#include "ClientMouse"
#include "ClientButtons"

char IP[30];
bool END = false;
void * thread_func1(void *arg){

    ClientMouse mouse = * (ClientMouse *) arg;
    	mouse.getCursorPos();
    return 0;
}

void * thread_func2(void *arg){

	ClientButtons buttons = * (ClientButtons *) arg;
    	buttons.sendEvent();
    return 0;
}

int main(int argc, char* argv[])
{
	if(argc!=2){
		printf("Wrong number of parameters!\nExample: ./server [ip_addres]\n");
		return 0;
	}
	sprintf(IP, "%s", argv[1]);
	ClientWindow wnd;
	wnd.createWindow();
	wnd.displayScreen();
	ClientSocket s(3425);

	char buf[32784];
	int j;
	int pixel_size = DISPLAY_WIDTH * DISPLAY_HEIGHT * 4;

	ClientMouse mouse(IP, wnd.window,&END);
	int result;
	pthread_t thread1;
    result = pthread_create(&thread1, NULL, thread_func1, &mouse);
	if (result != 0) {
		perror("Создание первого потока!");
	    return EXIT_FAILURE;
    }

	ClientButtons buttons(IP, wnd.window,&END);
    pthread_t thread2;
    result = pthread_create(&thread2, NULL, thread_func2, &buttons);
    if (result != 0) {
        perror("Создание второго потока!");
        return EXIT_FAILURE;
    }
    while(!END)
    {
    	for(int i = 0; i<pixel_size; i+=32784){
    		recvfrom(s.sock, buf, 32784, 0, NULL, NULL);
    		j = atoi(buf);
    		memcpy(&(wnd.image->data[j*32784]), buf, 32784);
    		if(j==128)
    			break;
    	}
    // Отображаем картинку в окне
    wnd.displayScreen();
    }

    result = pthread_join(thread1, NULL);
    if (result != 0) {
    	perror("Ждём первый поток");
    	return EXIT_FAILURE;
    }

    result = pthread_join(thread2, NULL);
        if (result != 0) {
        	perror("Ждём второй поток");
        	return EXIT_FAILURE;
        }
    close(s.sock);
    // Закрываем соединение с X сервером
    XCloseDisplay(wnd.display);
    return 0;
}
