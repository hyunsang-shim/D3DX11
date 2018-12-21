#pragma once
#include "Executes\Execute.h"

class ExeRect :
	public Execute
{
private:
	int number;
	class ColorBuffer;

	Shader* shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	UINT			vertexCount, indexCount;
	VertexColor* vertices;		// vertices only (FVF)
	UINT* indices;
	ColorBuffer* colorBuffer;




public:
	ExeRect(ExecuteValues* values);
	~ExeRect();

	// Program.cpp
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;


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