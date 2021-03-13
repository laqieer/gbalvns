#ifndef __MGBA_H__
#define __MGBA_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "gba.h"

#define REG_LOG_STR      (char*) 0x4FFF600
#define REG_LOG_LEVEL   *(vu16*) 0x4FFF700
#define REG_LOG_ENABLE  *(vu16*) 0x4FFF780

#define LOG_FATAL       0x100
#define LOG_ERR         0x101
#define LOG_WARN        0x102
#define LOG_INFO        0x103

IWRAM_CODE void mgbalog(const char *msg, const u32 level);


#ifdef __cplusplus
}
#endif
#endif
