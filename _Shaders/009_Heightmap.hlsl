cbuffer VS_ViewProjection : register(b0)
{
	matrix _view;
	matrix _projection;
}

cbuffer VS_World : register(b1)
{
	matrix _world;
}

cbuffer PS_Color : register(b0)
{
	float4 Color;
}

struct VertexInput
{
	float4 position : POSITION0;
   // float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION;		
    float4 color : COLOR0;	
};

// Added
SamplerState Sampler : register(s0);
Texture2D Map : register(t0);
Texture2D Map2 : register(t1);


PixelInput VS(VertexInput input)
{
	PixelInput output;
    // added. vertex height -> color
    float4 color = float4(1, 1, 1, 1);

    if (input.position.y > 10.0f)
        color = float4(1, 1, 0, 1);
    if (input.position.y > 20.0f)
        color = float4(1, 0, 0, 1);
    if (input.position.y > 30.0f)
        color = float4(0, 1, 0, 1);
    if (input.position.y > 40.0f)
        color = float4(0, 0, 1, 1);	
    
    output.color = color;
    output.position = mul(input.position, _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);    

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	//return Color;
    return input.color;

}