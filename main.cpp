//================================================================
//  FileName    : main.cpp
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
/*  Description :
    
    It begins by implementing a simulation of factory class that
    creates command objects encapsulating Command class behavior
    that relate to functionality of their  data objects Status,
    MsgBuffer, and execute function with the following commands 
    classes :
    -   OpenCmd
    -   PWDCmd
    -   CloseCmd
    -   QuitCmd
    -   MkdirCmd
    -   MoveCmd
    -   CdCmd
    -   LsCmd
    -   GetCmd
    -   PutCmd
    -   Help
    
    In main section, user is prompt  to enter the corresponding variables
    for hostname, user name, password and matching commands. Then
    each auto_ptr Command variable is assigns to its corresponding 
    command class to create, to process and to manage its own objects
    as well as to delete itself when no longer needed. In addition, 
    Map structure commands variables are instated to maps each
    different type of command string to its corresponding command 
    function invoked via command line, and the OpeCmd initiates 
    the program.
*/
//================================================================
#include <iostream>
#include <map>
#include <memory>
#include <stdlib.h>
#include <pwd.h>
#include <sstream>
#include <fstream>

#include "Command.h"
#include "MsgBuffer.h"
#include "FTPClient.h"
#include "debug.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

//----------------------------------------------------------------
/*  #1
    OpenCmd class derived from base class Command
    OpenCmd utilizes MsgBuffer object Status to process 
    functionalities relate to :
    - execute prompt and save user data input (hostname port)
    - open network connection through ftp object
    - authorized user data input through ftp object
 */
//----------------------------------------------------------------
class OpenCmd : public Command
{
    public:
    
    /*  execute
        Prompt user for hostname and port. if it'ssuccessfully
        obtained, data is saved in msgbuffer object message and sent 
        to open function
        @param hostname, input for address of ftp server connection
        @param port, input for client port
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
    */
    Command::Status execute(MsgBuffer &message)
        {
            std::string hostname;
            int port;
            *message.input >> hostname >> port;
            return open(hostname, port, message);
        }
        
    /*   open
        Opens establishes a connection with hostname and port data
        Utilizes MsgBuffer object ftp to call FTPclient function
        to open connection, to read data input and sent this data
        to authorize function.
        @param hostname, input for address of remote hostname server
        @param port, client default port 21 input
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
    */
    Command::Status open(std::string hostname, int port, MsgBuffer &message)
    {
        
        *message.output << " Open connection to \"" << hostname << ":"
        << port << "\"" << std::endl;
        message.ftp.open(hostname, port);
        message.ftp.readInto(*message.output);
        
        Command::Status authResult = authorize(message);
        if(authResult != OK)
        {
            return authResult;
        }
        
        std::stringstream currDirStream;
        if(!message.ftp.pwd(currDirStream))
        {
            return ERROR;
        }
        
        int pwd_code;
        currDirStream >> pwd_code;
        currDirStream >> message.workingDirectory;
        return OK;
    }
    
    /*   authorize
        Authorizes data input msg for ftp server utilizing MsgBuffer
        objects msg and ftp to call FTPClient function commands writeCmd
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK    
    */
    Command::Status authorize(MsgBuffer &message)
    {
        struct passwd *pass;
        pass = getpwuid(getuid());
        std::string netid(pass->pw_name);
        std::string input;
        
        *message.output << "Name ("
        << message.ftp.getHostname()
        << ":" << netid << "): ";
        *message.input >> input;
        if(!message.ftp.writeCmd("USER " + input + FTPClient::END_LINE,
                                 *message.output))
        {
            return ERROR;
        }
        std::cout << "Password: ";
        *message.input >> input;
        if(!message.ftp.writeCmd("PASS " + input + FTPClient::END_LINE,
                                 *message.output))
        {
            return ERROR;
        }
        return OK;
    }
};
//----------------------------------------------------------------
/*  #2
    PWDCmd class derived from base class Command
    PWDCmd class prints working directory by utilizing MsgBuffer objects
    status, message and ftp to execute message command
*/
//----------------------------------------------------------------
class PWDCmd : public Command
{
public:
    
