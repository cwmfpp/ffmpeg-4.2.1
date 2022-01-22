#ifndef HJK_LOG_H
#define HJK_LOG_H

#if 1
#define hjk_error(fmt, x...)  printf("%s:%s:%d: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##x);
#define hjk_warn(fmt, x...)   printf("%s:%s:%d: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##x);
#define hjk_trace(fmt, x...)  printf("%s:%s:%d: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##x);
#define hjk_debug(fmt, x...)  printf("%s:%s:%d: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##x);
#else
#define hjk_error(fmt, x...)
#define hjk_warn(fmt, x...)
#define hjk_trace(fmt, x...)
#define hjk_debug(fmt, x...)
#endif

#endif /* HJK_LOG_H */
