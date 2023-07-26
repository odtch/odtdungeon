#pragma once

#include "AssImpCore.h"
#include <assimp/scene.h>

// https://xphere.me/2019/05/bones-animation-with-openglassimpglm/
// For converting between ASSIMP and glm
static inline glm::vec3 ai_vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 ai_vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
static inline glm::quat ai_quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::mat4 ai_mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }
static inline glm::mat4 ai_mat4_cast(const aiMatrix3x3 &m) { return glm::transpose(glm::make_mat3(&m.a1)); }
