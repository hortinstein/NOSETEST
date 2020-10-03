#ifndef DEBUG_H
#define DEBUG_H


//!COMMENT reminder to talk about the importance of MACROs for testing and good tunable 
//!COMMENT feedback...i mean look at all the debug garbage it's spitting out right now
#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt "\n", \
    __FILE__, __LINE__, __func__, ##args)
#else
#define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

#ifdef DEBUG
#define DEBUG_ERROR(fmt, args...) fprintf(stderr, "ERROR: %s:%d:%s(): " fmt "\n", \
    __FILE__, __LINE__, __func__, ##args)
#else
#define DEBUG_ERROR(fmt, args...) /* Don't do anything in release builds */
#endif


//!COMMENT here are all your debug variables 
#ifdef DEBUG
#define PP_URL "http://localhost:8000"
#else
#define PP_URL "" /* Don't do anything in release builds */
#endif

#ifdef DEBUG
#define PP_KEY_URL "http://localhost:8000/key"
#else
#define PP_KEY_URL "" /* Don't do anything in release builds */
#endif

#ifdef DEBUG
#define PP_TASK_URL "http://localhost:8000/task"
#else
#define PP_TASK_URL "" /* Don't do anything in release builds */
#endif

#endif