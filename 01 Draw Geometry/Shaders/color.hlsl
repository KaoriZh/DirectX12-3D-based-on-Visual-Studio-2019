//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0) {
	float4x4 gWorld;
};

cbuffer cbPass: register(b1) {
	float4x4 gView;
	float4x4 gInvView;
	float4x4 gProj;
	float4x4 gInvProj;
	float4x4 gViewProj;
	float4x4 gInvViewProj;
	float3 gEyePosW;
	float cbPerObjectPad1;
	float2 gRenderTargetSize;
	float2 gInvRenderTargetSize;
	float gNearZ;
	float gFarZ;
	float gTotalTime;
	float gDeltaTime;
}

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// vin.PosL.xyz *= 1.25f + 0.75 * sin(gTime);
	
	// Transform to homogeneous clip space.
	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(posW, gViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
	float sin1 = 0.5f * sin(5.0f * gTotalTime / 3.0f) + 0.5f;
	float sin2 = 0.5f * sin(5.0f * gTotalTime / 5.0f) + 0.5f;
	float sin3 = 0.5f * sin(5.0f * gTotalTime / 7.0f) + 0.5f;
	float4 color;
	color.r = sin1;
	color.g = sin2;
	color.b = sin3;
	color.a = 1.0f;
	// clip(color.r - 0.5f);
    return color;
}