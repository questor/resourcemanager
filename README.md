resourcemanager
===============

small resource manager with file monitoring support. depends on my own eastl and eaio.

small sample:

```
ResourceManager *rm = ResourceManager::getInstance();

ResourceHandle handle = rm->registerResource("media/BokehSmall.png", true);
ImageResource *res = (ImageResource*)rm->getResource(handle);

uint32_t *data = res->getData();
int width = res->getWidth();
int height = res->getHeight();
```
