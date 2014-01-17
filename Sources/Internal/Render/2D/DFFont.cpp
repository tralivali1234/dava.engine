/*==================================================================================
 Copyright (c) 2008, binaryzebra
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the binaryzebra nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 =====================================================================================*/

#include "DFFont.h"

#include "Render/Texture.h"
#include "Render/RenderManager.h"
#include "Render/Shader.h"
#include "FileSystem/YamlParser.h"

namespace DAVA {

	DFFont::DFFont()
	{
		fontType = TYPE_DISTANCE;
		baseSize = 0;
		paddingLeft = paddingRight = paddingTop = paddingBottom = 0;
		lineHeight = 0;
		spread = 1.f;
	}
	
	DFFont::~DFFont()
	{
	}
	
	DFFont* DFFont::Create(const FilePath & path)
	{
		DFFont* font = new DFFont();
		
		if (!font->LoadConfig(path))
		{
			SafeRelease(font);
			return NULL;
		}
		
		return font;
	}
	
	Size2i DFFont::GetStringSize(const WideString & str, Vector<int32> *charSizes/* = 0*/) const
	{ 
		int32 charDrawed = 0;
		return DrawStringToBuffer(str, 0, 0, NULL, charDrawed, charSizes);
	}
	
	bool DFFont::IsCharAvaliable(char16 ch) const
	{
		CharsMap::const_iterator iter = chars.find(ch);
		return iter != chars.end();
	}
	
	uint32 DFFont::GetFontHeight() const
	{
		return (lineHeight - paddingTop - paddingBottom) * GetSizeScale();
	}
	
	Font * DFFont::Clone() const
	{
		DFFont* dfFont = new DFFont();
		dfFont->chars = chars;
		dfFont->baseSize = baseSize;
		dfFont->paddingLeft = paddingLeft;
		dfFont->paddingRight = paddingRight;
		dfFont->paddingTop = paddingTop;
		dfFont->paddingBottom = paddingBottom;
		dfFont->lineHeight = lineHeight;
		dfFont->spread = spread;
		dfFont->configPath = configPath;

		return dfFont;
	}
	
	bool DFFont::IsEqual(const Font *font) const
	{
		if (!Font::IsEqual(font))
			return false;
		
		DFFont* dfFont = (DFFont*) font;
		if (dfFont->configPath != configPath)
			return false;
		
		return true;
	}
	
	Size2i DFFont::DrawStringToBuffer(const WideString & str,
									  int32 xOffset,
									  int32 yOffset,
									  DFFontVertex* vertexBuffer,
									  int32& charDrawed,
									  Vector<int32> *charSizes /*= NULL*/) const
	{
		uint32 vertexAdded = 0;
		charDrawed = 0;
		
		float32 lastX = xOffset;
		float32 lastY = 0;
		float32 sizeScale = GetSizeScale();
		lastX -= paddingLeft * sizeScale;
		
		uint32 strLength = str.length();
		for (uint32 charPos = 0; charPos < strLength; ++charPos)
		{
			char16 charId = str.at(charPos);
			CharsMap::const_iterator iter = chars.find(charId);
			if (iter == chars.end())
			{
				int32 charSize = 0;
				if (charId == L' ')
					charSize = size / 2;
				
				if (charSizes)
					charSizes->push_back(charSize);
				
				lastX += charSize;
				
				continue;
			}
			
			const CharDescription& charDescription = iter->second;
			
			float32 nextKerning = 0;
			if (charPos + 1 < strLength)
			{
				Map<int32, float32>::const_iterator iter = charDescription.kerning.find(str.at(charPos + 1));
				if (iter != charDescription.kerning.end())
				{
					nextKerning = iter->second;
				}
			}
			
			float32 width = (charDescription.width) * sizeScale;
			
			float32 startHeight = charDescription.yOffset * sizeScale;
			float32 fullHeight = (charDescription.height + charDescription.yOffset) * sizeScale;
			
			startHeight += yOffset;
			fullHeight += yOffset;
			
			if (vertexBuffer)
			{
				vertexBuffer[vertexAdded].position.x = lastX;
				vertexBuffer[vertexAdded].position.y = startHeight;
				vertexBuffer[vertexAdded].position.z = 0;
				vertexBuffer[vertexAdded].texCoord.x = charDescription.u;
				vertexBuffer[vertexAdded].texCoord.y = charDescription.v;
				
				vertexBuffer[vertexAdded + 1].position.x = lastX + width;
				vertexBuffer[vertexAdded + 1].position.y = startHeight;
				vertexBuffer[vertexAdded + 1].position.z = 0;
				vertexBuffer[vertexAdded + 1].texCoord.x = charDescription.u2;
				vertexBuffer[vertexAdded + 1].texCoord.y = charDescription.v;
				
				vertexBuffer[vertexAdded + 2].position.x = lastX + width;
				vertexBuffer[vertexAdded + 2].position.y = fullHeight;
				vertexBuffer[vertexAdded + 2].position.z = 0;
				vertexBuffer[vertexAdded + 2].texCoord.x = charDescription.u2;
				vertexBuffer[vertexAdded + 2].texCoord.y = charDescription.v2;
				
				vertexBuffer[vertexAdded + 3].position.x = lastX;
				vertexBuffer[vertexAdded + 3].position.y = fullHeight;
				vertexBuffer[vertexAdded + 3].position.z = 0;
				vertexBuffer[vertexAdded + 3].texCoord.x = charDescription.u;
				vertexBuffer[vertexAdded + 3].texCoord.y = charDescription.v2;
				vertexAdded += 4;
			}
				
			int32 charWidth = (charDescription.width - paddingLeft - paddingRight + nextKerning) * sizeScale;
			if (charSizes)
				charSizes->push_back(charWidth);
			if (charPos < strLength - 1)
				lastX += charWidth;
			
			charDrawed++;
		}
		lastX += (paddingLeft + paddingRight) * sizeScale;
		lastY += yOffset + GetFontHeight();

		return Size2i(lastX, lastY);
	}
	
