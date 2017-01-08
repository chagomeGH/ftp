//================================================================
//  FileName    : Command.h
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
/*  Description : Command class is the based class and it is used 
    as an interfaces for all the different types of commands called 
    by user on the command line through the driver of the program.
*/
//================================================================
#ifndef Command_h
#define Command_h


#include "MsgBuffer.h"

class Command
{
    
    public:
    
    enum Status
    {
        OK, ERROR, EXIT
    };

    virtual Command::Status execute(MsgBuffer &message) = 0; // pure virtual

    virtual ~Command(){}                                       // pure virtual
    
};

#endif /* Command_h */
