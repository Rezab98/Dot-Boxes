#include <stdio.h> 
#include <stdbool.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080
#define max_clients 30
#define SA struct sockaddr
struct sockaddr_in servaddr, cli;  
int client_socket[30];
int connfdsarray[30];
int twoplayer[30];
int threeplayer[30];
int fourplayer[30];
int twoplayercount = 0;
int threeplayercount = 0;
int fourplayercount = 0;
int numberofgames = 1;
// Function designed for chat between client and server. 
void func(int sockfd, fd_set *connfds) 
{ 
    char buff[MAX]; 
    int n; 
    bzero(buff, MAX); 
     
    int max_sd = sockfd;
    int activity;
    int new_socket;
    struct sockaddr_in address;
    int sd;
    char *message ="Hi!\n";
    int cli = sizeof(address);
    for (;;) { 
        //printf("YAY2\n");
        for (int i = 0 ; i < max_clients ; i++)   
        {   
            sd = client_socket[i];   
            bool added = false;
            for(int j = 0; j < max_clients; j ++)
                if(sd == connfdsarray[j]){
                    added = true;
                    break;
                }

            if(sd > 0 && !added)
            {
                //printf("first if\n");
                FD_SET( sd , connfds); 
                connfdsarray[i] = sd;
            }
            if(sd > max_sd)
            {
                //printf("second if\n");

                max_sd = sd; 
            }  
        }
        //printf("YAY2.1\n");
        activity = select( max_sd + 1 , connfds , NULL , NULL , NULL);
        //printf("YAY3\n");
        
        
        if ((activity < 0))   
        {   
            printf("select error");   
        }   
        
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(sockfd, connfds))   
        {   
            /*
            if ((new_socket = accept(sockfd,  
                    (SA*)&address, &cli)) < 0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
            */
            
            new_socket = accept(sockfd,  
                    (SA*)&address, &cli);
            //inform user of socket number - used in send and receive commands  
            //printf("YAY3\n");
            /*
            printf("New connection , ip is : %s , port : %d\n" , inet_ntoa(address.sin_addr), 
                  (address.sin_port)); 
                */  
            //printf("YAY4\n");
            //send new connection greeting message  
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            {   
                perror("send");   
            }   
                 
            puts("Welcome message sent successfully");   
                 
            //add new socket to array of sockets  
            for (int i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }
        else
        {
            for (int i = 0; i < max_clients; i++)   
            {   
                sd = client_socket[i];   
                 
                if (FD_ISSET( sd , connfds))
                {
                    read(sd, buff, sizeof(buff)); 
                }

                if(buff == "2")
                {
                    twoplayer[twoplayercount] = sd;
                    twoplayercount++;
                    if(twoplayercount == 2)
                    {
                        twoplayercount = 0;
                        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                        if (sockfd == -1) { 
                            printf("socket creation failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Socket successfully created..\n"); 
                        bzero(&servaddr, sizeof(servaddr)); 
                    
                        // assign IP, PORT 
                        servaddr.sin_family = AF_INET; 
                        servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
                        servaddr.sin_port = htons(PORT + numberofgames);
                        numberofgames++;
                    
                        // Binding newly created socket to given IP and verification 
                        if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
                            printf("socket bind failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Socket successfully binded..\n"); 
                    
                        // Now server is ready to listen and verification 
                        if ((listen(sockfd, 5)) != 0) { 
                            printf("Listen failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Server listening..\n"); 
                    }

                }
                if(buff == "3")
                {
                    threeplayer[threeplayercount] = sd;
                    threeplayercount++;
                    if(threeplayercount == 3){
                        threeplayercount = 0;
                        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                        if (sockfd == -1) { 
                            printf("socket creation failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Socket successfully created..\n"); 
                        bzero(&servaddr, sizeof(servaddr)); 
                    
                        // assign IP, PORT 
                        servaddr.sin_family = AF_INET; 
                        servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
                        servaddr.sin_port = htons(PORT + numberofgames);
                        numberofgames++;
                    
                        // Binding newly created socket to given IP and verification 
                        if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
                            printf("socket bind failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Socket successfully binded..\n"); 
                    
                        // Now server is ready to listen and verification 
                        if ((listen(sockfd, 5)) != 0) { 
                            printf("Listen failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Server listening..\n"); 
                    
                    }
                }
                if(buff == "4")
                {
                    fourplayer[fourplayercount] = sd;
                    fourplayercount++;
                    if(fourplayercount == 4){
                        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                        if (sockfd == -1) { 
                            printf("socket creation failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Socket successfully created..\n"); 
                        bzero(&servaddr, sizeof(servaddr)); 
                    
                        // assign IP, PORT 
                        servaddr.sin_family = AF_INET; 
                        servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
                        servaddr.sin_port = htons(PORT + numberofgames);
                        numberofgames++;
                    
                        // Binding newly created socket to given IP and verification 
                        if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
                            printf("socket bind failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Socket successfully binded..\n"); 
                    
                        // Now server is ready to listen and verification 
                        if ((listen(sockfd, 5)) != 0) { 
                            printf("Listen failed...\n"); 
                            exit(0); 
                        } 
                        else
                            printf("Server listening..\n"); 
                    
                    }
                }
              
            }
        
        
        // print buffer which contains the client contents 
            printf("From client: %s\t To client : ", buff); 
        }
        /*
        bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
  
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        }
        */
    } 
} 
  
// Driver function 
int main() 
{ 
    int sockfd, connfd, len;
    fd_set connfds;
    
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
  
    // Accept the data packet from client and verification
    /*
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
    */
    // Function for chatting between client and server 
    
    FD_ZERO(&connfds);   
    FD_SET(sockfd, &connfds);  
    func(sockfd, &connfds); 
  
    // After chatting close the socket 
    close(sockfd); 
} 