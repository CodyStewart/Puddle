#include <iostream>
#include <optional>
#include <cstring>

#include "commonTypedefs.h"
#include "macros.h"
#include "resourceManager.h"

ResourceHandle::ResourceHandle(Resource& resource, char* buffer, uint32 size) : _resource(resource) {
	_buffer = buffer;
	_size = size;
}

ResourceHandle::~ResourceHandle() {
	//SAFE_DELETE_ARRAY(_buffer);
}

ResourceCache::ResourceCache() {
	resList.clear();
	resMap.clear();

	_cacheSize = 0;
	_allocated = 0;
}

// NOTE: for now we are simply adding to the allocated and cachesize whatever gets loaded at application start,
// Later, we will make it so that we create a specified cacheSize and allocate up to that cacheSize. When we need more memory, we'll increase the cacheSize
void ResourceCache::addToList(ResHandleShrdPtr listItem) {
	resList.push_back(listItem);
}

void ResourceCache::addToMap(ResHandleMapItem mapPair) {
	resMap.insert(mapPair);
}

ResHandleShrdPtr ResourceCache::getHandle(Resource* res) {
	ResHandleShrdPtr handle = nullptr;
	ResourceMap::iterator itemItr = resMap.find(res->_name);
	if (itemItr != resMap.end())
		handle = itemItr->second;
	else
		std::cout << "Resource " << res->_name << " not found!\n";

	return handle;
}


void ResourceCache::increaseCacheSize(uint32 amountToAdd) {
	_cacheSize += amountToAdd;
}

void ResourceCache::increaseAllocated(uint32 amountToAllocate) {
	_allocated += amountToAllocate;
}

bool ResourceLoader::loadResourcesFromFile(ZipFile* zip, ResourceCache* cache) {
	bool success = true;

	char* buffer = nullptr;
	for (ZipContentsMap::const_iterator itr = zip->m_ZipContentsMap.cbegin(); itr != zip->m_ZipContentsMap.cend(); itr++) {
		std::optional<int> index = itr->second;
		if (index.has_value()) {
			uint32 size = zip->GetFileLen(*index);
			buffer = new char[size];

			if (buffer) {
				zip->ReadFile(*index, buffer);
				Resource res(itr->first);
				ResourceHandle handle(res, buffer, size);
				ResHandleShrdPtr ptr = std::make_shared<ResourceHandle> (handle);
				cache->addToList(ptr);

				ResHandleMapItem item = std::make_pair (res._name, ptr);
				cache->addToMap(item);

				cache->increaseCacheSize(size);
				cache->increaseAllocated(size);
			}
			else {	// we were unable to allocate new memory
				std::cout << "Unable to allocate new memory in loadResourcesFromFile!\n";
				success = false;
			}
		}
	}

	return success;
}

ResourceManager::ResourceManager() {

}

bool ResourceManager::loadResources(ZipFile* zip) {
	bool success = true;

	success = _loader.loadResourcesFromFile(zip, &_cache);

	return success;
}

ResHandleShrdPtr ResourceManager::getHandle(Resource* res) {
	return _cache.getHandle(res);
}