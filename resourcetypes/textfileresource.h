
#ifndef __RESOURCEMANAGER_RESOURCETYPES_TEXTFILERESOURCE_H__
#define __RESOURCEMANAGER_RESOURCETYPES_TEXTFILERESOURCE_H__

#include "eastl/types.h"
#include "resourcemanager/resourceloader.h"
#include "resourcemanager/resourcebase.h"

//Raw and Text differ by text having a following zero byte
class TextFileResourceLoader;
class TextFileResource : public ResourceBase {
   friend class TextFileResourceLoader;
public:
   TextFileResource(eastl::string filename, ResourceLoader *loader)
      : ResourceBase(filename, loader)
      , mSize(0)
      , mData(nullptr)
   { }
   ~TextFileResource() {
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

   char *getData() {
      return (char*)mData;
   }
protected:
   uint8_t *mData;
   size_t mSize;
};

#endif   //#ifdef __RESOURCEMANAGER_RESOURCETYPES_TEXTFILERESOURCE_H__
