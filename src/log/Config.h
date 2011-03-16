#ifndef SQUID_SRC_LOG_CONFIG_H
#define SQUID_SRC_LOG_CONFIG_H

#include "log/Tokens.h"

class StoreEntry;

namespace Log
{

class LogConfig
{
public:
    void parseFormats();
    void dumpFormats(StoreEntry *e, const char *name) {
        accessLogDumpLogFormat(e, name, logformats);
    }

    /// File path to logging daemon executable
    char *logfile_daemon;

    /// Linked list of custom log formats
    logformat *logformats;
};

extern LogConfig TheConfig;

} // namespace Log

// Legacy parsing wrappers
#define parse_logformat(X)  (X)->parseFormats()
#define free_logformat(X)   do{ delete (*X).logformats; (*X).logformats=NULL; }while(false)
#define dump_logformat(E,N,D) (D).dumpFormats((E),(N))

#endif