
#ifndef __RESOURCEMANAGER_RESOURCETYPES_RAWFILERESOURCE_H__
#define __RESOURCEMANAGER_RESOURCETYPES_RAWFILERESOURCE_H__

#include "eastl/types.h"
#include "resourcemanager/resourceloader.h"
#include "resourcemanager/resourcebase.h"

class RawFileResourceLoader;
class RawFileResource : public ResourceBase {
   friend class RawFileResourceLoader;
public:
   RawFileResource(eastl::string filename, ResourceLoader *loader)
      : ResourceBase(filename, loader)
      , mSize(0)
      , mData(nullptr)
   { }
   ~RawFileResource() {
   }

   size_t getSize() {
      return mSize;
   }
   bool isLoaded() {
      return mData != nullptr;
   }

   void freeData() {
      delete[] mData;
      mData = nullptr;
   }

   uint8_t *getData() {
      return mData;
   }
protected:
   uint8_t *mData;
   size_t mSize;
};

#endif   //#ifdef __RESOURCEMANAGER_RESOURCETYPES_RAWFILERESOURCE_H__
