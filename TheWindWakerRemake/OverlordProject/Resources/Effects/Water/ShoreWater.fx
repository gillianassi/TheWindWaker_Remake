float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4 gColor :Diffuse <
 string UIName = "WaterColor";
 string UIWidget = "Color";
> =float4(1.0f, 1.0f, 1.0f, 1.0f);

float4 gFoamColor :Diffuse <
 string UIName = "FoamColor";
 string UIWidget = "Color";
> =float4(1.0f, 1.0f, 1.0f, 1.0f);

float2 gUVSpeed = float2(0.f,0.f);
Texture2D gFoamDiffuse;
Texture2D gWaterHighlight;

float gTotalTime
<
 string UIName = "TimeSpent";
 string UIWidget = "Slider";
 float UIMin = 0.0;
 float UIMax = 10.0; 

 float UIStep = 0.01f;
> = 0.f; 

float gPI = 3.14159265;
float gHighlightStrength = 0.2f;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Clamp;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{

	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );

	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	// Move the texCoord = input
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float3 color_rgb= gColor.rgb;

	//--float2 movingUVs = input.texCoord + (gUVSpeed * gTotalTime);
	
	float2 UVOffset = gUVSpeed;
	UVOffset.x = gUVSpeed.x * gTotalTime;
	// wave base formula A * cos(2 * pi * f * t + delta)  
	// Cos to start at the highest point of the wave (from 1 to -1)
	// Amplitude = 0.25 to only go over half the UV (From 0.25 to - 0.25)
	// sub 0.25 too the function (from 0 to -0.5)
	// PhaseSHift of Pi to start at the bottom of the sine wave (-0.5 to 0)
	float frequency = gUVSpeed.y;
	UVOffset.y = 0.25f * cos(2 * gPI * frequency * gTotalTime) - 0.25f;//+ 0.75f;

	float2 baseUV = input.texCoord + UVOffset;
	float2 HorizontalReversedUVOffset = UVOffset;
	HorizontalReversedUVOffset.x = -UVOffset.x;
	float2 reversedUV = input.texCoord + HorizontalReversedUVOffset;

	float4 baseTextureColor = gFoamDiffuse.Sample(samLinear, baseUV);
	float4 reversedTextureColor = gFoamDiffuse.Sample(samLinear, reversedUV);
	float4 HighlightColor = gWaterHighlight.Sample(samLinear, baseUV);

	float opacity = gColor.a * (baseTextureColor.a + reversedTextureColor.a + HighlightColor.a);
	//float opacity = gColor.a * baseTextureColor.a * reversedTextureColor.a;


	color_rgb += (baseTextureColor.rgb + reversedTextureColor.rgb);
	color_rgb += HighlightColor.rgb * (0.5f* cos(2 * gPI * frequency * gTotalTime) + 0.5f) * gHighlightStrength;
	//color_rgb += baseTextureColor.rgb + reversedTextureColor.rgb;

	color_rgb = saturate(color_rgb);
	if(color_rgb.g >= 0.9f)
	{
		color_rgb = gFoamColor;
	}
		////HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	//color_rgb = color_rgb * diffuseStrength;
	return float4(color_rgb, opacity);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique10 Default
{

	    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

technique10 TransparencyTech
{
	pass P0
	{
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

