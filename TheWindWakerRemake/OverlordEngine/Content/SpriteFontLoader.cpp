#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...
	if (pReader->Read<char>() != 'B' || pReader->Read<char>() != 'M' || pReader->Read<char>() != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	int versionNumber = (int)pReader->Read<char>();
	const int currentVersion = 3;
	if (currentVersion > versionNumber)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported!\n Asset Path: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	int blockId = (int)pReader->Read<char>();
	int blockSize = pReader->Read<int>();
	if (blockId != 1)
	{
		Logger::LogError(L"BlockId mismatach!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}
	//Retrieve the FontSize [fontDesc.fontSize]
	fontDesc.fontSize = pReader->Read<short>();
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);
	//Retrieve the FontName [fontDesc.fontName]
	fontDesc.fontName = pReader->ReadNullString();


	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = (int)pReader->Read<char>();
	blockSize = pReader->Read<int>();
	if (blockId != 2)
	{
		Logger::LogError(L"BlockId mismatach!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();
	//Retrieve PageCount
	int pageCount = (int)pReader->Read<short>();
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	if (pageCount > 1)
	{
		Logger::LogError(L"Only one texture per font is allowed!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
	}

	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(5);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = (int)pReader->Read<char>();
	blockSize = pReader->Read<int>();
	if (blockId != 3)
	{
		Logger::LogError(L"BlockId mismatach!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}
	//Retrieve the PageName (BinaryReader::ReadNullString)
	// Note: If you want to use multiple pages, be sure to itterate multiple tomes over the page names.
	std::wstring pageName = pReader->ReadNullString();
	
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	std::wstring fullPath = loadInfo.assetFullPath.parent_path().append(pageName);
	fontDesc.pTexture = ContentManager::Load<TextureData>(fullPath);

	//**********
	// BLOCK 4 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = (int)pReader->Read<char>();
	blockSize = pReader->Read<int>();
	if (blockId != 4)
	{
		Logger::LogError(L"BlockId mismatach!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}
	//Retrieve Character Count (see documentation)
	const size_t charInfoSize = 20;
	const size_t numChars = blockSize / charInfoSize;
	//Create loop for Character Count, and:
	
	wchar_t characterId{};
	FontMetric fontMetric{};
	for (size_t i = 0; i < numChars; i++)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		characterId = (wchar_t)pReader->Read<int>();
		//> Create instance of FontMetric (struct)
		
		//	> Set Character (CharacterId) [FontMetric::character]
		fontMetric.character = characterId;
		//	> Retrieve Xposition (store Local)
		float xPos = (float)pReader->Read<short>();
		//	> Retrieve Yposition (store Local)
		float yPos = (float)pReader->Read<short>();
		//	> Retrieve & Set Width [FontMetric::width]
		fontMetric.width = pReader->Read<unsigned short>();
		//	> Retrieve & Set Height [FontMetric::height]
		fontMetric.height = pReader->Read<unsigned short>();
		//	> Retrieve & Set OffsetX [FontMetric::offsetX]
		fontMetric.offsetX = pReader->Read<short>();
		//	> Retrieve & Set OffsetY [FontMetric::offsetY]
		fontMetric.offsetY = pReader->Read<short>();
		//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
		fontMetric.advanceX = pReader->Read<short>();
		//	> Retrieve & Set Page [FontMetric::page]
		fontMetric.page = pReader->Read<unsigned char>();
		//	> Retrieve Channel (BITFIELD!!!) 
		//		> See documentation for BitField meaning [FontMetrix::channel]

		int channel =  (int)pReader->Read<char>();
		switch (channel)
		{
		case 1:// Blue
			fontMetric.channel = 2;
			break;
		case 2: // Green
			fontMetric.channel = 1;
			break;
		case 4: // Red
			fontMetric.channel = 0;
			break;
		case 8: // Alpha
			fontMetric.channel = 3;
			break;
		default:
			Logger::LogError(L"Invalid channel!\nAsset Path: \'{}\'", loadInfo.assetSubPath);
			break;
		};
		
		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		fontMetric.texCoord = XMFLOAT2( xPos / float(fontDesc.textureWidth), yPos / float(fontDesc.textureHeight));
		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		fontDesc.metrics[characterId] = fontMetric;
		//(loop restarts till all metrics are parsed)
	}
	

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
