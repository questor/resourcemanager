
#include "resourcemanager.h"

#include "eastl/extra/hashfuncs.h"
#include "eastl/extra/debug.h"
#include "eastl/extra/stringutils.h"
#include "eastl/algorithm.h"

#include "eaio/FnEncode.h"
#include "eaio/FileStream.h"

ResourceManager *ResourceManager::mInstance = NULL;

//----------------------------------------------------------------------------------
// ResourceManager
//----------------------------------------------------------------------------------
// 
ResourceManager::ResourceManager()
   : mAvailableMemoryBudget(256*1024*1024)      //256MB Graphic Memory
   , mUsedMemory(0)
   , mVirtualInitTime(0)
{
#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
   int retval = amp_mutex_create(&mFilemonitorMutex, AMP_DEFAULT_ALLOCATOR);

   mFileChangeNotification.init();
   mFileChangeNotification.setWatchedDirectory(L"media/");
   mFileChangeNotification.setChangeTypeFlags(eaio::FileChangeNotification::kChangeTypeFlagModified);
   mFileChangeNotification.setOptionFlags(eaio::FileChangeNotification::kOptionFlagWatchSubdirectories);
   mFileChangeNotification.setNotificationCallback(&fileChangeCallback, this);
   ASSERT(mFileChangeNotification.start());
#endif
}

//----------------------------------------------------------------------------------
// ~ResourceManager
//----------------------------------------------------------------------------------
// 
ResourceManager::~ResourceManager() {

#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
   mFileChangeNotification.shutdown();

   amp_mutex_destroy(&mFilemonitorMutex, AMP_DEFAULT_ALLOCATOR);
#endif

   eastl::map<uint_t, ResourceItem>::iterator resIter = mResources.begin();
   while(resIter != mResources.end()) {
      ASSERT((*resIter).second.registerCounter == 0);    //make sure everything is unloaded
      (*resIter).second.resource->unload();
      delete (*resIter).second.resource;
      resIter++;
   }

   auto loaderIter = mLoaders.begin();
   while(loaderIter != mLoaders.end()) {
      delete *loaderIter;           //delete loader
      loaderIter++;
   }
}

//----------------------------------------------------------------------------------
// registerLoader
//----------------------------------------------------------------------------------
// 
void ResourceManager::registerLoader(ResourceLoader *newLoader) {
   mLoaders.pushBack(newLoader);
}

//----------------------------------------------------------------------------------
// registerResource
//----------------------------------------------------------------------------------
//
ResourceHandle ResourceManager::registerResource(eastl::string filename, bool instantLoad) {
   uint_t handle = eastl::getHashForceLower(filename.data());

   eastl::map<uint_t, ResourceItem>::iterator iter;
   iter = mResources.find(handle);

   if(iter != mResources.end()) {
      iter->second.registerCounter++;
      return handle;
   }

   //find resource-loader for this type
   int pos = filename.findLastOf('.');

   eastl::string extension;
   extension = filename.substr(pos+1);
   
   //search for loader
   auto loaderIter = mLoaders.begin();
   auto iterEnd = mLoaders.end();
   while((loaderIter != iterEnd) && (eastl::wildstrcmp((*loaderIter)->getLoadableExtension(), extension.c_str()) == 0)) {
      ++loaderIter;
   }
   ASSERT(loaderIter != iterEnd);

   ResourceItem item;
   item.registerCounter = 1;
   item.resource = (*loaderIter)->createResourceDescr(filename);
   item.registerTime = mVirtualInitTime++;
#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
   item.pathAndFilenameHash = eastl::murmurString(filename.c_str());
#endif
   mResources.insert(eastl::pair<uint_t, ResourceItem>(handle, item));

   if(instantLoad) {
      mResources[handle].resource->load();
      mUsedMemory += mResources[handle].resource->getSize();
      checkForResourcesToRemove();
   }

   return handle;
}

//----------------------------------------------------------------------------------
// prepareResource
//----------------------------------------------------------------------------------
// 
void ResourceManager::preloadResource(ResourceHandle handle) {
   //TODO: in extra thread?
   eastl::map<uint_t, ResourceItem>::iterator iter;
   iter = mResources.find(handle);
   ASSERT(iter != mResources.end());
   iter->second.resource->load();
   mUsedMemory += mResources[handle].resource->getSize();
   checkForResourcesToRemove();
}

//----------------------------------------------------------------------------------
// releaseResource
//----------------------------------------------------------------------------------
// 
void ResourceManager::releaseResource(ResourceHandle handle) {
   eastl::map<uint_t, ResourceItem>::iterator iter;
   iter = mResources.find(handle);
   ASSERT(iter != mResources.end());
   iter->second.registerCounter--;
   checkForResourcesToRemove();
}

//----------------------------------------------------------------------------------
// checkForResourcesToRemove
//----------------------------------------------------------------------------------
//
void ResourceManager::checkForResourcesToRemove() {
   while(mUsedMemory > mAvailableMemoryBudget) {
      //search for oldest, not used item
      auto toRemove = mResources.end();
      uint_t oldestFoundTime = mVirtualInitTime;
      for(auto resIter=mResources.begin(), end=mResources.end(); resIter != end; ++resIter) {
         if((*resIter).second.registerCounter == 0) {
            if((*resIter).second.registerTime < oldestFoundTime) {
               oldestFoundTime = (*resIter).second.registerTime;
               toRemove = resIter;
            }
         }
      }
      if(toRemove == mResources.end()) {
         eastl::printDebugString("resources are over budget and nothing can be removed!\nmemory budget %dMB   used memory %dMB", mAvailableMemoryBudget/1024/1024, mUsedMemory/1024/1024);
         return;
      }
      mUsedMemory -= (*toRemove).second.resource->getSize();
      (*toRemove).second.resource->unload();
      mResources.erase(toRemove);
   }
}

