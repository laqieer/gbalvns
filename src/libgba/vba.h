#ifndef __VBA_H__
#define __VBA_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "gba.h"

/**
 * Records a call graph entry. R12 should contain LR of previous caller.
 */
IWRAM_CODE void mcount();
/**
 * Starts profiling on the given address range (low ... high)
 */
IWRAM_CODE void monstartup(u32 low, u32 high);
/**
 * Controls profiling.
 *
 * mode = 0 stops profiling, mode != 0 starts it
 */
IWRAM_CODE void moncontrol(int mode);
/**
 * Clean up and stop profiling. gmon.out will be written
 */
IWRAM_CODE void moncleanup();
/**
 * Logs a message message to VBA console (or GDB console if using GDB).
 */
IWRAM_CODE void vbalog(const char *msg);
IWRAM_CODE void mappylog(const char *msg);


#ifdef __cplusplus
}
#endif
#endif