    /*  execute
        execute message containg password
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
     */
    Command::Status execute(MsgBuffer &message)
    {
        if(!message.ftp.pwd(*message.output))
        {
            return ERROR;
        }
        return OK;
    }
};
//----------------------------------------------------------------
/*  #3
    CloseCmd Class derived from base class Command
    CloseCmd class utilizes MsgBuffer objects status message and 
    ftp to execute close command
*/
//----------------------------------------------------------------
class CloseCmd : public Command
{
public:
    /*  execute
        execute message containg close command and close current    
        connection to ftp server if it is open.
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
     */
    Command::Status execute(MsgBuffer &message)
    {
        if(message.ftp.isOpen())
        {
            message.ftp.close(*message.output);
            return OK;
        }
        else
        {
            *message.output << " No connection to close." << std::endl;
            return ERROR;
        }
    }
};
//----------------------------------------------------------------
/*  #4
    QuitCmd Class derived from base class Command
    QuitCmd class utilizes MsgBuffer object status and message to 
    execute close command 
*/
//----------------------------------------------------------------
class QuitCmd : public CloseCmd
{
    public :
    /*   execute
        execute message containg quit command to close current 
        connection to ftp server and exit the running program
        @param message, input/out msg and ftp command socket object
        @returns, status result EXIT
    */
    Command::Status execute(MsgBuffer &message)
    {
        CloseCmd:: execute(message);
        *message.output << "GOODBYE!" << std::endl;
        return EXIT;
        
    }
};
//----------------------------------------------------------------
/*  #5
    MkdirCmd Class derived from base class Command
    MkdirCmd class utilizes MsgBuffer objects status, message and
    ftp to execute make directory.
*/
//----------------------------------------------------------------
class MkdirCmd : public Command
{
public:
    /*   execute
        execute message containg quit command and make a new directory
        on the ftp server by calling write command.
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
     */
    Command::Status execute(MsgBuffer &message)
    {
        if(!message.ftp.isOpen())
        {
            return ERROR;
        }
        
        std::string dir;
        *message.input >> dir;
        if(!message.ftp.writeCmd("MKD " + dir + FTPClient::END_LINE,
                                 *message.output))
        {
            return ERROR;
        }
        return OK;
    }
};
//----------------------------------------------------------------
/*  #6
    MoveCmd Class derived from base class Command
    MoveCmd class utilizes MsgBuffer objects status, message and
    ftp to execute move command
 */
//----------------------------------------------------------------
class MoveCmd : public Command
{
public:
    /*   execute
        execute message containg quit command, which renames a file
        on the ftp server, changes its name, but not its contents 
        and file doesn't need to be downloaded.
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
     */

    Command::Status execute(MsgBuffer &message)
    {
        if(!message.ftp.isOpen())
        {
            return ERROR;
        }
        std::string orgigin, dest;
        *message.input >> orgigin >> dest;
        std::stringstream moveCmdStream;
        moveCmdStream << "RNFR" << orgigin << FTPClient::END_LINE
        << "RNTO" << dest << FTPClient::END_LINE;
        if(!message.ftp.writeCmd(moveCmdStream.str(), *message.output))
        {
            return ERROR;
        }
        return OK;
    }
};

