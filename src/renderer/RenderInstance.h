#pragma once

#include "RenderCore.h"

class RenderInstance
{
	DEBUGCOUNTER(RenderInstance)
private:
    PosOri _posori;
private:
	vec4 _color;
private:
	float _textureOffsetY = 0;
	float _textureTile = 0;
protected:
    explicit RenderInstance( const PosOri& posori );
    virtual ~RenderInstance();
public:
    const PosOri& posori() const{ return _posori; }
    void setPosOri( const PosOri& posori );
public:
	const vec4& color() const{ return _color; }
	void setColor( const vec4& color );
public:
	float textureOffsetY() const{ return _textureOffsetY; }
	void setTextureOffsetY( float textureOffsetY );
	void addTextureOffsetY( float value );
public:
	float textureTile() const{ return _textureTile; }
	void setTextureTile( float textureTile );
};

class RenderInstancePNT : public RenderInstance
{
private:
	Mesh<VertexPNT>* _mesh;
	Material* _material;
protected:
    explicit RenderInstancePNT( const PosOri& posori, Mesh<VertexPNT>* mesh, Material* material );
    virtual ~RenderInstancePNT() override;
public:
	Mesh<VertexPNT>* mesh() const{ return _mesh; }
	Material* material() const{ return _material; }
};
