//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
	output.Position = float4(input.Position, 1.f);
	// Set the TexCoord
	output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    // Step 1: sample the texture
	float3 col = gTexture.Sample(samPoint, input.TexCoord).rgb;
	// Step 2: calculate the greyscale weighted average -> lumenocity method
	// this averagdes the wavelength instead of the pure color
	// https://www.tutorialspoint.com/dip/grayscale_to_rgb_conversion.htm
	float greyScale = 0.299 * col.r + 0.587 * col.g + 0.114 * col.b;
	// Step 3: return the color
    return float4( greyScale, greyScale, greyScale, 1.0f );
}


//TECHNIQUE
//---------
technique10 Grayscale
{
    pass P0
    {          
        // Set states
		SetRasterizerState(BackCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
