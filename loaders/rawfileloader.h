
#ifdef RESOURCEMANAGER_INCLUDE_RAWFILELOADER

#ifndef __RESOURCEMANAGER_LOADERS_RAWFILELOADERS_H__
#define __RESOURCEMANAGER_LOADERS_RAWFILELOADERS_H__

#include "eastl/types.h"
#include "resourcemanager/resourcetypes/rawfileresource.h"

#include "eaio/FileStream.h"

class RawFileResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "*"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new RawFileResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      RawFileResource *rawfile = (RawFileResource*)resource;

      eaio::FileStream fstream(filename.c_str());
      fstream.open();
      int len = fstream.getSize();
      uint8_t *memory = new uint8_t[len];
      fstream.read(memory, len);
      fstream.close();

      rawfile->mData = memory;
      rawfile->mSize = len;

      ASSERT(rawfile->isLoaded());
      return rawfile->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      RawFileResource *rawfile = (RawFileResource*)resource;
      rawfile->freeData();
   }
};

#endif   //#ifndef __RESOURCEMANAGER_LOADERS_RAWFILELOADERS_H__

#endif   //#ifdef RESOURCEMANAGER_INCLUDE_RAWFILELOADER

