#ifndef CBOY_LOG_H
#define CBOY_LOG_H

#include <stdio.h>
#include <stdlib.h>

#define VERBOSITY_ALL 0
#define VERBOSITY_DEBUG 1
#define VERBOSITY_WARN 2
#define VERBOSITY_ERROR 3

#define VERBOSITY VERBOSITY_ALL

#if VERBOSITY <= VERBOSITY_ALL
#define log(message, ...) { \
    fprintf(stdout, "\033[0;36m");  \
    fprintf(stdout, message "\n",  ##__VA_ARGS__); \
    fprintf(stdout, "\033[0m");     \
}
#else
#define log(message, ...) { }
#endif

#if VERBOSITY <= VERBOSITY_DEBUG
#define log_debug(message, ...) { \
    fprintf(stdout, "[DEBUG]:") \
    fprintf(stdout, message "\n", ##__VA_ARGS__); \
endif \
}
#else
#define log_debug(message, ...) {}
#endif

#if VERBOSITY <= VERBOSITY_WARN
#define log_warn(message, ...) { \
    fprintf(stdout, "\033[0;33m");  \
    fprintf(stdout, "[WARN]:");  \
    fprintf(stdout, message "\n", ##__VA_ARGS__); \
    fprintf(stdout, "\033[0m");     \
}
#else
#define log_warn(message, ...) { }
#endif


#if VERBOSITY < VERBOSITY_ERROR
#define log_fatal(message, ...) { \
    fprintf(stderr, "\033[1;31m");   \
    fprintf(stderr, "[FATAL] at %s:%d: ", __FILE__, __LINE__);   \
    fprintf(stderr, message "\n", ##__VA_ARGS__); \
    fprintf(stderr, "\033[0m");      \
    exit(1); \
}
#else
#define log_fatal(message, ...) { }
#endif



#endif //CBOY_LOG_H
