//================================================================
//  FileName    : FTPClient.h
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
/*  Description : 
    Implements a limited ftp client program that
    involves dealing with opening and closing a connection by
    establishing communication through a socket control object
    between ftp client and the remote hostname server using default 
    port 21 and transferring  files in passive mode by utilizing
    the following  FTP Access Control commands:
    
    -PASSIVE (PASV), which request server to send back its IP
    and port on which it listens to a data TCP connection from the
    user. It waits for a connection rather than initiate one upon 
    receipt of a transfer command. The response to this command 
    includes host and port address this server is listening on.
 
    -LOGOUT (QUIT), which terminates a USER and if file transfer is
    not in progress, the server closes the control connection and
    quit ftp.
*/
//================================================================
#ifndef FTPClient_h
#define FTPClient_h

#include <string>
#include <iostream>

#include "Socket.h"

class FTPClient
{
    public:
    
    FTPClient();
    ~FTPClient();
    bool isOpen() const;
    bool open(std::string hostname, int port=DEFAULT_PORT);
    void readInto(std::ostream &output);
    void writeFrom(std::istream &input);
    bool writeCmd(const std::string &cmd, std::ostream &output);
    
    Socket* openPassive(std::ostream &output);
    bool close(std::ostream &output, const bool force=false);
    bool pwd(std::ostream &out);
    const std::string getHostname(void) const;
    static const std::string END_LINE;
    static const int DEFAULT_PORT;

    private:
    
    // socket object cotnaing ip_address, port and socket address
    Socket *controlSocket;
    // Address of connected ftp server
    std::string hostname;
};

#endif /* FTPClient_h */