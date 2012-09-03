#ifndef _COMMON_H_
#define _COMMON_H_

#define RCPT_TOK '\n'
#define TO_MAIL_ADDR_SIZE 128

#include <vector>
#include <string>
using std::vector;
using std::string;

int my_itoa (char *to, unsigned int from);
unsigned int my_atoi (const char *to);
string GetCurrentTime ();
string GetCurrentDate ();

#endif /*_COMMON_H_*/
