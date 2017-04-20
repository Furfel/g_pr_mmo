#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/time.h>
#include <string.h>

#define PORT 9528
#define WELCOME_BYTE 0x7
#define WELCOME_NOSPACE 0x15
#define COMMAND 0x11
#define NOP 0x1
#define SET_NAME 0x2
#define MOVE 0x3

#define LOG_LINES 3

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

unsigned long Tick() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

char* logLines[LOG_LINES];

void PushLog(const char* log, ...) {
	printf("Fin");
	if(logLines[LOG_LINES-1]!=NULL) free(logLines[LOG_LINES-1]);
	return;
	unsigned char log_lines;
	for(log_lines=LOG_LINES-1;log_lines>0;--log_lines)
		logLines[log_lines]=logLines[log_lines-1];
	size_t len;
	va_list arg;
	va_start(arg,log);
	len = vsnprintf(NULL,0,log,arg);
	logLines[0]=(char*)calloc(1,len+1);
	vsprintf(logLines[0],log,arg);
	va_end(arg);
}

void redraw(char* bufor) {
	int y,x,z,index;
	char count,c;
	for(y=0;y<=6;++y) {
		for(x=0;x<=6;++x) {
			count = bufor[index]; ++index;
				for(z=0;z<count;z++) {
					c = bufor[index]; ++index;
				}
			printf("%d",(int)(c%100));
			}
		move(y,x*3);
	}
	int i;
	for(i=0;i<LOG_LINES;++i) {
		move(8+i,2);
		if(logLines[i]!=NULL)
			printf("%s",logLines[i]);
	}
}

int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void nonblock(int state)
{
    struct termios ttystate;
 
    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);
 
    if (state==1)
    {
        //turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
    }
    else if (state==2)
    {
        //turn on canonical mode
        ttystate.c_lflag |= ICANON;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
 
}

void nltozero(char* src, size_t size) {
    for(int i=0;i<size;i++) {
        if(src[i]=='\n' || src[i]=='\r')
            src[i]='\0';
    }
}

int main(int argc, char* argv[]) {
	int i;
	for(i=0;i<LOG_LINES;++i) logLines[i]=NULL;
	
    char* adres;
    char name[40];
    bzero(name,40);
    if(argc > 1) {
        adres = argv[1];
    } else {
        printf("Polacz z serwerem: ");
        adres = malloc(24);
        bzero(adres,24);
        fgets(adres,23,stdin);
        nltozero(adres,23);
    }

    if(argc > 2) {
        memcpy(name,argv[2],MIN(strlen(argv[2]),39));
        printf("Ustawiono imie %s\n",name);
    } else {
        printf("Podaj imie gracza: ");
        fgets(name,39,stdin);
        nltozero(name,39);
    }
    
    int socketf;
    socketf = socket(AF_INET,SOCK_STREAM,0); //IPv4, TCP
    if(socketf<0) {
        printf("Can\'t start a socket!\n");
        return 1;
    }
    
    struct hostent* serwer; //Adres hosta
    serwer = gethostbyname(adres);

    if(serwer == NULL) {
        printf("Can\'t locate host %s!\n",adres);
        return 1;
    }

    struct sockaddr_in serwer_socket;
    bzero((char*) &serwer_socket,sizeof(serwer_socket));
    serwer_socket.sin_family = AF_INET; //IPv4
    bcopy((char*) (serwer->h_addr),(char*)&serwer_socket.sin_addr.s_addr,serwer->h_length); //Wklej adres do struktury
    serwer_socket.sin_port = htons(PORT); //Wklej port w odpowiednim BOM do struktury
    
    char bufor[1024]; //Max 1KB bufora
    int n;

    bzero(bufor,1024);

    if(connect(socketf,(struct sockaddr *)&serwer_socket,sizeof(serwer_socket)) < 0) {
        printf("Can\'t connect!\n");
        return 1;
    }

    n=read(socketf,bufor,1023);
    if(n<0) {
        printf("Can\'t read handshake from server!\n");
        close(socketf);
        return 1;
    }

    if(bufor[0]==WELCOME_BYTE) {
        bufor[0]=' ';
        PushLog("(Server):%s\n",bufor+1);
    } else if(bufor[0]==WELCOME_NOSPACE){
        bufor[0]=' ';
        PushLog("(Server full):%s\n",bufor+1);
        close(socketf);
        return 0;
    } else {
        PushLog("Bad handshake (is it the server?)\n");
        close(socketf);
        return 1;
    }

    bufor[0]=COMMAND;
    bufor[1]=SET_NAME;
    memcpy(bufor+2,name,40);
    
    printf("<<<Setting name\n");
	n = write(socketf,bufor,strlen(bufor));
    if(n<0) {
        printf("Can\'t send to server!\n");
        close(socketf);
        return 1;
    }
   
    
    unsigned long tm; tm = Tick();
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr,TRUE);
    tm = Tick() - tm;
    printf("...Sleeping %lums\n",(1000*50-tm)/1000);
    usleep(1000*50-tm);
    
    char ch; ch=0x1;
    while(ch != 'q') {
		ch = getch();
		n = read(socketf,bufor,1023);
		if(n<0)
			PushLog("Cannot read!\n");
		else {
			PushLog(">>>Read\n");
			redraw(bufor);
		}
		refresh();
		usleep(1000*50);
	}
    
    endwin();
    return 0;
    
    short x,y,index;
    unsigned char count,z,c;
    
    for(i=0;i<10;i++) {
    
		printf(">>>Reading map\n");
		n = read(socketf,bufor,1023);
		if(n<0) {
			printf("Cannot read!\n");
		} else {
            printf(">>>Read %d\n",n);
			index=3;
			for(y=0;y<=6;++y) {
				for(x=0;x<=6;++x) {
					count = bufor[index]; ++index;
					for(z=0;z<count;z++) {
						c = bufor[index]; ++index;
					}
					printf("%d%d:",c%10,count%10);
				}
				printf("\n");
			}
		}
		
		c=(i%2==0?(i%4==0?'w':'s'):(i%3==0?'a':'d'));
		printf("<<<Moving %c\n",c);
		bufor[0]=COMMAND;
		bufor[1]=MOVE;
		bufor[2]=c;
		bufor[3]=0x0;
		write(socketf,bufor,strlen(bufor));
	
		printf("...Sleeping 100ms\n");
		usleep(1000*100);
		
	}
 
    nonblock(1);
    while(!i)
    {
        usleep(1000*100);
        i=kbhit();
        if (i!=0)
        {
            c=fgetc(stdin);
            if (c=='q')
                i=1;
            else if(c=='w' || c=='s' || c=='a' || c=='d') {
                bufor[0]=COMMAND; bufor[1]=MOVE; bufor[3]=0;
                bufor[2]=c;
                write(socketf,bufor,strlen(bufor));
                i=0;
            } else {
                bufor[0]=COMMAND; bufor[1]=NOP; bufor[2]=0;
                write(socketf,bufor,strlen(bufor));
                i=0;
            }
        } else {
           bufor[0]=COMMAND; bufor[1]=NOP; bufor[2]=0;
           write(socketf,bufor,strlen(bufor));
           i=0;
        }

        fprintf(stderr,"%d ",i);
    }
    printf("\n you hit %c. \n",c);
    nonblock(2);

    return 0;
}
