
#ifdef RESOURCEMANAGER_INCLUDE_TEXTFILELOADER

#ifndef __RESOURCEMANAGER_LOADERS_TEXTFILELOADERS_H__
#define __RESOURCEMANAGER_LOADERS_TEXTFILELOADERS_H__

#include "eastl/types.h"
#include "resourcemanager/resourcetypes/textfileresource.h"

#include "eaio/FileStream.h"

class TextFileResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "txt"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new TextFileResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      TextFileResource *textfile = (TextFileResource*)resource;

      eaio::FileStream fstream(filename.c_str());
      fstream.open();
      int len = fstream.getSize();
      ASSERT(len > 0);
      uint8_t *memory = new uint8_t[len+1];
      fstream.read(memory, len);
      fstream.close();

      memory[len] = 0;

      textfile->mData = memory;
      textfile->mSize = len+1;

      ASSERT(textfile->isLoaded());
      return textfile->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      TextFileResource *textfile = (TextFileResource*)resource;
      textfile->freeData();
   }
};

#endif   //#ifndef __RESOURCEMANAGER_LOADERS_TEXTFILELOADERS_H__

#endif   //#ifdef RESOURCEMANAGER_INCLUDE_TEXTFILELOADER

