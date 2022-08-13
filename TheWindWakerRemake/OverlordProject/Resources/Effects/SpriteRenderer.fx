float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
    DepthEnable = FALSE;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
    uint TextureId : TEXCOORD0; //Can be ignored
    float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
    float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
	// pivot offset influence
	float3 pivotOffset3 = float3(pivotOffset.x, pivotOffset.y, 0.f);
	pos -= pivotOffset3;
    if (rotation != 0.f)
    {
		//Step 3.
		//Do rotation calculations
		//Transform to origin
		pos = float3(pos.x - offset.x , pos.y - offset.y , pos.z);
		//Rotate
		float rotx = (pos.x * rotCosSin.x) -  (pos.y * rotCosSin.y);
		float roty = (pos.y * rotCosSin.x) + (pos.x * rotCosSin.y);
		pos = float3(rotx, roty, pos.z);
		//Retransform to initial position
		pos = float3(pos.x + offset.x , pos.y + offset.y , pos.z);
    }

	//Geometry Vertex Output
    GS_DATA geomData = (GS_DATA) 0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
	//Extract the position data from the VS_DATA vertex struct
    float3 position = float3(vertex[0].TransformData.x ,vertex[0].TransformData.y,vertex[0].TransformData.z);
	//Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
    float2 offset = float2(vertex[0].TransformData.x,vertex[0].TransformData.y); 
	//Extract the rotation data from the VS_DATA vertex struct
    float rotation = vertex[0].TransformData.w;  
	//Extract the pivot data from the VS_DATA vertex struct
    float2 pivot = vertex[0].TransformData2.xy; 
	//Extract the scale data from the VS_DATA vertex struct
    float2 scale = float2(vertex[0].TransformData2.z,vertex[0].TransformData2.w); 
	//Initial Texture Coordinate
    float2 texCoord = float2(0.f, 0.f); 
	
	// Calculate the rotCosSin
	float2 rotCosSin = float2(1.f,0.f);
	if(rotation != 0)
	{
		rotCosSin = float2(cos(rotation) , sin(rotation));
	}
	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB

	pivot = pivot * (gTextureSize * scale);

	//VERTEX 1 [LT]
	float3 vertexPos = position;
    CreateVertex(triStream, vertexPos, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivot);

	//VERTEX 2 [RT]
	vertexPos = position + float3(gTextureSize.x * scale.x, 0, 0);
	texCoord = float2(1.f, 0.f); 
    CreateVertex(triStream, vertexPos, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivot);

	//VERTEX 3 [LB]
	vertexPos = position + float3(0, gTextureSize.y * scale.y, 0);
	texCoord = float2(0.f, 1.f); 
    CreateVertex(triStream, vertexPos, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivot);

	//VERTEX 4 [RB]
	vertexPos = position + float3(gTextureSize.x * scale.x, gTextureSize.y * scale.y, 0);
	texCoord = float2(1.f, 1.f); 
    CreateVertex(triStream, vertexPos, vertex[0].Color, texCoord, rotation, rotCosSin, offset, pivot);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
}

// Default Technique
technique10 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
