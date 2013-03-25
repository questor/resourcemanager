
#ifdef RESOURCEMANAGER_INCLUDE_RESIZEIMAGEPROCESSOR

/*
int origWidth, origHeight, comp;
uint32_t *origData = (uint32_t*)stbi_load_from_memory((stbi_uc*)inBuffer, len, &origWidth, &origHeight, &comp, 4);
delete[] inBuffer;

if((width == -1) || (height == -1)) {
   width = origWidth;
   height = origHeight;
   return origData;
} else {
   uint32_t *res = ImageResize::resample<HermiteFilter>(origWidth, origHeight, origData, width, height);
   delete[] origData;
   return res;
   return NULL;
}
*/

#endif   //#ifdef RESOURCEMANAGER_INCLUDE_RESIZEIMAGEPROCESSOR
