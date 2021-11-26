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

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
};


VS_OUTPUT VS_UNLIT(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 TexCoord : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
        
    output.Position = mul(Pos, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Color = float4(1.f, 1.f, 1.f, 1.f);
    
    return output;
}


float4 PS_UNLIT( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}
