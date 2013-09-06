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



#ifndef __RESOURCEEDITORQT__HEIGHTMAPEDITORCOMMANDS2__
#define __RESOURCEEDITORQT__HEIGHTMAPEDITORCOMMANDS2__

#include "Commands2/Command2.h"
#include "Commands2/CommandAction.h"
#include "DAVAEngine.h"

using namespace DAVA;

class HeightmapProxy;
class LandscapeProxy;
class SceneEditor2;

class ActionEnableHeightmapEditor: public CommandAction
{
public:
	ActionEnableHeightmapEditor(SceneEditor2* forSceneEditor);
	
protected:
	SceneEditor2* sceneEditor;
	
	virtual void Redo();
};

class ActionDisableHeightmapEditor: public CommandAction
{
public:
	ActionDisableHeightmapEditor(SceneEditor2* forSceneEditor);
	
protected:
	SceneEditor2* sceneEditor;
	
	virtual void Redo();
};


class ModifyHeightmapCommand: public Command2
{
public:
	ModifyHeightmapCommand(HeightmapProxy* heightmapProxy,
						   Heightmap* originalHeightmap,
						   const Rect& updatedRect);
	virtual ~ModifyHeightmapCommand();

protected:
	HeightmapProxy* heightmapProxy;
	uint16* undoRegion;
	uint16* redoRegion;
	Rect updatedRect;

	virtual void Redo();
	virtual void Undo();

	virtual Entity* GetEntity() const;

	uint16* GetHeightmapRegion(Heightmap* heightmap);
	void ApplyHeightmapRegion(uint16* region);
};

class CopyPasteHeightmapCommand: public Command2
{
public:
	CopyPasteHeightmapCommand(bool heightmapModified,
							  bool tilemaskModified,
							  HeightmapProxy* heightmapProxy,
							  Heightmap* originalHeightmap,
							  const Rect& heightmapUpdatedRect,
							  LandscapeProxy* landscapeProxy,
							  Image* originalTilemaskImage,
							  const Rect& tilemaskUpdatedRect);

	~CopyPasteHeightmapCommand();

protected:
	HeightmapProxy* heightmapProxy;
	LandscapeProxy* landscapeProxy;

	bool heightmapModified;
	bool tilemaskModified;

	uint16* undoRegion;
	uint16* redoRegion;

	Image* undoImageMask;
	Image* redoImageMask;

	Rect heightmapUpdatedRect;
	Rect tilemaskUpdatedRect;

	bool firstRun;

	virtual void Redo();
	virtual void Undo();

	virtual Entity* GetEntity() const;

	uint16* GetHeightmapRegion(Heightmap* heightmap, const Rect& updatedRect);
	void ApplyHeightmapRegion(uint16* region, const Rect& updatedRect);
	Sprite* ApplyImageToTexture(Image* image,
								Texture* texture,
								const Rect& updatedRect);
};

#endif /* defined(__RESOURCEEDITORQT__HEIGHTMAPEDITORCOMMANDS2__) */
