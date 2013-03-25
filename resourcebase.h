
#ifndef __ISTL_ADDONS_RESOURCEMANAGER_RESOURCEBASE_H__
#define __ISTL_ADDONS_RESOURCEMANAGER_RESOURCEBASE_H__

#include "eastl/types.h"
#include "resourceloader.h"

#include "eastl/string.h"

class ResourceBase {
public:
   ResourceBase(eastl::string filename, ResourceLoader *loader)
      : mNumberRefCount(0)
      , mLoader(loader)
      , mFilename(filename)
   {
   }
   virtual ~ResourceBase()	{  
      unload();
   }

   bool load() {
      return mLoader->loadResource(mFilename, this);
   }
   void unload() {
      mLoader->freeResource(this);
   }

   virtual size_t getSize() = 0;
   virtual bool isLoaded() = 0;

   void lock()     { mNumberRefCount++; }
   void unlock()   { mNumberRefCount--; }
   bool isLocked() { return (mNumberRefCount > 0) ? true : false;  }

   const eastl::string &getFilename() { return mFilename; }
   const uint_t getRefCount() { return mNumberRefCount; }
protected:
   uint_t mNumberRefCount;
   ResourceLoader *mLoader;
   eastl::string mFilename;
};

#endif