//----------------------------------------------------------------
/*  #7
    CdCmd class derived from base class Command
    CdCmd class utilizes MsgBuffer objects status, message and
    ftp to execute change directory command.
*/
//----------------------------------------------------------------
class CdCmd : public Command
{
public:
    /*   execute
        execute message containg change directory command which 
        changes the current directory in the ftp server by calling 
        write command and updates current working directory in 
        stringstream local variable currDirStream.
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
    */
    Command::Status execute(MsgBuffer &message)
    {
        std::string directory;
        *message.input >> directory;
        if(!message.ftp.writeCmd("CWD " + directory + FTPClient::END_LINE, *message.output))
        {
            return ERROR;
        }
        
        std::stringstream currDirStream;
        if(!message.ftp.pwd(currDirStream))
        {
            return ERROR;
        }
        int pwdCode;
        currDirStream >> pwdCode;
        currDirStream >> message.workingDirectory;
        return OK;
    }
};
//----------------------------------------------------------------
/*  #8
    LsCmd class derived from base class Command
    LsCmd class utilizes MsgBuffer objects status, message and
    ftp to execute list files command.
*/
//----------------------------------------------------------------
class LsCmd : public Command
{
public:
    /*   execute
        execute message containg list all files command which 
        lists all files and folders in current directory.
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK 
    */
    Command::Status execute(MsgBuffer &message)
    {
        Socket *dataSocket = message.ftp.openPassive(*message.output);
        if(dataSocket == NULL)
        {
            *message.output << "Cannot establish data connection: " <<
            std::endl;
            return ERROR;
        }
        pid_t pid;
        if((pid = fork()) == -1)            // fork
        {
            perror("fork error: " );    // check for error
            exit(EXIT_FAILURE);
            
        }
        else if(pid == 0)                   // child process
        {
            dataSocket->readInto(*message.output);
            exit(EXIT_SUCCESS);
        }
        else                                // parent process
        {
            if(!message.ftp.writeCmd("LIST" + FTPClient::END_LINE, *message.output))
            {
                return ERROR;
            }
        }
        delete dataSocket;
        dataSocket = NULL;
        return OK;
    }
};
//----------------------------------------------------------------
/*  #9
    GetCmd Class derived from base class Command
    GetCmd class utilizes MsgBuffer objects status, message and
    ftp to execute get command involving :
    - Passive move (PASV) and retrieve (RETR) commands
    - Local file options to open to create or for writing only
        (O_CREATE | O_WRONLY)
    - Local file permissions read and write for user, group and other
      which flags are S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
*/
//----------------------------------------------------------------
class GetCmd: public Command
{
public:
    /*   execute
        execute message containg get command which gets file from
        ftp server.
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
    */
    Command::Status execute(MsgBuffer &message)
    {
        struct timeval startTime, endTime;
        std::string fileName;
        *message.input >> fileName;
        
        // permission flags = read user| write user| read group| read other
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        
        if(open(fileName.c_str(), O_WRONLY | O_CREAT, mode))
        {
            Socket *dataSocket = message.ftp.openPassive(*message.output);
            if(dataSocket == NULL)
            {
                *message.output << "Cannot established connection: "
                << std::endl;
                return ERROR;
            }
            
            pid_t pid;
            if((pid = fork()) == -1)
            {
                perror("fork error: ");
                exit(EXIT_FAILURE);
            }
            else if(pid == 0)
            {
                std::fstream file;
                file.open(fileName.c_str(), std::fstream::in |
                          std::fstream::out);
                dataSocket->readInto(file);
                dataSocket->shutdown();
                file.close();
                exit(EXIT_SUCCESS);
            }
            else
            {
                if(!message.ftp.writeCmd("TYPE I" + FTPClient::END_LINE,
                                           *message.output))
                {
                    return ERROR;
                }
                gettimeofday(&startTime, NULL);
                // RETR ftp server transfers a copy of the file
                message.ftp.writeCmd("RETR " + fileName + FTPClient::END_LINE,
                                       *message.output);
                int status;
                waitpid(pid, &status, 0);
                gettimeofday(&endTime, NULL);
                message.ftp.readInto(*message.output);
            }
            
            delete dataSocket;
            dataSocket = NULL;
            
            double dt = ((double)(endTime.tv_sec - startTime.tv_sec) +
                         (double)(endTime.tv_sec - startTime.tv_sec) / 1000000);
            *message.output << "Received in " << dt << "seconds" << std::endl;
            return OK;
        }
        else{
            *message.output << "File error: " << std::endl;
            return ERROR;
        }
        
    }
    
};
//----------------------------------------------------------------
/*  #10
    PutCmd class derived from base class Command
    PutCmd class utilizes MsgBuffer objects status, message and
    ftp to execute put command which puts a file on the server 
    involving:
    - local file and remote file
    - Permission flags for remote files directory options 
        to open and to read only (O_DIRECTORY | O_RDONLY)
    - Local file permissions read and write for user, group and other
    which flas are S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
*/
//----------------------------------------------------------------
class PutCmd : public Command
{
public:
    /*   execute
        execute message containg put command which gets file from
        ftp server which puts a file on the server
        @param message, input/out msg and ftp command socket object
        @returns, status result type ERROR or OK
     */
    Command::Status execute(MsgBuffer &message)
    {
        std::string localFile;
        std::string remoteFile;
        
        char userInput = message.input->peek();
        if(userInput == '\n')
        {
            *message.output << "(local-file) ";
            *message.input >> localFile;
            *message.output << "(remote-file) ";
            *message.input >> remoteFile;
        }
        else
        {
            *message.input >> localFile;
            *message.input >> remoteFile;
        }
        
        // permission flags = read user | write user | read group | read other
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int dirfd = open(remoteFile.c_str(), O_DIRECTORY | O_RDONLY, mode);
        fchdir(dirfd);
        
        Socket *dataSocket = message.ftp.openPassive(*message.output);
        if(dataSocket == NULL)
        {
            *message.output << "Cannot established connection :" <<
            std::endl;
            return ERROR;
        }
        
        // Fork process
        pid_t pid;
        if((pid = fork()) == -1 )
        {
            perror("fork error: ");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0)
        {
            std::fstream file;
            file.open(localFile.c_str(), std::fstream::in | std::fstream::out);
            dataSocket->writeFrom(file);
            dataSocket->shutdown();
            file.close();
            exit(EXIT_SUCCESS);
        }
        else
        {
            if(!message.ftp.writeCmd("TYPE I" + FTPClient::END_LINE, *message.output))
            {
                return ERROR;
            }
            if(!message.ftp.writeCmd("STOR " + remoteFile + FTPClient:: END_LINE,
                                       *message.output))
            {
                return ERROR;
            }
            int status = 0;
            waitpid(pid, &status, 0);
            message.ftp.readInto(*message.output);
        }
        delete dataSocket;
        dataSocket = NULL;
        return OK;
    }
};

