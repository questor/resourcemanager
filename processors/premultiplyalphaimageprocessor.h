
//#define RESOURCEMANAGER_INCLUDE_PREMULTIPLYALPHAIMAGEPROCESSOR
#ifdef RESOURCEMANAGER_INCLUDE_PREMULTIPLYALPHAIMAGEPROCESSOR

#ifndef __RESOURCEMANAGER_PROCESSORS_PREMULTIPLYALPHAIMAGEPROCESSOR_H__
#define __RESOURCEMANAGER_PROCESSORS_PREMULTIPLYALPHAIMAGEPROCESSOR_H__

// filename.png.premultiplyalpha

#include "eastl/string.h"

#include "resourcemanager.h"
#include "resourceloader.h"

class PremultiplyAlphaImageLoader : public ResourceLoader {
public:
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }

   const char *getLoadableExtension() { return "premultiplyalpha"; }

   bool loadResource(eastl::string filename, ResourceBase *resource) {

      ASSERT(0);

      //is this correct?
      eastl::string pureFilename = filename.substr(0, filename.findLastOf('.'));
      size_t parameterStart = pureFilename.findLastOf('.');
      eastl::string parameter = pureFilename.substr(parameterStart+1);
      pureFilename = pureFilename.substr(0, parameterStart);
      ResourceManager *manager = ResourceManager::instance();
      ResourceHandle handle = manager->registerResource(pureFilename, true);        //load instantly
      ImageResource *img = (ImageResource*)manager->getResource(handle);

      if(!img->isLoaded())
         return false;

      ImageResource *premulAlpha = (ImageResource*)resource;
      premulAlpha->mWidth = img->mWidth;
      premulAlpha->mHeight = img->mHeight;
      premulAlpha->mComponents = img->mComponents;
      premulAlpha->mData = new uint32_t[gauss->mWidth*gauss->mHeight];

      memcpy(premulAlpha->mData, img->mData, premulAlpha->mWidth*premulAlpha->mHeight*sizeof(uint32_t));
      manager->unlock(handle);
      manager->releaseResource(handle);

      //premultiply alpha
      uint32_t *ptr = (uint32_t*)premulAlpha->mData;
      for(uint_t i=0; i<premulAlpha->mHeight*premulAlpha->mWidth; i++) {
         uint32_t pixel = *ptr;
         uint32_t a = (pixel & 0xff000000) >> 24;
         uint32_t r = (pixel & 0x00ff0000) >> 16;
         uint32_t g = (pixel & 0x0000ff00) >> 8;
         uint32_t b = (pixel & 0x000000ff);
         r = r*a/255;
         g = g*a/255;
         b = b*a/255;
         *(ptr++) = (a<<24) | (r<<16) | (g<<8) | b;
      }
      return premulAlpha->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *premulAlpha = (ImageResource*)resource;
      premulAlpha->freeImage();
   }
};

#endif   //#ifndef __RESOURCEMANAGER_PROCESSORS_PREMULTIPLYALPHAIMAGEPROCESSOR_H__

#endif   //#ifdef RESOURCEMANAGER_INCLUDE_PREMULTIPLYALPHAIMAGEPROCESSOR
