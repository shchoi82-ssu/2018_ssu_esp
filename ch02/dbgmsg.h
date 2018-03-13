#ifndef __DEBUG__
#define __DEBUG__

#ifdef DEBUG
#define REDS    "\x1b[31m"
#define REDE    "\x1b[0m"
#define DBGMSG_PREFX    REDS "<< DBGMSG >> "

#define DBGMSG(msg,...) fprintf(stderr, \
        DBGMSG_PREFX"[%s %s %d] : " msg "\n" REDE, __FILE__, __func__, \
        __LINE__, ##__VA_ARGS__)

#else
#define DBGMSG(...)
#endif
#endif

