//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ModelConstBuffer : register(b0) // Object buffer
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer ColorConstBuffer : register(b1)
{
    float4 color1;
    float4 color2;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 TexCoord : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.Position = mul(Pos, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Tex = TexCoord;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    //return lerp(color1, color2, input.Tex.y);
    return lerp(float4(0.2f, 0.6f, 1.f, 1.f), float4(0.f, 0.f, 0.8f, 1.f), input.Tex.y);
}
