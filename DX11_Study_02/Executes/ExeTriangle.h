#pragma once
#include "Execute.h"

class ExeTriangle :
	public Execute
{
private:
	Shader* shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	UINT			vertexCount, indexCount;
	Vertex* vertices;		// vertices only (FVF)
	UINT* indices;



public:
	ExeTriangle( ExecuteValues* values);
	~ExeTriangle();
	
	// Program.cpp
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;


};