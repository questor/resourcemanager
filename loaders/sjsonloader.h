
#ifdef RESOURCEMANAGER_INCLUDE_SJSONLOADER

#ifndef __RESOURCEMANAGER_LOADERS_SJSONLOADERS_H__
#define __RESOURCEMANAGER_LOADERS_SJSONLOADERS_H__

#include "eastl/types.h"
#include "resourcemanager/resourcetypes/sjsonresource.h"
#include "sjson/sjson.h"

#include "eaio/FileStream.h"

class SJSONResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "sjson"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new SJSONResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      SJSONResource *sjson = (SJSONResource*)resource;

      eaio::FileStream fstream(filename.c_str());
      bool success = fstream.open();
      ASSERT(success);
      int len = fstream.getSize();
      char *memory = new char[len+1];
      fstream.read(memory, len);
      memory[len] = 0;
      fstream.close();

      sjson->mRootnode = sJSONparse(memory);

      delete[] memory;

      sjson->mSize = len;     //roughly the filesize

      ASSERT(sjson->isLoaded());
      return sjson->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      SJSONResource *sjson = (SJSONResource*)resource;
      sjson->freeDocument();
   }
};

#endif   //#ifndef __RESOURCEMANAGER_LOADERS_SJSONLOADERS_H__

#endif   //#ifdef RESOURCEMANAGER_INCLUDE_SJSONLOADER

