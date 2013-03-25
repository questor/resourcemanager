
#ifndef __RESOURCEMANAGER_RESOURCETYPES_SJSONRESOURCE_H__
#define __RESOURCEMANAGER_RESOURCETYPES_SJSONRESOURCE_H__

#include "eastl/types.h"
#include "resourcemanager/resourceloader.h"
#include "resourcemanager/resourcebase.h"
#include "sjson/sjson.h"

class SJSONResourceLoader;
class SJSONResource : public ResourceBase {
   friend class SJSONResourceLoader;
public:
   SJSONResource(eastl::string filename, ResourceLoader *loader)
      : ResourceBase(filename, loader)
      , mRootnode(nullptr)
      , mSize(0)
   { }
   ~SJSONResource() {
   }

   size_t getSize() {
      return mSize;
   }
   bool isLoaded() {
      return mRootnode != nullptr;
   }

   void freeDocument() {
      if(mRootnode) {
         sJSONdelete(mRootnode);
         mRootnode = nullptr;
      }
   }

   sJSON *getData() {
      return mRootnode;
   }
protected:
   size_t mSize;
   sJSON *mRootnode;
};

#endif   //#ifndef __RESOURCEMANAGER_RESOURCETYPES_SJSONRESOURCE_H__
