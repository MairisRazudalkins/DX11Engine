//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

cbuffer ModelConstBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer DirectionalLightConstBuffer : register(b1)
{
    float4 LightDiffuse;
    float4 LightAmbient;
    float4 LightSpecular;
    float3 LightDirection;
}

Texture2D txDiffuse : register(t0);
Texture2D txSpec : register(t1);
Texture2D txNorm : register(t2);

SamplerState samLinear : register(s0);

struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float4 PosWS : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT VS_TEXTURED(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 TexCoord : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.PosWS = mul(Pos, World);
    
    output.Pos = output.PosWS;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    output.Normal = normalize(mul(float4(NormalL, 0.f), World).xyz);
    output.TexCoord = TexCoord;

    return output;
}

float4 CalculateAmbient(float2 uv)
{
    return LightAmbient * txDiffuse.Sample(samLinear, uv);
}

float4 CalculateDiffuse(float3 normal, float2 uv)
{
    return max(0.f, dot(normalize(normal), LightDirection)) * LightDiffuse;
}

float4 PS_TEXTURED(VS_OUTPUT input) : SV_Target
{
    return CalculateAmbient(input.TexCoord) + CalculateDiffuse(normalize(input.Normal), input.TexCoord);
}
