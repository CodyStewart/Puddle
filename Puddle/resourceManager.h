#pragma once

#include <algorithm>
#include <string>
#include <list>
#include <map>
#include <memory>

#include "commonTypedefs.h"
#include "zipFile.h"

struct Resource {
	std::string _name;
	Resource(const std::string& name) {
		_name = name;
		std::transform(_name.cbegin(), _name.cend(), _name.begin(), [](unsigned char c) { return std::tolower(c); });
	}
};

struct ResourceHandle {
	ResourceHandle(Resource& resource, char* buffer, uint32 size);
	~ResourceHandle();

	const std::string getName() { return _resource._name; }
	uint32 size() const { return _size; }
	char* buffer() const { return _buffer; }
	char* writableBuffer() { return _buffer; }

private:
	Resource _resource;
	char* _buffer;
	uint32 _size;
};

typedef std::list< std::shared_ptr <ResourceHandle> > ResourceList;		// NOTE: not used for this application but may come in handy when Least Recently Used Cache is implemented
typedef std::map< std::string, std::shared_ptr <ResourceHandle> > ResourceMap;
typedef std::shared_ptr<ResourceHandle> ResHandleShrdPtr;
typedef std::pair<std::string, std::shared_ptr <ResourceHandle> > ResHandleMapItem;

struct ResourceCache {
	ResourceCache();

	void addToList(ResHandleShrdPtr listItem);
	void addToMap(ResHandleMapItem mapPair);

	std::shared_ptr<ResourceHandle> getHandle(Resource* res);

	void increaseCacheSize(uint32 amountToAdd);
	void increaseAllocated(uint32 amountToAllocate);

private:
	ResourceList resList;
	ResourceMap resMap;
	uint32 _cacheSize;
	uint32 _allocated;
};

// NOTE: ResourceLoader currently has no data, only behavior. This is so it can be expanded in the future when resources may have special load requirements
struct ResourceLoader {
	bool loadResourcesFromFile(ZipFile* zip, ResourceCache* cache);
};

struct ResourceManager {
	friend struct ResourceLoader; 

	ResourceManager();

	bool loadResources(ZipFile* zip);
	ResHandleShrdPtr getHandle(Resource* res);

private:
	ResourceCache _cache;
	ResourceLoader _loader;
};