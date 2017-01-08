# ftp
File Transfer Protocol 
FTP Client
Background 

File Transfer Protocol (FTP) is a standard network protocol used to transfer computer files between a client and server on a computer network. FTP is built on client-server model architecture and uses separate control and data connections between a client and server. FTP users authenticate themselves in the form of a username and password, but can connect anonymously if the server is configured to allow it. 

FTP Client Data Transfer Mode Types 

While transferring data over the network, FTP Client uses Image mode commonly called Binary mode data connection. The sending machine sends each file byte for byte, and the recipient stores the bytestream as it receives it.  FTP Data Transfer in Stream Mode, data is sent as a continuous stream, relieving FTP from doing any processing. Rather all processing is left up to TCP. FTP Data Transfer in Block Mode FTP breaks the data into several blocks (block header, byte count, and data field) and then passes it on to TCP. 

Description

FTP Client project implement an ftp client program based on the RFC protocol. FTP Client implements a socket descriptor that accepts connection from a ftp client on a port socket that connects to an open source. FTP Client program runs in passive mode (PASV). Client creates a TCP control connection from a random port to the FTP server command port 21. Client uses the control connection to send a PASV command to the server and then receives a server IP address and server port number from the server, which client uses to open a data connection from an arbitrary client port to the server IP address and server port number received. The server responds over the control connection with three-digits status codes in ASCII, representing the code for the response and an optional text message representing an explanation or requests.

Design

The FTP Client project is design in 4 main classes:
-	Class Main: works as the driver of the program and a factory for all different Commands Classes 
-	Class Command : Base class for commands
-	Class MsgBuffer: store command to be executed
-	Class FTPClient: connects a socket control object between ftp client and the remote hostname
-	Class Socket:  creates socket object for connection from a ftp client 
-	Class Debug:  prevents compiler from scanning same class definitions for socket and ftp classes
When debugging program 

Implementation
The FTP Client focuses on limited functionalities in terms of USER, PASS, SYST, PASV, LIST, RETR, STOR, and QUIT. The program is invoked with ftp [hostname]. It connects a session to the remote hostname server using the default port 21 and transfers files in passive mode. If no argument is given, the client program does not establish any connection until it receives an open command. FTP Client responds to a user authentication using css432 account with password provided in class, and perform cd, ls, get, put, close and quit commands. 


Class Main (main.cpp)
In main () section, user is prompt to enter the corresponding variables for hostname, user name, password and matching commands. Then each auto_ptr Command variable is assigns to its corresponding command class to create, to process and to manage its own objects as well as to delete itself when no longer needed. In addition, Map structure commands variables are instated to maps each different type of command string to its corresponding command function invoked via command line.

Main is the driver of the FTP client program and it begins by implementing a simulation of factory class that creates command objects encapsulating Command class behavior that relate to functionality of their data objects Status, MsgBuffer, and execute () function with the following commands classes:


1.	OpenCmd :  open <host> <port> - open new ftp connection
OpenCmd class utilizes MsgBuffer object Status to process functionalities relate to:
    - Execute prompt and save user data input (hostname port)
    - Open network connection through ftp object
    - Authorized user data input through ftp object
Behavior:  open, establish a TCP connection to ip on port and send user identification to the server.
FTP CLIENT Interface in terms of OPEN, PASS, AND  QUIT	
- Command::Status open(string hostname, int port, MsgBuffer &message) 
Establishes a connection with hostname and data port by utilizing MsgBuffer object ftp to call FTPclient function to open connection, to read data input and sent this data to authorize function.
-Command::Status authorize(MsgBuffer &message)
Authorizes data input msg for ftp server utilizing MsgBuffer objects msg and ftp to call FTPClient function commands writeCmd
CloseCmd:: execute(message); 
return EXIT;

2.	PWDCmd  pwd – print woking directory 
PWDCmd class prints working directory by utilizing MsgBuffer objects status, message and ftp to execute message command.
Command: PASS user_password SYST	
Behavior: Send the user password to the server.
FTP Client interface: password: user_password	

3.	CloseCmd
Command: QUIT
CloseCmd class utilizes MsgBuffer objects status message and ftp to execute close command and close current connection to ftp server if it is open.
FTP Client interface: Close ; Close the connection but not quit ftp.

4.	QuitCmd
Command: QUIT (if not closed)
QuitCmd class utilizes MsgBuffer object status and message to execute close command and to close current connection to ftp server and exit the running program.
FTP Client interface: quit ;Close the connection and quit ftp.

5.	MkdirCmd  mkdir <directory > - create new directory within current directory 
Command: MKD 
MkdirCmd class utilizes MsgBuffer objects status, message and ftp to and make a new directory on the ftp server by calling write command.
FTP Client interface: MkdirCmd; Make Directory based on the directory specified in the pathname to be created as a directory.

6.	MoveCmd  move<origin> <dest> -rename a file
 Command: MoveCmd
MoveCmd class utilizes MsgBuffer objects status, message and ftp to execute move command, which renames a file on the ftp server, changes its name, but not its contents and file doesn't need to be downloaded. Checks if file is open, if it is creates origin and destination variables and uses 2 other command: #1 Rename from (RNFR), which specifies the old pathname of the file, which is to be renamed, and its immediately followed by a "rename to" command specifying the new file pathname.
#2 Rename to (RNT), which specifies the new pathname of the file specified in the immediately preceding "rename from" command.  Together the two commands cause a file to be renamed.
FTP Client interface: move<origin> <dest> -rename a file

