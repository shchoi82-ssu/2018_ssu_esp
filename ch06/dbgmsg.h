#ifndef __DBGMSG__
#define __DBGMSG__

#include <stdio.h>

#ifdef _DBGMSG
// regular
#define REDS           "\x1b[1;31m"
#define GREENS         "\x1b[1;32m"
#define YELLOWS        "\x1b[1;33m"
#define BLUES          "\x1b[1;34m"
#define PURPLES        "\x1b[1;35m"
#define CYANS          "\x1b[1;36m"
#define WHITES         "\x1b[1;37m\x1b[43m"

// Bold High Intensty
#define IREDS           "\x1b[0;103m\x1b[4;32m\x1b[1;91m"
#define IGREENS         "\x1b[1;92m"
#define IYELLOWS        "\x1b[1;93m"
#define IBLUES          "\x1b[1;94m"
#define IPURPLES        "\x1b[1;95m"
#define ICYANS          "\x1b[0;104m\x1b[1;96m"
#define IWHITES         "\x1b[1;97m"

#define COLORE         "\x1b[0m"

#define DBGMSG_PREFX    "<< DMSG >> "
#define DBGMSGD_PREFX   "<< DEBUG>> "
#define DBGMSGI_PREFX   "<< INFO >> "
#define DBGMSGW_PREFX   "<< WARN >> "
#define DBGMSGE_PREFX   "<< ERROR>> "

#define DBGMSG(msg,...) fprintf(stderr, \
		ICYANS DBGMSG_PREFX"[%s %s %d] : " msg "\n" COLORE, __FILE__, __func__, \
		__LINE__, ##__VA_ARGS__)

//DEBUG
#define DBGMSGD(msg,...) fprintf(stderr, \
		IPURPLES DBGMSGD_PREFX"[%s %s %d] : " msg "\n" COLORE, __FILE__, __func__, \
		__LINE__, ##__VA_ARGS__)

//INFO
#define DBGMSGI(msg,...) fprintf(stderr, \
		IGREENS DBGMSGI_PREFX"[%s %s %d] : " msg "\n" COLORE, __FILE__, __func__, \
		__LINE__, ##__VA_ARGS__)

//WARN
#define DBGMSGW(msg,...) fprintf(stderr, \
		IYELLOWS DBGMSGW_PREFX"[%s %s %d] : " msg "\n" COLORE, __FILE__, __func__, \
		__LINE__, ##__VA_ARGS__)

//ERROR
#define DBGMSGE(msg,...) fprintf(stderr, \
		IREDS DBGMSGE_PREFX"[%s %s %d] : " msg "\n" COLORE, __FILE__, __func__, \
		__LINE__, ##__VA_ARGS__)

#else
#define DBGMSG(...)
#define DBGMSGD(...)
#define DBGMSGI(...)
#define DBGMSGW(...)
#define DBGMSGE(...)
#endif
#endif


