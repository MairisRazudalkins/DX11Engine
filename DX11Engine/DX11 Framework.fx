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

cbuffer LightingConstBuffer : register(b1) // Light buffer
{
    float4 DiffuseLight;
    float4 AmbientLight;
    float4 SpecularLight;
    float3 LightVecW;
}

cbuffer MaterialConstBuffer : register(b2) // Mtrl buffer
{
    float4 DiffuseMtrl;
    float4 AmbientMtrl;
    float4 SpecularMtrl;
    float SpecularPower;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    //float4 Pos : SV_POSITION;
    //float3 NormalL : NORMAL0;
    //float4 Color : COLOR0;
    
    float4 PosH : SV_POSITION; // TIDY THIS UP!
    float3 NormalL : NORMAL0;
    float2 Tex : TEXCOORD0;
    float3 PosW : POSITION;
    float4 Color : COLOR0;
};

Texture2D txDiffuse : register(t0);
Texture2D txSpec : register(t1);
Texture2D txNorm : register(t2);

SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.NormalL = mul(float4(NormalL, 0.f), World).xyz;
    output.NormalL = normalize(output.NormalL);
    
    output.PosW = mul(float4(Pos.xyz, 1.f), World).xyz;
    
    output.PosH = mul(Pos, World);
    output.PosH = mul(output.PosH, View);
    output.PosH = mul(output.PosH, Projection);
    
    
    // --------- ORIGINAL --------- \\
    //output.Pos = mul( Pos, World );
    //output.Pos = mul( output.Pos, View );
    //output.Pos = mul( output.Pos, Projection );
    //output.Color.rgba = 1.f;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    input.NormalL = normalize(input.NormalL);

    float4 norm = txNorm.Sample(samLinear, input.Tex);
    norm = (norm * 2.f) - 1.f;

    float3 bumpNormal = (norm.x * 0.2f) + (norm.y * .1f) + (norm.z * input.NormalL);
    bumpNormal = normalize(bumpNormal);

    float lightIntensity = saturate(dot(bumpNormal, LightVecW)) * SpecularPower;

    float4 textureColor = txDiffuse.Sample(samLinear, input.Tex);

    float4 color = saturate(textureColor * lightIntensity);
    color = color * textureColor;
    
    return input.Color;
}
