//================================================================
//  FileName    : MsgBuffer.cpp
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
//================================================================
#include "MsgBuffer.h"

/*  Constructor
    Initializes MsgBuffere objects variables that deal with 
    input/output streams, configuration and ftp connections.
    @param input, reference to the input istream
    @param output, reference to the output ostream
    @param workingDirectory, refrence to the file directory string
 */
MsgBuffer::MsgBuffer(std::istream &input, std::ostream &output):

    input(&input), output(&output), workingDirectory("")
{
    
}
