/*
 * BaseType.h
 *
 *  Created on: 2014.5.24
 *      Author: liyongshun
 */

#ifndef BASETYPE_H_
#define BASETYPE_H_

typedef unsigned char  BYTE;
typedef char CHAR;
typedef int  INT;
typedef short SHORT;
typedef long LONG;

typedef unsigned short WORD;
typedef unsigned short WORD16;
typedef unsigned long  WORD32;
typedef unsigned char BOOLEAN;

#ifdef WIN32
typedef unsigned __int64 WORD64;
#else
typedef unsigned long long WORD64;
#endif

#endif /* BASETYPE_H_ */
