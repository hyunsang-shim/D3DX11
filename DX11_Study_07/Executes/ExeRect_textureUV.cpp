#include "stdafx.h"
#include "ExeRect_textureUV.h"

ExeRect_textureUV::ExeRect_textureUV(ExecuteValues* values)
	: Execute(values)
	, vertexCount(4), indexCount(6)
	, number(0)
	, flagUV(0),	fUV_UtoEnd(0.2f),	fUV_VtoEnd(0.2f)
{
	shader = new Shader(ShaderPath + L"006_TextureUV.hlsl");
	worldBuffer = new WorldBuffer();
	colorBuffer = new ColorBuffer();

	vertices = new VertexTexture[vertexCount];
	vertices[0].Position = D3DXVECTOR3(-1, 0, 0);
	vertices[1].Position = D3DXVECTOR3(-1, 0, 0);
	vertices[2].Position = D3DXVECTOR3(0, -1, 0);
	vertices[3].Position = D3DXVECTOR3(0, 0, 0);

	vertices[0].Uv = D3DXVECTOR2(0, 0);
	vertices[1].Uv = D3DXVECTOR2(fUV_UtoEnd, 0);
	vertices[2].Uv = D3DXVECTOR2(0, fUV_VtoEnd);
	vertices[3].Uv = D3DXVECTOR2(fUV_UtoEnd, fUV_VtoEnd);


	indices = new UINT[indexCount]{ 0, 1, 2, 1, 3, 2 };



	// Sampler DEsc
	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);

		States::CreateSampler(&desc, &state[0]);

		//
		/*desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		States::CreateSampler(&desc, &state[1]);

		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		States::CreateSampler(&desc, &state[2]);

		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		D3DXCOLOR color = D3DXCOLOR(1, 0, 0, 1);
		memcpy(&desc.BorderColor, &color, sizeof(D3DXCOLOR));
		States::CreateSampler(&desc, &state[3]);*/
		
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		States::CreateSampler(&desc, &state[1]); 

	}

	// Create VertexBuffer
	{
		D3D11_BUFFER_DESC  desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(
			&desc, &data, &vertexBuffer
		);

		assert(SUCCEEDED(hr));

	}

	// Create IndexBuffer
	{
		D3D11_BUFFER_DESC  desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(
			&desc, &data, &indexBuffer
		);

		assert(SUCCEEDED(hr));
	}

	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
			D3D::GetDevice(),						// Device to draw
			(TexturesPath + L"crate.png").c_str(),	// Source file (Full path)
			NULL,									// D3DX11_IMAGE_LOAD_INFO *pLoadInfo 
			NULL,									// ID3DX11ThreadPump *pPump 
			&srv,									// ID3DX11ShaderResourceView **ppShaderResourceView 
			NULL									// HRESULT *pHResult
		);

		assert(SUCCEEDED(hr));
	}

}

ExeRect_textureUV::~ExeRect_textureUV()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeRect_textureUV::Update()
{

	if (Keyboard::Get()->Up('P'))
	{
		flagUV++;

		if (flagUV > 2)
			flagUV = 0;

	}



	if (flagUV >0)
	{
		switch (flagUV)
		{
		case 1:
			vertices[1].Uv.x = fUV_UtoEnd;
			vertices[3].Uv.x = fUV_UtoEnd;
			break;
		case 2:
			vertices[2].Uv.y = fUV_VtoEnd;
			vertices[3].Uv.y = fUV_UtoEnd;
			break;
		}

		D3D::GetDC()->UpdateSubresource(
			vertexBuffer,
			0,
			NULL,
			vertices,
			sizeof(VertexColor) * vertexCount,
			0);

	}

	

}

void ExeRect_textureUV::PreRender()
{
}

void ExeRect_textureUV::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	// IA = Input Assembler
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	colorBuffer->SetPSBuffer(0);
	worldBuffer->SetVSBuffer(1);
	shader->Render();
	D3D::GetDC()->PSSetSamplers(0, 1, &state[number]);
	D3D::GetDC()->PSSetShaderResources(0, 1, &srv);
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ExeRect_textureUV::PostRender()
{

	ImGui::Begin("Color");
	{
		ImGui::SliderInt("Number", &number, 0, 1);

		switch (flagUV)
		{
		case 0:
			ImGui::Text("No U or V Selected. Pressing Key [ P ] cycles none - U - V"); 
			break;
		case 1:
			ImGui::Text("U-Coord Selected. Pressing Key [ P ] cycles none - U - V");
			ImGui::SliderFloat("Selected Coord : U2", &fUV_UtoEnd, 0.0f, 1.0f);
			break;
		case 2:
			ImGui::Text("V-Coord Selected. Pressing Key [ P ] cycles none - U - V");
			ImGui::SliderFloat("Selected Coord : V2", &fUV_VtoEnd, 0.0f, 1.0f);
			break;
		}
		
	}
	ImGui::End();
}

void ExeRect_textureUV::ResizeScreen()
{
}
