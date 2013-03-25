
//#define RESOURCEMANAGER_INCLUDE_GAUSSIMAGEPROCESSOR
#ifdef RESOURCEMANAGER_INCLUDE_GAUSSIMAGEPROCESSOR

#ifndef __RESOURCEMANAGER_PROCESSORS_GAUSSIMAGEPROCESSOR_H__
#define __RESOURCEMANAGER_PROCESSORS_GAUSSIMAGEPROCESSOR_H__

// filename.png.kernel=4 blub=6.gauss

#include "eastl/string.h"

//hhhhmmmm
#include "misc/gaussfilter.h"

#include "resourcemanager/resourcemanager.h"
#include "resourcemanager/resourceloader.h"

class GaussImageLoader : public ResourceLoader {
public:
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }

   const char *getLoadableExtension() { return "gauss"; }

   bool loadResource(eastl::string filename, ResourceBase *resource) {
      eastl::string pureFilename = filename.substr(0, filename.findLastOf('.'));
      size_t parameterStart = pureFilename.findLastOf('.');
      eastl::string parameter = pureFilename.substr(parameterStart+1);
      pureFilename = pureFilename.substr(0, parameterStart);
      ResourceManager *manager = ResourceManager::getInstance();
      ResourceHandle handle = manager->registerResource(pureFilename, true);        //load instantly
      ImageResource *img = (ImageResource*)manager->getResource(handle);

      if(!img->isLoaded())
         return false;

      ImageResource *gauss = (ImageResource*)resource;
      gauss->mWidth = img->mWidth;
      gauss->mHeight = img->mHeight;
      gauss->mComponents = img->mComponents;
      gauss->mData = new uint32_t[gauss->mWidth*gauss->mHeight];

      memcpy(gauss->mData, img->mData, gauss->mWidth*gauss->mHeight*sizeof(uint32_t));
      manager->unlock(handle);
      manager->releaseResource(handle);

      size_t kernel=parameter.findFirstOf("kernel=");

      GaussFilter filter(12);
      uint32_t *tmpData = new uint32_t[gauss->mWidth*gauss->mHeight];
      filter.applyFilter(gauss->mData, tmpData, gauss->mWidth, gauss->mHeight);
      delete[] tmpData;

      return gauss->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *gauss = (ImageResource*)resource;
      gauss->freeImage();
   }
};

#endif   //#ifndef __RESOURCEMANAGER_GAUSSIMAGEPROCESSOR_H__

#endif   //#ifdef RESOURCEMANAGER_INCLUDE_GAUSSIMAGEPROCESSOR