//----------------------------------------------------------------------------------
// fileChangeCallback
//----------------------------------------------------------------------------------
//
#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
void ResourceManager::fileChangeCallback(eaio::FileChangeNotification* pFCN, const char16_t* pDirectoryPath,
                                         const char16_t* pFileName, int nChangeTypeFlags, void* pContext) {
   //mark as reloadable
   eastl::string dir, file;
   char buffer[1024];
   eaio::strlcpyUTF16ToUTF8(buffer, sizeof(buffer), pDirectoryPath, sizeof(buffer));
   dir.assign(buffer);
   eaio::strlcpyUTF16ToUTF8(buffer, sizeof(buffer), pFileName, sizeof(buffer));
   file.assign(buffer);

   eastl::string completePath(dir+file);
   eastl::replace_all(completePath, "\\", "/");
   uint32_t hash = eastl::murmurString(completePath.c_str());

   ResourceManager *rm = (ResourceManager*)pContext;
   amp_mutex_lock(rm->mFilemonitorMutex);
   //duplicate entries are automatically removed :)
   rm->mModifiedFiles.insert(eastl::pair<uint32_t, eastl::string>(hash, completePath));
   amp_mutex_unlock(rm->mFilemonitorMutex);

   printf("notification: %s\n", completePath.c_str());
   //<media\templates\>    <dragon.sjson>
}
#endif

//----------------------------------------------------------------------------------
// pollForFileChanges
//----------------------------------------------------------------------------------
//
void ResourceManager::pollForFileChanges() {
#ifdef RESOURCEMANAGER_FILE_MONITOR_ENABLED
   static int slowdownCounter = 0;
   slowdownCounter++;
   if(slowdownCounter == 64) {
      mFileChangeNotification.poll(30);
      slowdownCounter = 0;

      //now scan list for changed files
      amp_mutex_lock(mFilemonitorMutex);

      bool reloadedSomething = false;
      for(auto iter=mModifiedFiles.begin(); iter!=mModifiedFiles.end(); /*empty*/) {
         bool keep = false;

         auto resIter = mResources.begin();
         while((resIter != mResources.end()) &&
               ((*resIter).second.pathAndFilenameHash != (*iter).first))
            resIter++;
         if(resIter != mResources.end()) {
            if((*resIter).second.resource->isLocked()) {
               //printf("found assert <%s> to reload, but is locked\n", (*iter).second.c_str());

               keep = true;
            } else {
               eaio::FileStream testAccess((*iter).second.c_str());
               bool success = testAccess.open();
               if(success) {
                  testAccess.close();

                  //printf("reloading asset <%s>\n", (*iter).second.c_str());

                  ResourceBase *res = (*resIter).second.resource;

                  mUsedMemory -= res->getSize();
                  res->unload();

                  //let resourcehandle item untouched

                  res->load();                     //load immediatly again
                  mUsedMemory += res->getSize();

                  reloadedSomething = true;
               } else {
                  //printf("found assert <%s> to reload, but file is not yet accessible\n", (*iter).second.c_str());

                  keep = true;
               }
            }
         }

         if(keep == false) {
            mModifiedFiles.erase(iter++);
         } else {
            ++iter;
         }
      }
      amp_mutex_unlock(mFilemonitorMutex);

      if(reloadedSomething)
         checkForResourcesToRemove();     //neccessary?
   }
#endif
}

//----------------------------------------------------------------------------------
// printWebDebug
//----------------------------------------------------------------------------------
//
void ResourceManager::printWebDebug(eastl::string &result) {
   result.appendSprintf("Used Memory: %.2fMB(%d Bytes)<br>", mUsedMemory/1024.0f/1024.0f, mUsedMemory);
   result.appendSprintf("Available Budget: %.2fMB(%d Bytes)<br>", mAvailableMemoryBudget/1024.0f/1024.0f, mAvailableMemoryBudget);

   result.appendSprintf("Number Resources: %d<br>", mResources.size());

   result.append("<div class=\"jsonreport\"></div>");

   result.append("<script type=\"text/javascript\">\n");
   result.append("var data = \'{");
   bool firstEntry = true;
   for(auto iter = mResources.begin(); iter != mResources.end(); ++iter) {
      ResourceBase *res = (*iter).second.resource;
      if(firstEntry == false) {
         result.append(",");
      }
      firstEntry = false;

      result.appendSprintf("\"%s\" : {", res->getFilename());

      result.appendSprintf("\"refCount\" : %d,", res->getRefCount());
      result.appendSprintf("\"registerCount\" : %d", (*iter).second.registerCounter);

      result.append("}");
   }
   result.append("}\';\n");
   result.append("document.getElementsByClassName(\'jsonreport\')[0].innerHTML=_.jsonreport(data);\n");
   result.append("</script>");
}
