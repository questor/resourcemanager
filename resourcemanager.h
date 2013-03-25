
#ifndef __ISTL_ADDONS_RESOURCEMANAGER_RESOURCEMANAGER_H__
#define __ISTL_ADDONS_RESOURCEMANAGER_RESOURCEMANAGER_H__

#include "eastl/string.h"
#include "eastl/map.h"
#include "eastl/vector.h"

#include "amp/amp_mutex.h"

#include "resourceloader.h"
#include "resourcebase.h"

#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
   #include "eaio/FileNotification.h"
#endif

typedef uint32_t ResourceHandle;

class ResourceManager {
public:
   ResourceManager();
   ~ResourceManager();

   void setMaximumMemory(size_t memory) { mAvailableMemoryBudget = memory; }
   size_t getUsedMemory() { return mUsedMemory; }

   void registerLoader(ResourceLoader *newLoader);

   ResourceHandle registerResource(eastl::string filename, bool instantLoad = true);
   void preloadResource(ResourceHandle handle);
   void releaseResource(ResourceHandle handle);

   /** automatically calls lock on the resource if it's already loaded*/
   ResourceBase *getResource(ResourceHandle handle) {
      ResourceBase *resource = mResources[handle].resource;
      if(resource->isLoaded())
         resource->lock();
      return resource;
   }

   void unlock(ResourceHandle handle) {
      ResourceBase *resource = mResources[handle].resource;
      resource->unlock();
   }

   static ResourceManager* createInstance() {
      ASSERT(mInstance == NULL);
      mInstance = new ResourceManager();
      return mInstance;
   }
   static void shutdown() {
      ASSERT(mInstance != NULL);
      delete mInstance;
      mInstance = NULL;
   }
   static ResourceManager *getInstance() {
      return mInstance;
   }

   void printWebDebug(eastl::string &result);

   void pollForFileChanges();
protected:
   void checkForResourcesToRemove();
   static ResourceManager *mInstance;

#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
   eaio::FileChangeNotification mFileChangeNotification;
   static void fileChangeCallback(eaio::FileChangeNotification* pFCN, const char16_t* pDirectoryPath,
                                  const char16_t* pFileName, int nChangeTypeFlags, void* pContext);
#endif

   typedef struct {
      uint_t registerCounter;
      uint32_t registerTime;
      ResourceBase *resource;
#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
      uint32_t pathAndFilenameHash;
#endif
   } ResourceItem;

#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
   amp_mutex_t mFilemonitorMutex;
   eastl::map<uint32_t, eastl::string> mModifiedFiles;
#endif

   eastl::map<uint32_t, ResourceItem> mResources;
   eastl::vector<ResourceLoader*> mLoaders;

   uint32_t mVirtualInitTime;

   size_t mAvailableMemoryBudget;
   size_t mUsedMemory;
};

#endif
