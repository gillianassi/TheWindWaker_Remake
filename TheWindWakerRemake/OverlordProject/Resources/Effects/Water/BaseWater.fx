float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4 gColor:Diffuse <
 string UIName = "Color";
 string UIWidget = "Color";
> = float4(0.03f, 0.23f, 0.39f,1.f);


float gBrightness 
<
 string UIName = "Brightness";
 string UIWidget = "Slider";
 float UIMin = 0.f;
 float UIMax = 1.f; 

 float UIStep = 0.1f;
> = 0.4f; 


struct VS_INPUT {
	float3 pos : POSITION;
	float3 normal : NORMAL;
};
struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};
//DepthEnable = true;
//DepthWriteMask = true;
//DepthFunc = less;
//StencilEnable = false;
//
//StencilReadMask = 0x0F;
//StencilWriteMask = 0x0F;
//
//FrontFaceStencilFunc = always;
//BackFaceStencilFunc = always;
//
//FrontFaceStencilDepthFail = keep;
//BackFaceStencilDepthFail = keep;
//
//FrontFaceStencilPass = keep;
//BackFaceStencilPass = keep;
//
//FrontFaceStencilFail = keep;
//BackFaceStencilFail = keep;

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input) {
	VS_OUTPUT output;
	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float3 color_rgb = gColor.rgb;
	float color_a = gColor.a;

	////HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection) + gBrightness;
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	//color_rgb = color_rgb * diffuseStrength;

	return float4(color_rgb , color_a);
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

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
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

