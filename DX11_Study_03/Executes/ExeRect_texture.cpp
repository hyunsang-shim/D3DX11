#include "stdafx.h"
#include "ExeRect_texture.h"

ExeRect_texture::ExeRect_texture(ExecuteValues* values)
	: Execute(values)
	, vertexCount(4), indexCount(6)
{
	shader = new Shader(defShader + L"004_Texture.hlsl");
	worldBuffer = new WorldBuffer();
	colorBuffer = new ColorBuffer();

	vertices = new VertexTexture[vertexCount];
	vertices[0].Position = D3DXVECTOR3(0, 0, 0);
	vertices[1].Position = D3DXVECTOR3(0, 1, 0);
	vertices[2].Position = D3DXVECTOR3(1, 0, 0);
	vertices[3].Position = D3DXVECTOR3(1, 1, 0);

	vertices[0].Uv = D3DXVECTOR2(0, 0);
	vertices[1].Uv = D3DXVECTOR2(1, 0);
	vertices[2].Uv = D3DXVECTOR2(0, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 1);


	indices = new UINT[indexCount]{ 0, 1, 2, 1, 3, 2 };

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
			D3D::GetDevice(), (defTextures + L"crate.png").c_str(), NULL, NULL,
				&srv,
				NULL);

		assert(SUCCEEDED(hr));
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

	D3D::GetDC()->PSSetShaderResources(0, 1, &srv);
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ExeRect_texture::PostRender()
{
	ImGui::Begin("Color");
	{
		ImGui::SliderFloat3("Color", (float*)&colorBuffer->Data.Color, 0, 1);
	}
	ImGui::End();
}

void ExeRect_texture::ResizeScreen()
{
}
