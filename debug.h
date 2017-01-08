//================================================================
//  FileName    : debug.h
//  Course      : CSS 432
//  Student     : Mariana Chagoyan
//  Professor   : Robert Dimpsey
//  Date        : 05/28/2016 - 06/06/2016
//  Program 5   : FTP Client
/*  Description : Prevents the compiler from scanning same class
                    class definitions for socket and ftp
*/
//================================================================
#ifndef debug_h
#define debug_h

#ifdef DEBUG_ENABLE
#define DEBUG(out)out
#else
#define DEBUG(out)
#endif

#endif /* debug_h */
