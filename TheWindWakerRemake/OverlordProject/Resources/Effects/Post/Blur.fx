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
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
	float width, height;
	gTexture.GetDimensions(width, height);
	
	// Step 2: calculate dx and dy (UV space for 1 pixel)
	float dx = 1 / width;
	float dy = 1 / height;
	
	// Step 3: Create a double for loop (5 iterations each)
	
	uint boxWidth = 5;
	uint pixelsFromCenter = floor(5 * 0.5f);
	float3 color = float3(0.f, 0.f, 0.f);
	float2 offset;
	float2 SampledTextureCoord;
	float nrOfItterations = 0;
	
	// Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	int pixelMove = 2;
	// Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	for(uint i=0; i < boxWidth ; i++)
	{
		offset.x = ((float)i - pixelsFromCenter) * pixelMove * dx;
		SampledTextureCoord.x =  input.TexCoord.x + offset.x;
		// ensure that you stayon the texture
		if(SampledTextureCoord.x >= 0.f && SampledTextureCoord.x <= 1.f)
		{
			for(uint j=0; j < boxWidth ; j++)
			{
				offset.y = ((float)j * - pixelsFromCenter) * pixelMove * dy;
				SampledTextureCoord.y =  input.TexCoord.y + offset.y;
				// ensure that you stayon the texture
				if(SampledTextureCoord.y >= 0.f && SampledTextureCoord.y <= 1.f)
				{
					color += gTexture.Sample(samPoint, SampledTextureCoord).rgb;
					// manually count the itterations for borders
					++nrOfItterations;
				}
				
			}
		}

	}
	

	// Step 4: Divide the final color by the number of passes (in this case 5*5)
	color /= nrOfItterations;
	// Step 5: return the final color
	return float4(color, 1.0f);
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}