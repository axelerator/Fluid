#ifndef rgbabmp__
#define rgbabmp__

#include <string>

typedef struct {
        char* data;
        int width;
        int height;
        int bpp;
        std::string filename;  
} RGBAbmp;
RGBAbmp *loadBmp(std::string filename) ;
#endif