//----------------------------------------------------------------
/*  #11
    Help class derived from base class Command
    Help displays all the available commands and short description
 */
//----------------------------------------------------------------
class Help : public Command{
    
public:
    
    Command::Status execute(MsgBuffer &message)
    {
        *message.output<< "Commands: " << std::endl
        << "open <host> <port> - open new ftp connection: " << std::endl
        << "pwd - print working directory " << std::endl
        << "ls  - list all files in current directory" << std::endl
        << "cd <directory> - change current working directory"
        << "directory to given directory " << std::endl
        << "mkdir <directory > - create new directory  " << std::endl
        << "within current directory" << std::endl
        << "move <origin> <dest> - rename a file " << std::endl
        << "get < filename> - download remote file  " << std::endl
        << "put < filename> - upload local file " << std::endl;
        
        return OK;
    }
};

int main(int argc, char *argv[])
{
    std::string hostname = "";
    if(argc > 1)
    {
        std::string arg1 = argv[1];
        if(arg1 == "help")
        {
            std::cout << " usage: " << argv[0] << "[ftpserver]" << std::endl;
            
        }
        else
        {
            hostname = std::string(arg1);
        }
    }
    
    /*   Each auto_ptr Command variable assigns each command type to
        its corresponding command class to create and to manage its
        own object obtian as well as to delet its object when each
        auto_ptr is destroyed 
    */
    std::auto_ptr<Command> open(new OpenCmd());
    std::auto_ptr<Command> quit(new QuitCmd());
    std::auto_ptr<Command> cd(new CdCmd());
    std::auto_ptr<Command> close(new CloseCmd());
    std::auto_ptr<Command> ls(new LsCmd());
    std::auto_ptr<Command> get(new GetCmd());
    std::auto_ptr<Command> put(new PutCmd());
    std::auto_ptr<Command> pwd(new PWDCmd());
    std::auto_ptr<Command> mkdir(new MkdirCmd());
    std::auto_ptr<Command> help(new Help());
    std::auto_ptr<Command> move(new MoveCmd());
    
    /*   Map variables commands maps each command string to its corresponding
        command function invoked via command line
    */
    std::map<std::string,Command*> commands;
    commands["open"] = open.get();
    commands["quit"] = quit.get();
    commands["cd"] = cd.get();
    commands["close"] = close.get();
    commands["ls"] = ls.get();
    commands["get"] = get.get();
    commands["put"] = put.get();
    commands["pwd"] = pwd.get();
    commands["mkdir"] = mkdir.get();
    commands["help"] = help.get();
    commands["move"] = move.get();
    commands["mv"] = move.get();
    
    
    MsgBuffer message(std::cin, std::cout);
    if(hostname.size() > 0)
    {
        OpenCmd().open(hostname, FTPClient::DEFAULT_PORT, message);
    }
    std::string shellName = "ftp";
    std::string commandStr;
    
    while (1)
    {
        std::cout << shellName << ":" << message.workingDirectory << "> "
            << std::flush;
        std::cin >> commandStr;
        
        
        std::map<std::string,Command*>::iterator itCmd = commands.find(commandStr);
        
        if(itCmd == commands.end())
        {
            std::cerr << "Command \"" << commandStr << "\" Does not exist!"
            << std::endl;
        }
        else
        {
            Command::Status returnCode = itCmd->second->execute(message);
            
            switch (returnCode)
            {
                case Command::ERROR: std::cout << "ERROR!" << std::endl;break;
                case Command::EXIT: std::cout << " EXITING ..." <<  std::endl;
                    return 0;
                default:break;
            }
            
        }
        
    }
}