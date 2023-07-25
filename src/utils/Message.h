#pragma once

#include "Debugging.h"
#include "String.h"
#include "List.h"
#include "Threading.h"
#include "Array.h"

enum MessageType {
	NoMessage = 0,
	// Windowing
	WindowCreated,
	WindowResized,
	WindowDestroyed,
	// Vulkan
	VulkanMaterialCreated,
	VulkanTextureCreated,
	VulkanLayerCreated,
	VulkanMeshPNTCreated,
	VulkanRenderInstancePNTCreated,
	VulkanLightAdded,
};

struct Message {
public:
	MessageType type;
	void* p1;
	void* p2;
	void* sender;
public:
	explicit Message()
		:type( NoMessage )
		,p1( null )
		,p2( null )
		,sender( null )
	{}
	explicit Message( const Message& src )
		:type( src.type )
		,p1( src.p1 )
		,p2( src.p2 )
		,sender( src.sender )
	{}
public:
	void operator=( const Message& src ){
		type = src.type;
		p1 = src.p1;
		p2 = src.p2;
		sender = src.sender;
	}
};

class MessageHandler {
public:
	virtual bool handle( const Message& message ) = 0;
};

class MessageListener {
public:
	virtual void receive( MessageType type, void* p1, void* p2, void* sender ) = 0;
};

class MessageQueue : public MessageListener {
	DEBUGCOUNTER( MessageQueue );
private:
	std::mutex _mutex;
private:
	enum { MaxMessages = 1000 };
	Message _messages[ MaxMessages ];
	uint _next_read = 0;
	uint _next_write = 0;
public:
	explicit MessageQueue();
	~MessageQueue();
public:
	void post( const Message& message );
	void post( MessageType type, void* p1, void* p2, void* sender );
	bool get( Message& message );
	void handle( MessageHandler* handler, uint max_messagecount = 10 );
protected:
	virtual void receive( MessageType type, void* p1, void* p2, void* sender ) override;
};

class MessageListeners {
private:
	Array<MessageListener*> _listeners;
public:
	explicit MessageListeners( uint max_listener_count );
	~MessageListeners();
public:
	void add( MessageListener* listener );
	void remove( MessageListener* listener );
public:
	void send( MessageType type, void* p1, void* p2, void* sender );
};

