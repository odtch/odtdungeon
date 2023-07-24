#pragma once

#include "ResourceCore.h"

struct VertexP {
	glm::vec3 pos;
//public:
//	static ResourceType* GetResourceType();
};

struct VertexPNT {
	glm::vec3 position; float dummy;
	glm::vec3 normal; float dummy2;
	glm::vec2 texCoord; float dummy3[2];
public:
	VertexPNT()
		:position( 0, 0, 0 )
		,normal( 0, 0, 0 )
		,texCoord( 0, 0 )
	{}
	VertexPNT( const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texCoord )
		:position( pos )
		,normal( normal )
		,texCoord( texCoord )
	{}
	bool operator==(const VertexPNT& other) const {
		return position == other.position && normal == other.normal && texCoord == other.texCoord;
	}
//	void setInterpolated( const VertexPNT& a, const VertexPNT& b, float fa ){
//		float fb = 1.0f - fa;
//		position = a.position * fa + b.position * fb;
//		normal = glm::normalize( a.normal * fa + b.normal * fb );
//		texCoord = a.texCoord * fa + b.texCoord * fb;
//	}
//	static bool Equals( const VertexPNT& a, const VertexPNT& b ){
//		const float maxdiff = 0.41f;
//		if( maxdiff < Vec3( a.position - b.position ).length2() )
//			return false;
//		if( maxdiff < Vec3( a.normal - b.normal ).length2() )
//			return false;
////		if( maxdiff < Vec2( a.texCoord - b.texCoord ).length2() )
////			return false;
//		return true;
//	}
//	static VkVertexInputBindingDescription getBindingDescription() {
//		VkVertexInputBindingDescription bindingDescription{};
//		bindingDescription.binding = 0;
//		bindingDescription.stride = sizeof(VertexPNT);
//		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//		return bindingDescription;
//	}
//	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
//		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
//		attributeDescriptions[0].binding = 0;
//		attributeDescriptions[0].location = 0;
//		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
//		attributeDescriptions[0].offset = offsetof(VertexPNT, position);
//		attributeDescriptions[1].binding = 0;
//		attributeDescriptions[1].location = 1;
//		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//		attributeDescriptions[1].offset = offsetof(VertexPNT, normal);
//		attributeDescriptions[2].binding = 0;
//		attributeDescriptions[2].location = 2;
//		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
//		attributeDescriptions[2].offset = offsetof(VertexPNT, texCoord);
//		return attributeDescriptions;
//	}
//	static ResourceType* GetResourceType();
};

