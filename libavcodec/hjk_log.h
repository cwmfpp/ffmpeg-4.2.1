#ifndef HJK_LOG_H
#define HJK_LOG_H

#if 1

#ifdef MAC_OS
#include <pthread.h>
#define gettid_a ({uint64_t tid;pthread_threadid_np(0, &tid);(int)tid;})
#else
#define gettid_a ({(int)syscall(__NR_gettid);})
#endif
//#define gettid_a (10)
    
#define hjk_error(fmt, x...)  printf("%s:%d:%d: " fmt "\n", __FUNCTION__, __LINE__, gettid_a, ##x);
#define hjk_warn(fmt, x...)  printf("%s:%d:%d: " fmt "\n", __FUNCTION__, __LINE__, gettid_a, ##x);
#define hjk_trace(fmt, x...)  printf("%s:%d:%d: " fmt "\n", __FUNCTION__, __LINE__, gettid_a, ##x);
#define hjk_debug(fmt, x...)  printf("%s:%d:%d: " fmt "\n", __FUNCTION__, __LINE__, gettid_a, ##x);
#else
#define hjk_error(fmt, x...)
#define hjk_warn(fmt, x...)
#define hjk_trace(fmt, x...)
#define hjk_debug(fmt, x...)
#endif

#endif /* HJK_LOG_H */