7.	CdCmd cd <directory> - change current working directory from directory to given directory 
Command: CWD
CdCmd class utilizes MsgBuffer objects status, message and ftp to execute change directory command,
which changes the current directory in the ftp server by calling write command and updates current working directory in  stringstream local variable currDirStream.
FTP Client interface: cd function subdir; it change the server's current working directory to subdirectory.

8	LsCmd   <ls>  - list all files in current directory
Command: PASV LIST
LsCmd class utilizes MsgBuffer objects status, message and ftp to execute list files command, which lists all files and folders in current directory.
FTP Client interface: lists all files in current directory by asking the server to send back its current directory contents through the data connection

9 	GetCmd get < filename> - download remote file
GetCmd class utilizes MsgBuffer objects status, message, and ftp to execute get command, which gets file from the current remote directory (ftp server). 
User is prompt for the file name and the name is stored on string variable file.  
FTP Client sets permissions flags for file modes before writing a file received from the server utilizing permission flags to set up the type of mode the client needs to verify local file permission for the get command. Then Socket objet dataSocket is used to establish connection with ftp server and MsgBuffer object message is used for calling WriteCmd class to deal with writing disks functionalities. 

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
S_IRUSR : read permission, owner
S_IWUSR : write permission, owner
S_IRGRP : read permission, group
S_IROTH : read permission, other
Command: PASV RETR file
FTP Client interface in terms of PASV, RETR file (O_CREATE | O_WRONLY)  
FTP server transfers a copy of the file through command Retrieve (RETR) and (PASV) passive mode  connection request server waits for a connection rather than initiate one upon receipt of a transfer command. FTP client has to specify what type of mode client need when writing to its local disk a file received from the server options an uses the two other open file flags:
- Local file options to open to create or for writing only
O_WRONLY:  if the file exists, it is open for writing only 
O_CREATE :  open to create. if the file exits, this flag has no effect . But if does not exits, the file can be created and the user ID of the  file shall be et to the group ID of the file’s parent directory. 

10	PutCmd put < filename> - upload local file and store file into the current remote directory
Command: PASV STOR file	
PutCmd class utilizes MsgBuffer objects status, message and ftp to execute message that contains the put command, which gets file from ftp server. 
Command::Status execute(MsgBuffer &message)
User gets prompt to put filename input that it needs to be store and from which remote file it comes from, so that data is stored in 2 string variables localFile and remote file FTP Client sets permissions flags for file modes before writing a file received from the server utilizing permission flags to set up the type of mode the client needs to verify local file permission for the get command. 
Permission flags for remote files directory options to open and to read only are:
O_DIRECTORY :open directory but if pathname is not a directory cause the open to fail. 
O_RDONLY : open for reading only 
Local file permissions read and write for user, group and other which flags are:
 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
FTP Client interface: put file in terms of PASV, STOR, and local file option (O_RDONLY)	
(PASV) passive mode connection request server waits for a connection rather than initiate one upon receipt of a transfer command. Then STORE (STOR) command causes server to accept the data transferred via the data connection and to store the data as a file at the server site.  If the file specified in the pathname exists at the server site, then its contents should be replaced by the data being transferred.  A new file is created at the server site if the file specified in the pathname does not already exist. Then it puts a file on the server involving the following:
int dirfd = open(remoteFile.c_str(), O_DIRECTORY | O_RDONLY, mode)

11.	Help – help 
Help displays all the available commands and short description.

Class Command

Command class is the base class for all the commands and it is used as interfaces for all the different types of commands called by user on the command line through the driver of the program.
Command class contains 2 objects:
Status type enum containing OK, ERROR and EXIT variables and MsgBuffer message containing the command 
It also has a private method execute(MsgBuffer &message) to connected with execution class in the driver class.

Class MsgBuffer

Implements functionalities that deal with input/output streams, configuration and ftp connections by utilizing a FTPClient object behavior to assign a given type to a message and in return give capabilities to multiplexing messages.

MsgBuffer(std::istream &input, std::ostream &output) 
Constructor declare two variables:  
istream *input - pointer to input stream of message buffer
ostream *output - pointer to output stream of message buffer
    
MsgBuffer class it has 2 objects:
ftp object type FTPClient pointer to ftp connection in message buffer
 workingDirectory type string representing the name of working directory

Class FTPClient

Implements a limited ftp client program that involves dealing with opening and closing a connection by establishing communication through a socket control object between ftp client and the remote hostname server using default port 21 and transferring files in passive mode by utilizing the following FTP Access Control commands:

-(PASV) passive connection request server to send back its IP and port on which it listens to a data TCP connection from the user/client. It waits for a connection rather than initiate one upon receipt of a transfer command. The response to this command includes host and port address this server is listening on.
 
-LOGOUT (QUIT), which terminates a USER and if file transfer is not in progress, the server closes the control connection and quit ftp.

Class Socket

Implements a socket descriptor that accepts connection from a ftp client on a port socket that connects to an open source. It has 2 private objects:
 controlSocket object, which it has a pointer type Socket to the  address ip, port and socket address
 hostname objet, which is a string holding the hostname variable containing the Address of connected ftp server

Socket(const char *ip_address, const int port) 

Creates an open connection to open resources which requires an ip address for the network location of remote resource to connect to and a port, client port input
 


