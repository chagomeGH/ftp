//================================================================
//  FileName    : FTPClient.cpp
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
//================================================================
#include "FTPClient.h"
#include "Socket.h"
#include <string.h>     // strcpy, strcat
#include <stdio.h>      // printf
#include <sstream>      // istringstream, ostringstream
#include <sys/types.h>

#include "debug.h"

// FTP return code if server is closing connection
#define CLOSE_CONN 421

const int FTPClient::DEFAULT_PORT(21);
const std::string FTPClient::END_LINE("\r\n");

/*  Constructor
    Instantiates a null controlSocket object
*/
FTPClient::FTPClient()
{
    controlSocket = NULL;
}
/*  Destructor
    Closes controlSocket object connections
*/
FTPClient::~FTPClient()
{
    if(isOpen())
    {
        close(std::cerr);
    }
}
/*  isOpen
    identifies if controlSocket is open
    @returns, true if control socket is not null
*/
bool FTPClient::isOpen()const
{
    return controlSocket != NULL;
}
/*  open
    identifies if controlSocket is not already open and if its 
    closed, it connects a session to the remote hostname server 
    using default port 21.
    @param hostname, value of remote hostname server
    @param port, default port value  that connects to remote resoruce
    @returns, true if controlSocket was not already open
*/
bool FTPClient:: open(std::string hostname, int port)
{
    if(isOpen())
    {
        return false;
    }
    this->hostname = hostname;
    controlSocket = new Socket(hostname.c_str(), port);
    return true;
}
/*  readInto
    Read from socketControl object into a given output stream
    @output, data ostream output to display
*/
void FTPClient::readInto(std::ostream &output)
{
    controlSocket->readInto(output);
}
/*  writeFrom
    Write into socketControl object from a given output stream
    @input, data istream input to display
*/
void FTPClient:: writeFrom(std::istream &input)
{
    controlSocket->writeFrom(input);
}
/*  openPassive
    Establishes a new passive connection with ftp server by requesting
    an address and port if not argument is given, client doesn't 
    established any connection until it receives a PASV
    @param output, output ostream displaying written results
    @returns, a new tcp socket if connection was established or 
    null if an error occur an connection was not established.
*/
Socket* FTPClient:: openPassive(std::ostream &output)
{
    std::stringstream responseStream;
    // PASV : requests server to send back its IP and port on which
    // it listent to a data TCP connection from user
    if(!writeCmd("PASV" + END_LINE, responseStream))
    {
        return NULL;
    }
    output << responseStream.str();
    int code = 0;
    int host1, host2, host3, host4, port1, port2;
    responseStream >> code;

    if(code != 227)     // code 227 = entering passive mode
    {
        return NULL;
    }
    responseStream.ignore(200, '(');
    responseStream >> host1; responseStream.ignore();
    responseStream >> host2; responseStream.ignore();
    responseStream >> host3; responseStream.ignore();
    responseStream >> host4; responseStream.ignore();

    std::stringstream hostStream;
    hostStream << host1 << "." << host2 << "." << host3 << "." << host4;
    std::string host = hostStream.str();
    responseStream >> port1; responseStream.ignore();
    responseStream >> port2;
    // TCP port for data transfer (one time)
    int port = port1 * 256 + port2;
    DEBUG(std::cout << "Open data connection to \"" << host << ";" << port << "\"" << std::endl;)
    return new Socket(host.c_str(), port);
}

/*  close
    close current controlSocket connection and quit ftp if writen 
    command equals QUIT if not close
    @param output, output ostream displaying written results
    @param force, set to false as a defalult
    @returns, true if connection was successfully closed false if not
*/
bool FTPClient::close(std::ostream &output, const bool force)
{
    
    if(!writeCmd("QUIT" + END_LINE, output))
    {
        return false;
    }
    
    delete controlSocket;
    controlSocket = NULL;
    return true;
}
/*  getHostname
    gets address of current connected ftp sever
    @returns, the address of the hostnate ftp sever
 */
const std::string FTPClient::getHostname(void)const
{
    return hostname;
}

/*  writeCmd
    Write the corresponding ftp command to the current ftp server
    and reads and outputs the returning code command, if code 421
    handles CLOSE_CONN by deleting controlSocket
    @param cmd, the string command to be interpreted
    @param output, the message output to be displayed
    returns@ false if connection is closed and true if not
 */
bool FTPClient::writeCmd(const std::string &cmd, std::ostream &output)
{
    if(!isOpen())
    {
        return false;
    }
    controlSocket->write<const char>(cmd.c_str(), cmd.size());
    std::stringstream result;
    int returnCode;
    
    readInto(result);
    output << result.str();
    
    result >> returnCode;
    if(returnCode == CLOSE_CONN) //code 421
    {
        delete controlSocket;
        controlSocket = NULL;
        output << " Connection close by server!" << std::endl;
        return false;
    }
    return true;
}
/*  pwd
    Retrieves and writes current working directory to an output souce
    by returning the PWD written command to the current ftp sever
    @param out, the message to be displayed
    returns@ a call to function writeCmd
 */

bool FTPClient:: pwd(std::ostream &out)
{
    return writeCmd("PWD" + END_LINE, out);
}
