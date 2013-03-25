
#ifndef __RESOURCEMANAGER_RESOURCETYPES_IMAGERESOURCE_H__
#define __RESOURCEMANAGER_RESOURCETYPES_IMAGERESOURCE_H__

#include "eastl/types.h"
#include "resourcemanager/resourceloader.h"
#include "resourcemanager/resourcebase.h"

class GaussImageLoader;
class PngResourceLoader;
class JpgResourceLoader;
class BmpResourceLoader;
class TgaResourceLoader;
class PsdResourceLoader;
class HdrResourceLoader;
class PicResourceLoader;
class GifResourceLoader;
class ImageResource : public ResourceBase {
   friend class GaussImageLoader;
   friend class PngResourceLoader;
   friend class JpgResourceLoader;
   friend class BmpResourceLoader;
   friend class TgaResourceLoader;
   friend class PsdResourceLoader;
   friend class HdrResourceLoader;
   friend class PicResourceLoader;
   friend class GifResourceLoader;
public:
   ImageResource(eastl::string filename, ResourceLoader *loader)
      : ResourceBase(filename, loader)
      , mWidth(0)
      , mHeight(0)
      , mComponents(0)
      , mData(nullptr)
   { }
   ~ImageResource() {
   }

   size_t getSize() {
      return mWidth*mHeight*mComponents;
   }
   bool isLoaded() {
      return mData != nullptr;
   }
   void freeImage() {
      delete[] mData;
      mData = nullptr;
   }

   uint32_t *getData() {
      return mData;
   }
   int getWidth() {
      return mWidth;
   }
   int getHeight() {
      return mHeight;
   }
   int getComponents() {
      return mComponents;
   }
protected:
   uint32_t *mData;
   int mWidth, mHeight, mComponents;
};

#endif
