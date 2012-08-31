#include "Common.h"

#include <string>
using std::string;

#include <iostream>

#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <sstream>
using std::stringstream;

int my_itoa (char *to, unsigned int from)
{
    return sprintf (to, "%u", from);
}

unsigned int my_atoi (const char *to)
{
    return (unsigned int) strtoul (to, NULL, 10);
}

string GetCurrentTime ()
{
    char current_time[32] = { 0 };
    time_t now = time (NULL);
    struct tm *ptr = localtime (&now);

    strftime (current_time, 32, "%Y-%m-%d %H:%M:%S", ptr);

    return string (current_time);
}

string GetCurrentDate ()
{
    char current_time[32] = { 0 };
    time_t now = time (NULL);
    struct tm *ptr = localtime (&now);

    strftime (current_time, 32, "%Y-%m-%d", ptr);

    return string (current_time);
}

int writer( char *data, size_t size, size_t nmemb, string * writerData )
{
    unsigned long sizes = size * nmemb;

    if( writerData == NULL )
    {
        return 0;
    }

    writerData->append( data, sizes );

    return sizes;
}

ssize_t LoopRead( int fd, const void *vptr, const size_t n )
{
    int nread = 0;
    int nleft = n;
    char *ptr = ( char * ) vptr;

    if( n <= 0 )
        return -1;
    while( nleft > 0 )
    {
        if( ( nread = read( fd, ptr, nleft ) ) < 0 )
        {
            if( errno == EINTR )
                nread = 0;
            else
                return -1;
        }
        else if( nread == 0 )
            break;
        nleft = nleft - nread;
        ptr = ptr + nread;
    }
    return ( n - nleft );
}

int read_tmp_file( char *filename, string * p_buffer )
{
    struct stat statbuf;
    void *read_buf = NULL;
    int file_read = -1;

    memset( &statbuf, 0, sizeof( struct stat ) );

    if( !filename )
    {
        return -1;
    }

    if( strstr( filename, "../" ) || strstr( filename, "..\\" ) )
    {
        return -1;
    }

    if( !stat( filename, &statbuf ) )
    {
        if( statbuf.st_size == 0 )
            return -1;

        read_buf = ( void * ) malloc( statbuf.st_size + 1 );
        if( !read_buf )
        {
            return -1;
        }
        memset( read_buf, 0, statbuf.st_size + 1 );


        file_read = open( filename, O_RDONLY );
        if( ( !file_read == -1 )
            || ( LoopRead( file_read, read_buf, statbuf.st_size ) !=
                 statbuf.st_size ) )
        {
            if( read_buf )
            {
                free( read_buf );
                read_buf = NULL;
            }

            if( file_read == -1 )
            {
            }
            else
            {
                close( file_read );
            }
            return -1;
        }
        close( file_read );

        *p_buffer = string( ( char * ) read_buf );
        if( read_buf )
        {
            free( read_buf );
            read_buf = NULL;
        }
    }

    return 0;
}

void string_tolower(string &src)
{
    std::use_facet< std::ctype <char> >(std::locale()).tolower(
                    &(*src.begin()),&(*src.end()));
}

int my_str2int(const string& s)
{
	stringstream ss;
	ss << s;
	int ii = 0;
	ss >> ii;

	return ii;
}

string my_int2str(const int i)
{
	stringstream ss;
	ss << i;
	string s;
	ss >> s;

	return s;
}

bool is_tail(const string& str,const string& tail)
{
	if (str.find(tail,-tail.size()) != string::npos)
		return true;
	else
		return false;
}
