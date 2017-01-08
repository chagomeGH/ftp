//================================================================
//  FileName    : MsgBuffer.h
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
/*  Description : implements functionalities that deal with input/
    output streams, configuration and ftp connections by utilizing
    a FTPClient object behavior to assign a given type to a message
    and in return give capabilities to multiplexing messages.
*/
//================================================================
#ifndef MsgBuffer_h
#define MsgBuffer_h

#include <iostream>

#include "FTPClient.h"

class MsgBuffer
{
public:
    
    MsgBuffer(std::istream &input, std::ostream &output);
    
    std::istream *input;    // pointer to input stream of message buffer
    std::ostream *output;   // pointer to output stream of message buffer
    
    FTPClient ftp;          // pointer to ftp connection in message buffer
    std::string workingDirectory; // the name of working directory
};
#endif /* MsgBuffer_h */
