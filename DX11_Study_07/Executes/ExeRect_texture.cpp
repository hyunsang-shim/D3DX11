#include "stdafx.h"
#include "ExeRect_texture.h"

ExeRect_texture::ExeRect_texture(ExecuteValues* values)
	: Execute(values)
	, vertexCount(4), indexCount(6)
	, number(0)
	, flagUV(0), fUV_UtoEnd(0.2f), fUV_VtoEnd(0.2f)
	, Location(0, 0), Rotation(0, 0, 0), Scale(1, 1)
	, number2(0)
{
	D3DXMatrixIdentity(&World);

	shader = new Shader(ShaderPath + L"005_TextureMulti.hlsl");
	worldBuffer = new WorldBuffer();
	colorBuffer = new ColorBuffer();

	vertices = new VertexTexture[vertexCount];
	// 12-27 modified
	vertices[0].Position = D3DXVECTOR3(-0.5, -0.5, 0);
	vertices[1].Position = D3DXVECTOR3(-0.5,  0.5, 0);
	vertices[2].Position = D3DXVECTOR3( 0.5, -0.5, 0);
	vertices[3].Position = D3DXVECTOR3( 0.5,  0.5, 0);

	vertices[0].Uv = D3DXVECTOR2(0, 0);
	vertices[1].Uv = D3DXVECTOR2(0, 1);
	vertices[2].Uv = D3DXVECTOR2(1, 0);
	vertices[3].Uv = D3DXVECTOR2(1, 1);


	indices = new UINT[indexCount]{ 0, 1, 2, 3,2,1 };



	// Sampler Desc
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

		HRESULT hr2 = D3DX11CreateShaderResourceViewFromFile(
			D3D::GetDevice(),						// Device to draw
			(TexturesPath + L"wall.png").c_str(),	// Source file (Full path)
			NULL,									// D3DX11_IMAGE_LOAD_INFO *pLoadInfo 
			NULL,									// ID3DX11ThreadPump *pPump 
			&srv2,									// ID3DX11ShaderResourceView **ppShaderResourceView 
			NULL									// HRESULT *pHResult
		);

		assert(SUCCEEDED(hr));
	}

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);	// for front face

		desc.CullMode = D3D11_CULL_NONE;
		States::CreateRasterizer(&desc, &rasterizer[1]);	// for back face
	}

}

ExeRect_texture::~ExeRect_texture()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeRect_texture::Update()
{
	D3DXMATRIX Y, Z, T;
	D3DXMatrixIdentity(&Y);
	D3DXMatrixIdentity(&Z);
	D3DXMatrixIdentity(&T);

	if (Keyboard::Get()->Press(VK_SHIFT))
	{
		if (Keyboard::Get()->Press('A'))
			Rotation.y -= 30.0f * Time::Delta();

		if (Keyboard::Get()->Press('D'))
			Rotation.y += 30.0f * Time::Delta();


		if (Keyboard::Get()->Press('W'))
			Rotation.z -= 30.0f * Time::Delta();
		if (Keyboard::Get()->Press('S'))
			Rotation.z += 30.0f * Time::Delta();
	}
	else
	{
		if (Keyboard::Get()->Press('A'))
			Location.x -= 10.0f * Time::Delta();

		if (Keyboard::Get()->Press('D'))
			Location.x += 10.0f * Time::Delta();


		if (Keyboard::Get()->Press('W'))
			Location.y += 10.0f * Time::Delta();
		if (Keyboard::Get()->Press('S'))
			Location.y -= 10.0f * Time::Delta();
	}


	D3DXMatrixTranslation(&T, Location.x, Location.y, 0);
	D3DXMatrixRotationY(&Y, Math::ToRadian(Rotation.y));
	D3DXMatrixRotationZ(&Z, Math::ToRadian(Rotation.z));

	World = Y * Z * T;
}

void ExeRect_texture::PreRender()
{
}

void ExeRect_texture::Render()
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
	D3D::GetDC()->PSSetShaderResources(1, 1, &srv2);

	{
		D3D::GetDC()->RSSetState(rasterizer[number2]);
	}

	{
		worldBuffer->SetMatrix(World);
		worldBuffer->SetVSBuffer(1);
	}
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);


	// 2018-12-27
	/*{
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		world._41 = 3;
		world._42 = 0;
		world._43 = 0;
		worldBuffer->SetMatrix(world);
		worldBuffer->SetVSBuffer(1);
	}
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);*/

	// 2018-12-27
	/*{
		D3DXMATRIX world2;
		D3DXMatrixIdentity(&world2);
		world2._41 = 2;
		world2._42 = 0;
		world2._43 = 0;
		worldBuffer->SetMatrix(world2);
		worldBuffer->SetVSBuffer(1);
		D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	}*/

	// 2018-12-27
	/*{
		D3DXMATRIX world3, s, t;
		D3DXMatrixTranslation(&t, 0, 0, 0);
		D3DXMatrixScaling(&s, 2, 2, 1);
		world3 = s * t;
		worldBuffer->SetMatrix(world3);
		worldBuffer->SetVSBuffer(1);
		D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	}*/


	// 2018-12-27
	/*{
		D3DXMATRIX world4, s, r, t;
		D3DXMatrixTranslation(&t, -2, -2, 0);
		D3DXMatrixScaling(&s, 2, 2, 1);
		D3DXMatrixRotationY(&r, Math::ToRadian(-45));
		world4 = s * r * t;
		worldBuffer->SetMatrix(world4);
		worldBuffer->SetVSBuffer(1);
		D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	}*/

}

void ExeRect_texture::PostRender()
{

	ImGui::Begin("Color");
	{
		ImGui::SliderInt("Number", &number, 0, 1);
		ImGui::SliderInt("Rasterizer Index", &number2, 0, 1);	// rasterizer Select
	}
	ImGui::End();
}

void ExeRect_texture::ResizeScreen()
{
}
