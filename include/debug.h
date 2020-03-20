#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ##args)
#else
#define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

#ifdef DEBUG
#define DEBUG_ERROR(fmt, args...) fprintf(stderr, fmt, ##args)
#else
#define DEBUG_ERROR(fmt, args...) /* Don't do anything in release builds */
#endif

#ifdef DEBUG
#define PP_URL "http://localhost:8000"
#else
#define PP_URL "" /* Don't do anything in release builds */
#endif

#endif