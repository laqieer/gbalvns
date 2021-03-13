#include <string.h>
#include "mgba.h"
#include "common.h"

IWRAM_CODE void mgbalog(const char* msg, const u32 level) {
    REG_LOG_ENABLE = 0xC0DE;
    const u32 max_chars_per_line = 256; //not arbitrary. the register is just 256 chars wide
    u32 chars_left = strlen(msg);

    while(chars_left) { //breaks the message into 256-char log entries
        u32 chars_to_write = _Min(chars_left, max_chars_per_line);

        memcpy(REG_LOG_STR, msg, chars_to_write);
        REG_LOG_LEVEL = level; //every time this is written to, mgba creates a new log entry

        msg += chars_to_write; //increments the POINTER to the string, took me forever to realize
        chars_left -= chars_to_write;
    }
}
