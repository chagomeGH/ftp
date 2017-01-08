//================================================================
//  FileName    : Socket.cpp
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
//================================================================
#include "Socket.h"
#include <unistd.h>         // read, write, close
#include <fcntl.h>          // file control options
#include <sys/socket.h>     // socket, bind, listen, inet_ntoa
#include <iostream>
#include <stdlib.h>
#include <cstring>

const int Socket::BUF_SIZE(8192);


/*  Constructor
    Creates an open connection to open resources 
    @param ip_address, network location of remote resource to connect to
    @param port, client port input
*/
Socket::Socket(const char *ip_address, const int port):

sockDescriptor(socket(AF_INET, SOCK_STREAM, 0))
{
    if(sockDescriptor < 0)
    {
        std::cerr << " Cannot create socket! " << std:: endl;
        exit(1);
    }
    
    sockaddr_in address;
    createAddress(ip_address, port , address);
    
    int result = connect(sockDescriptor, (sockaddr*)&address, sizeof(address));
    if(result < 0)
    {
        std::cerr << " Cannot connect to socket!" << std::endl;
        exit(1);
    }
}
/*  Destructor
    Closes the socket
*/
Socket::~Socket()
{
    close();
}

/*  setAsync
    Sets socket to be handle asynchronolusly by switiching socket
    to non-blocking I/O io handler using fcntl() function causing
    read() not to block.
*/
void Socket::setAsync()const
{
    
    fcntl(sockDescriptor, F_SETOWN, getpid());
    fcntl(sockDescriptor, F_SETFL, FASYNC);
    
}
/*  readInto
    Read into data buffer and displyas the data
    @output, data ostream output to display
*/
void Socket::readInto(std::ostream &output)
{
    //identify socket for 1000 ms
    char buf[Socket::BUF_SIZE];
    int nread;
    // while socket is ready to read
    while (poll(1000))
    {
        // guarantee to return from read
        nread = read<char>(buf, Socket::BUF_SIZE -1);
        if(nread == 0)
        {
            break;
        }
        // even if nread < BUFLEN
        output << std::string(buf, nread);
    }
}
/*  writeFrom
    Write from data buffer
    @input, data istream input to display
 */
void Socket::writeFrom(std::istream &input)
{
    char buf[Socket::BUF_SIZE];
    int i = 0;
    while(1)
    {
        if(i == Socket::BUF_SIZE)
        {
            i =0;
            this->write<char>(buf, Socket::BUF_SIZE);
        }
        else if(!(input >> buf[i]) || input.eof())
        {
            break;
        }
        i++;
    }
    
    if(i > 0)
    {
        write<char>(buf, i+1);
    }
}

/*  poll
    identifies socket descitptors to be read or write data
    @param timeout, the expriing time of the call
    @returns, true with a positive value indicating total number 
    of socket desciptor that have been selected
*/
bool Socket::poll(int timeout)
{
    
    struct pollfd ufds;
    ufds.fd = sockDescriptor;   // sd to read
    ufds.events = POLLIN;        // if sd is ready to read
    ufds.revents = 0;             // zero initialized
    
    int numEvents = ::poll(&ufds, 1, 1000);
    // if value is zero, the call timeout  and not sd is selected
    return ufds.revents & POLLIN && numEvents > 0;
}

/*  shutdown
    causes all  or part of a duplex connection associated on socket
    descriptor to shut down
*/
void Socket::shutdown()
{
    ::shutdown(sockDescriptor, SHUT_WR);
    
}
/*  close
    closes sd before desctruction of socket to remove its reference
    and allow it to be reusable
*/
void Socket:: close()
{
    ::close(sockDescriptor);
}
/*  createAddress
    Creates a network address representation of remote source
    @param ip_address, internet addres of remote soruce
    @param port, value of port that connects to remote resoruce
    @param address, resource address structure
*/
void Socket:: createAddress(const char *ip_address , const int port, sockaddr_in &address)
{
    struct hostent* host = gethostbyname(ip_address);
    
    memset((char *)&address, '\0', sizeof(address));
    address.sin_family = AF_INET;
    bcopy(host->h_addr, (char *)&address.sin_addr, host->h_length);
    address.sin_port = htons(port);
}