	float32 DFFont::GetSpread() const
	{
		return 0.25f / (spread * GetSizeScale());
	}
	
	float32 DFFont::GetSizeScale() const
	{
		return size / baseSize;
	}
	
	bool DFFont::LoadConfig(const DAVA::FilePath &path)
	{
		YamlParser* parser = YamlParser::Create(path.GetAbsolutePathname());
		if (!parser)
			return false;
		
		configPath = path;
		
		YamlNode* rootNode = parser->GetRootNode();
		const YamlNode* configNode = rootNode->Get("font");
		if (!configNode)
			return false;
		const YamlNode* charsNode = configNode->Get("chars");
		if (!charsNode)
			return false;
		
		baseSize = configNode->Get("size")->AsFloat();
		const YamlNode* paddingTop = configNode->Get("padding_top");
		if (paddingTop)
			this->paddingTop = paddingTop->AsFloat();
		const YamlNode* paddingLeft = configNode->Get("padding_left");
		if (paddingLeft)
			this->paddingLeft = paddingLeft->AsFloat();
		const YamlNode* paddingBottom = configNode->Get("padding_bottop");
		if (paddingBottom)
			this->paddingBottom = paddingBottom->AsFloat();
		const YamlNode* paddingRight = configNode->Get("padding_right");
		if (paddingRight)
			this->paddingRight = paddingRight->AsFloat();
		const YamlNode* lineHeight = configNode->Get("lineHeight");
		if (lineHeight)
			this->lineHeight = lineHeight->AsFloat();
		const YamlNode* spread = configNode->Get("spread");
		if (spread)
			this->spread = spread->AsFloat();
		
		const MultiMap<String, YamlNode*> charsMap = charsNode->AsMap();
		for (MultiMap<String, YamlNode*>::const_iterator iter = charsMap.begin(); iter != charsMap.end(); ++iter)
		{
			char16 charId = atoi(iter->first.c_str());
			CharDescription charDescription;
			charDescription.height = iter->second->Get("height")->AsFloat();
			charDescription.width = iter->second->Get("width")->AsFloat();
			charDescription.xOffset = iter->second->Get("xoffset")->AsFloat();
			charDescription.yOffset = iter->second->Get("yoffset")->AsFloat();
			charDescription.xAdvance = iter->second->Get("xadvance")->AsFloat();
			charDescription.u = iter->second->Get("u")->AsFloat();
			charDescription.u2 = iter->second->Get("u2")->AsFloat();
			charDescription.v = iter->second->Get("v")->AsFloat();
			charDescription.v2 = iter->second->Get("v2")->AsFloat();
			
			chars[charId] = charDescription;
		}
		
		const YamlNode* kerningNode = configNode->Get("kerning");
		if (kerningNode)
		{
			const MultiMap<String, YamlNode*> kerningMap = kerningNode->AsMap();
			for (MultiMap<String, YamlNode*>::const_iterator iter = kerningMap.begin(); iter != kerningMap.end(); ++iter)
			{
				int32 charId = atoi(iter->first.c_str());
				CharsMap::iterator charIter = chars.find(charId);
				if (charIter == chars.end())
					continue;
				
				const MultiMap<String, YamlNode*> charKerningMap = iter->second->AsMap();
				for (MultiMap<String, YamlNode*>::const_iterator iter = charKerningMap.begin(); iter != charKerningMap.end(); ++iter)
				{
					int32 secondCharId = atoi(iter->first.c_str());
					float32 kerning = iter->second->AsFloat();
					charIter->second.kerning[secondCharId] = kerning;
				}
			}
		}
		
		return true;
	}
	
	FilePath DFFont::GetTexturePath() const
	{
		String fileName = configPath.GetFilename();
		String fileExtension = configPath.GetExtension();
		fileName.replace(fileName.length() - fileExtension.length(), fileExtension.length(), ".tex");
		FilePath path = configPath.GetDirectory() + fileName;
		return path;
	}
		
	YamlNode * DFFont::SaveToYamlNode() const
	{
		YamlNode *node = Font::SaveToYamlNode();
		//Type
		node->Set("type", "DFFont");
		
		String pathname = configPath.GetFrameworkPath();
		node->Set("name", pathname);
		
		return node;
	}
	
	const FilePath & DFFont::GetFontPath() const
	{
		return configPath;
	}
	
}