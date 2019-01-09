#pragma once
#include "Executes\Execute.h"

class ExeGrid :
	public Execute
{
private:
	typedef VertexTexture VertexType;	//added
	
	UINT width, height;
	
	
	class ColorBuffer;
	Shader* shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	UINT			vertexCount, indexCount;
	VertexType* vertices;
	UINT* indices;
	ColorBuffer* colorBuffer;




public:
	ExeGrid(ExecuteValues* values);
	~ExeGrid();

	// Program.cpp
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;

	
	Texture* texture[3];


private:
	class ColorBuffer : public ShaderBuffer
	{
	public:
		ColorBuffer()
			:ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Color = D3DXCOLOR(1, 1, 1, 1);
		}
		~ColorBuffer()
		{

		}
		struct Struct 
		{
			D3DXCOLOR Color;
		}Data;
	};



};