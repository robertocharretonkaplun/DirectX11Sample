//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer Camera : register(b0)
{
matrix View;
matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b1)
{
matrix World;
float4 vMeshColor;
};

//--------------------------------------------------------------------------------------
// Light data
//--------------------------------------------------------------------------------------
struct LightData
{
float3 Position; // posición de la luz en el espacio del mundo
float3 Color; // color de la luz
float Range; // alcance de la luz (distancia máxima que puede llegar la luz)
float Intensity; // intensidad de la luz
};
static const LightData g_Light = { float3(0, 0, 0), float3(1, 1, 1), 100.0f, 1.0f };

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
float4 Pos : POSITION;
float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
float4 Pos : SV_POSITION;
float2 Tex : TEXCOORD0;
float3 WorldPos : TEXCOORD1; // posición del vértice en el espacio del mundo
};

PS_INPUT VS(VS_INPUT input)
{
PS_INPUT output = (PS_INPUT)0;

// transforma la posición del vértice al espacio del mundo
output.WorldPos = mul(input.Pos, World).xyz;

// transforma la posición del vértice al espacio de la vista y al espacio de proyección
output.Pos = mul(input.Pos, World);
output.Pos = mul(output.Pos, View);
output.Pos = mul(output.Pos, Projection);
output.Tex = input.Tex;

return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
float3 lightDir = g_Light.Position - input.WorldPos;
float dist = length(lightDir);
lightDir /= dist;

float atten = 1.0f / (1.0f + 0.1f * dist + 0.01f * dist * dist);
float intensity = saturate(dot(normalize(lightDir), normalize(float3(0, 0, -1))));

float4 diffuse = txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;
float3 finalColor = diffuse.rgb * g_Light.Color * g_Light.Intensity * atten * intensity;

return float4(finalColor, diffuse.a);
}