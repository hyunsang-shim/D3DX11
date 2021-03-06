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
	ID3D11ShaderResourceView* srv2;		// 

	int number;
	ID3D11SamplerState*			state[4];
	float fUV_UtoEnd;
	float fUV_VtoEnd;
	int flagUV = 0;				// UV Coord for U(end value) -1 | UV Coord for V(end value) 1 | 0 for select none



	D3DXVECTOR2		Location;
	D3DXVECTOR3		Rotation;
	D3DXVECTOR2		Scale;
	D3DXMATRIX		World;

	int number2;
	ID3D11RasterizerState* rasterizer[2];

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