#pragma once
#include "Executes\Execute.h"

class ExeRect_texture :
	public Execute
{
private:
	
	class ColorBuffer;

	Shader* shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	UINT			vertexCount, indexCount;
	VertexTexture*	vertices;	
	UINT* indices;
	ColorBuffer* colorBuffer;
	ID3D11ShaderResourceView* srv;		// 

	int number;
	ID3D11SamplerState*			state[4];



public:
	ExeRect_texture(ExecuteValues* values);
	~ExeRect_texture();

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