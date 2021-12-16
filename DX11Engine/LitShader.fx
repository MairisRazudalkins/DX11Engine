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

cbuffer MaterialConstBuffer : register(b2)
{
    float4 DiffuseMtrl;
    float4 AmbientMtrl;
    float4 SpecularMtrl;
    float SpecularPower;
    float3 EyePos;
}

Texture2D txDiffuse : register(t0);
Texture2D txSpec : register(t1);
Texture2D txNorm : register(t2);

SamplerState samLinear : register(s0);

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
    
    output.PosWS = mul(Pos, World);
    
    output.Pos = output.PosWS;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    output.Normal = normalize(mul(float4(NormalL, 0.f), World).xyz);
    output.TexCoord = TexCoord;

    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 CalculateAmbient(float2 uv)
{
    return LightAmbient * AmbientMtrl + txDiffuse.Sample(samLinear, uv);
}

float4 CalculateDiffuse(float3 normal, float2 uv)
{
    return max(0.f, dot(normalize(normal), LightDirection)) * LightDiffuse;
}

float4 CalculateSpecular(float4 position, float3 normal, float2 uv)
{
    return pow(max(dot(reflect(-LightDirection, normal * (txNorm.Sample(samLinear, uv).rgb * 2.f - 1.f)), normalize(EyePos - position.xyz)), 0.0f), SpecularPower) * (SpecularMtrl * (LightSpecular + txSpec.Sample(samLinear, uv)));
    
    //return LightDiffuse * pow(max(0.f, dot(normalize(reflect(-LightDirection, normalize(normal))), EyeDir)), SpecularMtrl);
}

float4 PS_LIT( VS_OUTPUT input ) : SV_Target
{
    input.Normal = normalize(input.Normal);
    
    float4 ambientColor = CalculateAmbient(input.TexCoord);
    float4 diffuseColor = CalculateDiffuse(input.Normal, input.TexCoord);
    float4 specularColor = CalculateSpecular(input.PosWS, input.Normal, input.TexCoord);
    
    float4 color = ambientColor + diffuseColor + specularColor;
    color.a = DiffuseMtrl.a;
    
    return color; //ambientColor + diffuseColor + specularColor;
    
   // return AmbientMtrl + (max(0.f, dot(normalize(LightDirection), input.Normal)) * LightDiffuse); //diffuse shading from light direction
    
    //float3 eyePosDir = normalize(EyePos - input.VertexPos);
    //
    //float3 reflection = 2.f * (dot(LightDirection, input.Normal)) * input.Normal - LightDirection;
    //float4 specular = pow(dot(reflection, eyePosDir), Shininess) * LightDiffuse * SpecularMtrl;
    //
    //return specular;
}
