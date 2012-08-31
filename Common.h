#ifndef _myCOMMON_H
#define _myCOMMON_H

#include <string>
using std::string;

#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int my_itoa (char *to, unsigned int from);

unsigned int my_atoi (const char *to);

string GetCurrentTime ();

string GetCurrentDate ();

int writer( char *data, size_t size, size_t nmemb, string * writerData );

ssize_t LoopRead( int fd, const void *vptr, const size_t n );

int read_tmp_file( char *filename, string * p_buffer );

void string_tolower(string &src);

int my_str2int(const string& s);

string my_int2str(const int i);

bool is_tail(const string& str,const string& tail);

#endif /*_myCOMMON_H*/
