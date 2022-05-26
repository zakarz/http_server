#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main(int argc, char** argv) {

    int port = 7070;

    if (argc > 1) {
        port = atoi(argv[1]);
    }

    int serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0) {
        std::cerr << "Error while creating a socket" << std::endl;
        exit(errno);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int bound = bind(serverFd, (const struct sockaddr*)&address, sizeof(address));

    if (bound < 0) {
        std::cerr << "Could not bind to given port" << std::endl;
        exit(errno);
    }

    int listening = listen(serverFd, 1024);

    if (listening < 0) {
        std::cerr << "Could not start listening" << std::endl;
        exit(errno);
    }

    while (true) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLen = sizeof(clientAddress);
        int clientFd =accept4(serverFd, reinterpret_cast<sockaddr*>(&clientAddress),&clientAddressLen,SOCK_CLOEXEC);
        struct in_addr clientAddr = clientAddress.sin_addr;
        int clientPort = ntohs(clientAddress.sin_port);
        char* clientIp = inet_ntoa(clientAddr);
        std::cout << "Addr: " << clientAddr.s_addr << std::endl;
        std::cout << "Got connection from " << clientIp << ":" << clientPort << std::endl;

        int  number;
        char buf[20]={"Hello world"};
    
        ssize_t sentBytes = send(clientFd, (const void*)&buf[0], strlen(buf), 0);
        cout <<sentBytes <<"bytes were sent succesfully\n" ;
             
        if (sentBytes < 0) {
            std::cerr << "Could not write to client. Error: " << errno << std::endl;
            close(clientFd);
            continue;
        }
        
    
    }
    return 0;
}
