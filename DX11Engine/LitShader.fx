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

cbuffer DirectionalLightConstBuffer : register(b1)
{
    float4 LightDiffuse;
    float4 LightSpecular;
    float3 LightDirection;
}

cbuffer MaterialConstBuffer : register(b2)
{
    float4 DiffuseMtrl;
    float4 AmbientMtrl;
    float4 SpecularMtrl;
    float SpecularPower;
    float3 EyePos;
}

// Math and equasions used from -> https://www.3dgep.com/texturing-lighting-directx-11/

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float4 PosWS : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS_LIT(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 TexCoord : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    output.PosWS = mul(World, Pos);
    
    output.Normal = normalize(mul(float4(NormalL, 0.0f), World).xyz);
    output.TexCoord = TexCoord;
    
    
    //output.VertexPos = Pos;
    //    
    //output.VertexPos = mul(Pos, World);
    //output.VertexPos = mul(output.VertexPos, View);
    //output.VertexPos = mul(output.VertexPos, Projection);
    //
    //output.Normal = normalize(mul(float4(NormalL, 0.0f), World).xyz);
    
    
    //float diffuseAmount = max(dot(normalize(LightDirection), output.Normal), 0.f);
    //output.Color.rgb = diffuseAmount * (DiffuseMtrl * LightDiffuse).rgb;
    //output.Color.a = DiffuseMtrl.a;
    //
    //output.EyePos = -(EyePos.xyz - mul(Pos, output.PosH)).xyz;
    //
    //output.Tex = TexCoord;
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_LIT( VS_OUTPUT input ) : SV_Target
{
    input.Normal = normalize(input.Normal);
    
    //diffuse shading from light direction
    return max(0.f, dot(normalize(LightDirection), input.Normal)) * LightDiffuse; // * DiffuseMtrl;
    
    //float3 eyePosDir = normalize(EyePos - input.VertexPos);
    //
    //float3 reflection = 2.f * (dot(LightDirection, input.Normal)) * input.Normal - LightDirection;
    //float4 specular = pow(dot(reflection, eyePosDir), Shininess) * LightDiffuse * SpecularMtrl;
    //
    //return specular;
}
