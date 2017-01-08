//================================================================
//  FileName    : Socket.h
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
/*  Description : Implements a socket descriptor that accepts 
    connection from a ftp client on a port socket that connects to
    an open source. */
//================================================================
#ifndef Socket_h
#define Socket_h

#include <netdb.h>      // gethostbyname
#include <unistd.h>     // read, write, close
#include <sys/poll.h>   // poll - input/output multiplexing
#include <iostream>     


class Socket
{
    public:
    
    Socket(const char *ip_address, const int port);
    ~Socket();
    void setAsync()const;
    bool poll(int timeout = 0);
    template<typename T>
    int read(T *dataBuffer, const int length)const;
    template<typename T>
    void write(T *dataBuffer, const int length)const;
    void readInto(std::ostream &output);
    void writeFrom(std::istream &input);
    void shutdown();
    
    private:
    
    const int sockDescriptor;       // refrence to the socket descriptor
    void close();                   // 
    void createAddress(const char *ip_address, const int port, sockaddr_in &address);
    static const int BUF_SIZE;

};

/*  read
    Read data buffer from remote destination
    @param dataBuffer, buffer array of data to read into
    @param length, capacity of buffer
    @ return number sd elements read
*/
    template<typename T>
    int Socket:: read(T *dataBuffer, const int length)const
    {
        return:: read(sockDescriptor, dataBuffer, length);
    }
/*  write
    Write a data buffer from remote destination
    @param dataBuffer, buffer array of data to write
    @param length, capacity of buffer
    @ return number of sd elements written
*/

    template<typename T>
    void Socket::write(T *dataBuffer, const int length) const
    {
        ::write(sockDescriptor, dataBuffer, length);
    }

#endif /* Socket_h */
