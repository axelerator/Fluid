#include "RGBAbmpLoader.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string> 

using namespace std;
 
RGBAbmp *loadBmp(string filename) {
  RGBAbmp *result    = new RGBAbmp;
  std::ifstream input(filename.c_str(), std::ios::in | std::ios::binary);
   if (!input.is_open()) {cout << "Error: Failed to load texture" << filename << endl;exit(1);}
   int start;
   result->bpp = 0;
   char buffer[1024];
   input.read(buffer,10);
   input.read((char*)&start,4);

   input.read(buffer,4);
   input.read((char*)&(result->width), 4);
   input.read((char*)&(result->height), 4);
   input.read((char*)&(buffer),2);
   input.read((char*)&(result->bpp),2);
   input.read(buffer,start-20);
   result->data = new char[result->width*result->height*result->bpp];
   input.read(result->data,result->width*result->height*result->bpp);
   input.close();
   
   unsigned char temp;
   int channels = result->bpp/8;
   int currpos;
   int stride = (result->width*channels);
   for (int r = 0; r < result->height; ++r)
       for (int c = 0; c < result->width;++c) {
                currpos = stride*r+c*channels;
                temp = result->data[currpos+1];
                result->data[currpos+1] =  result->data[currpos+3];
                result->data[currpos+3] =  temp;
           }
return result;
}
