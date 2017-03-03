#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 9528
#define WELCOME_BYTE 0x7
#define WELCOME_NOSPACE 0x15
#define COMMAND 0x11
#define SET_NAME 0x2

void nltozero(char* src, size_t size) {
    for(int i=0;i<size;i++) {
        if(src[i]=='\n' || src[i]=='\r')
            src[i]='\0';
    }
}

int main(int argc, char* argv[]) {
    char* adres;
    if(argc > 1) {
        adres = argv[1];
    } else {
        printf("Polacz z serwerem: ");
        adres = malloc(24);
        bzero(adres,24);
        fgets(adres,23,stdin);
        nltozero(adres,23);
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
    
    char name[40];
    char bufor[1024]; //Max 1KB bufora
    int n;

    bzero(name,40);
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
        printf("(Server):%s\n",bufor);
    } else if(bufor[0]==WELCOME_NOSPACE){
        bufor[0]=' ';
        printf("(Server full):%s\n",bufor);
        close(socketf);
        return 0;
    } else {
        printf("Bad handshake (is it the server?)\n");
        close(socketf);
        return 1;
    }

    printf("Podaj imie gracza: ");
    fgets(name,39,stdin);
    nltozero(name,39);
    bufor[0]=COMMAND;
    bufor[1]=SET_NAME;
    memcpy(bufor+2,name,40);
    
    n = write(socketf,bufor,strlen(bufor));
    if(n<0) {
        printf("Can\'t send to server!\n");
        close(socketf);
        return 1;
    }

    return 0;
}
