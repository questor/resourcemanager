
#ifdef RESOURCEMANAGER_INCLUDE_STBIMAGELOADERS

#ifndef __RESOURCEMANAGER_LOADERS_STBIMAGELOADERS_H__
#define __RESOURCEMANAGER_LOADERS_STBIMAGELOADERS_H__

#include "eastl/types.h"
#include "stb_image/stb_image.h"
#include "resourcemanager/resourcetypes/imageresource.h"

#ifdef STBI_PNG_SUPPORT
class PngResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "png"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif
#ifdef STBI_JPG_SUPPORT
class JpgResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "jpg"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif
#ifdef STBI_BMP_SUPPORT
class BmpResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "bmp"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif
#ifdef STBI_TGA_SUPPORT
class TgaResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "tga"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif
#ifdef STBI_PSD_SUPPORT
class PsdResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "psd"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif
#ifdef STBI_HDR_SUPPORT
class HdrResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "hdr"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif
#ifdef STBI_PIC_SUPPORT
class PicResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "pic"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif
#ifdef STBI_GIF_SUPPORT
class GifResourceLoader : public ResourceLoader {
public:
   const char *getLoadableExtension() { return "gif"; }
   ResourceBase *createResourceDescr(eastl::string filename) {
      return new ImageResource(filename, this);
   }
   bool loadResource(eastl::string filename, ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->mData = (uint32_t*)stbi_load(filename.data(), &png->mWidth, &png->mHeight, &png->mComponents, 4);
      ASSERT(png->isLoaded());
      return png->isLoaded();
   }
   void freeResource(ResourceBase *resource) {
      ImageResource *png = (ImageResource*)resource;
      png->freeImage();
   }
};
#endif


#endif   //#ifndef __RESOURCEMANAGER_LOADERS_STBIMAGELOADERS_H__

#endif   //#ifdef RESOURCEMANAGER_INCLUDE_STBIMAGELOADERS

