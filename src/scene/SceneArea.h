#pragma once

#include "SceneCore.h"
#include "SceneNode.h"

class SceneArea : public SceneNode
{
	DEBUGCOUNTER(SceneArea)
private:
	String _name;
	RenderLayer* _layer = null;
//private:
//	Mat4 _transformFromParent;
//	Mat4 _transformToParent;
public:
	explicit SceneArea( const String& name, RenderLayer* layer );
	virtual ~SceneArea();
public:
	const String& name() const{ return _name; }
//public:
//	const Mat4& transformFromParent() const{ return _transformFromParent; }
//	const Mat4& transformToParent() const{ return _transformToParent; }
//	void setTransformInParent( const Mat4& transform, float scale );
//	PosOri transformPosOriFromParentToLocal( const PosOri& posoriInParent );
//	PosOri transformPosOriFromLocalToParent( const PosOri& posoriInLocal );
public:
	RenderLayer* layer() const{ return _layer; }
//public:
//	void addRenderable( Renderable* renderable );
//	void addRenderables( const List<Renderable*>& renderables );
//	void removeRenderable( Renderable* renderable );
//	void removeRenderables( const List<Renderable*>& renderables );
public:
	virtual SceneArea* findArea() override { return this; }
//	virtual bool isInArea() const override{ return true; }
//	virtual SceneArea* area() override{ return this; }
//public:
//	virtual bool isArea() const override { return true; }
//	virtual SceneArea* asArea() override { return this; }
};

//class SceneAreaShifter : public Property
//{
//public:
//	static PropertyType Type;
//private:
//	String _name;
//	PosOriProperty* _posori = null;
//	float _radius;
//private:
//	SceneArea* _target_area = null;
//public:
//	explicit SceneAreaShifter( const String& name, float radius, SceneArea* target_area );
//	virtual ~SceneAreaShifter() override;
//public:
//	const String& name() const{ return _name; }
//public:
//	const Vec3& position() const { return _posori->get().position(); }
//	float radius() const{ return _radius; }
//public:
//	SceneArea* target_area() const{ return _target_area; }
//protected:
//	virtual void bind( PropertyObject* object ) override;
//};

//class SceneAreaJumper : public AnimatedProperty
//{
//public:
//	static PropertyType Type;
//private:
//	SceneObject* _object = null;
//	PosOriProperty* _posori = null;
//	float _radius;
//private:
//	SceneArea* _prev_area = null;
//	SceneAreaShifter* _next_shifter = null;
//public:
//	explicit SceneAreaJumper( float radius );
//	virtual ~SceneAreaJumper() override;
//public:
//	const Vec3& position() const { return _posori->get().position(); }
//	float radius() const{ return _radius; }
//protected:
//	virtual void bind( PropertyObject* object ) override;
//protected:
//	virtual void animate( float dt ) override;
//private:
//	void checkForShifter( SceneNode* node );
//};
