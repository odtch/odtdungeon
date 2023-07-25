#pragma once

#include "vulkan/VulkanCore.h"
#include "vulkan/command/VulkanTask.h"

class VulkanMeshLoader {
	DEBUGCOUNTER( VulkanMeshLoader );
	ODTNOCOPY( VulkanMeshLoader );
private:
	String _name;
private:
	VulkanCommandPool* _commandPool = null;
private:
	VulkanTask _loadMB;
	VulkanTask _buildBLAS;
public:
	explicit VulkanMeshLoader();
	virtual ~VulkanMeshLoader();
public:
	void create( const String& name, VulkanCommandPool* commandPool );
	void destroy();
public:
	void reinit();
	void tryUpdateIfNeeded( VulkanMesh* mesh );
	void start();
public:
	VulkanTask& loadMBTask(){ return _loadMB; }
	VulkanTask& buildBLASTask(){ return _buildBLAS; }
public:
	friend class VulkanMeshBackgroundLoader;
};

class VulkanMeshBackgroundLoader {
	DEBUGCOUNTER( VulkanMeshBackgroundLoader );
	ODTNOCOPY( VulkanMeshBackgroundLoader );
private:
	VulkanMeshLoader _loader;
	bool _running = false;
	bool _hasnewwork = false;
	int count = 0;
public:
	explicit VulkanMeshBackgroundLoader();
	virtual ~VulkanMeshBackgroundLoader();
public:
	void create( const String& name, VulkanCommandPool* commandPool );
	void destroy();
public:
	void preTick();
	void tryUpdateIfNeeded( VulkanMesh* mesh );
	void postTick();
};
