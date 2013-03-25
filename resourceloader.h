
#ifndef __ISTL_ADDONS_RESOURCEMANAGER_RESOURCELOADER_H__
#define __ISTL_ADDONS_RESOURCEMANAGER_RESOURCELOADER_H__

#include "eastl/types.h"

class ResourceBase;
class ResourceLoader {
public:
   virtual const char *getLoadableExtension() = 0;    //can include wildcards! (? and *)
   virtual ResourceBase *createResourceDescr(eastl::string filename) = 0;
   virtual bool loadResource(eastl::string filename, ResourceBase *resource) = 0;
   virtual void freeResource(ResourceBase *resource) = 0;
};

#endif
