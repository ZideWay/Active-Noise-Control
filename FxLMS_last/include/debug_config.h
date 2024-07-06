#ifndef __DEBUG_CONFIG_H
#define __DEBUG_CONFIG_H

#define __DEBUG__ 0  
#ifdef __DEBUG__  
#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif

#endif