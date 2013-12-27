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


#ifndef __DAVAENGINE_RENDER_BASE_H__
#define __DAVAENGINE_RENDER_BASE_H__

#include "Base/BaseTypes.h"
#include "DAVAConfig.h"

/**
	\defgroup render Rendering abstraction
	\defgroup render_2d 2D Rendering
	\defgroup render_3d 3D Rendering
*/

/*
 Boroda: Я не знаю почему но <glew/gl.h> при включение из .mm файлов выдает ошибку как будто кто-то уже подключил 
 стандартный GL хотя в проекте нигде нет на него упоминаний. Есть подозрение что какой-то конфликт с внутренностями ObjC
 Как обойти пока непонятно - я сделал этот хедер чтобы включать его только там где это реально надо. 
 
 Раньше это лежало в BaseTypes.h
 */

#ifdef __DAVAENGINE_IPHONE__
	#define __DAVAENGINE_OPENGL__
	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
#elif defined(__DAVAENGINE_MACOS__)
	#define __DAVAENGINE_OPENGL__
	//	#include <GL/glew.h>
    #ifdef __DAVAENGINE_MACOS_VERSION_10_6__
        #include "OpenGL/gl.h"
    #endif //#ifdef __DAVAENGINE_MACOS_VERSION_10_6__

	#include <OpenGL/OpenGL.h>
	//	#include <GLUT/glut.h>
#elif defined(__DAVAENGINE_WIN32__)
 	#if defined(__DAVAENGINE_RENDER_AUTOCONFIG__)
 	#define __DAVAENGINE_DIRECTX9__
//	#define __DAVAENGINE_OPENGL__
 	#endif 

	#if defined(__DAVAENGINE_OPENGL__)
		#include <GL/glew.h>
		#include <direct.h>
	#elif defined(__DAVAENGINE_DIRECTX9__)
		// TODO: right now Sprite code do not compiles without GL types. Fix that. 
		#include <GL/gl.h>
		// #include <direct.h>
		
		#define D3D_DEBUG_INFO
		#include <d3d9.h>
		#include <dxerr.h>
#endif 
#elif defined(__DAVAENGINE_ANDROID__)
	#define __DAVAENGINE_OPENGL__
	#include <GLES/gl.h>
	#include <GLES/glext.h>
 	#include <GLES2/gl2.h>
 	#include <GLES2/gl2ext.h>

#else //PLATFORMS
	//other platforms
#endif//PLATFORMS 


#if defined(__DAVAENGINE_OPENGL__)
#include "Render/OGLHelpers.h"
#elif defined(__DAVAENGINE_DIRECTX9__)
#include "Render/D3D9Helpers.h"
#endif 


namespace DAVA
{

enum eBlendMode
{
	BLEND_NONE = 0,				// blend mode not defined
	BLEND_ZERO,
	BLEND_ONE,
	BLEND_DST_COLOR,
	BLEND_ONE_MINUS_DST_COLOR,
	BLEND_SRC_ALPHA,
	BLEND_ONE_MINUS_SRC_ALPHA,
	BLEND_DST_ALPHA,
	BLEND_ONE_MINUS_DST_ALPHA,
	BLEND_SRC_ALPHA_SATURATE,
	BLEND_SRC_COLOR,
	BLEND_ONE_MINUS_SRC_COLOR,

	BLEND_MODE_COUNT,
};

extern const String BLEND_MODE_NAMES[BLEND_MODE_COUNT];

enum PixelFormat
{
    FORMAT_INVALID = 0,
    FORMAT_RGBA8888 = 1,
    FORMAT_RGBA5551,
    FORMAT_RGBA4444,
    FORMAT_RGB888,
    FORMAT_RGB565,
    FORMAT_A8,
    FORMAT_A16,
    FORMAT_PVR4,
    FORMAT_PVR2,
    
    FORMAT_RGBA16161616,
    FORMAT_RGBA32323232,

    FORMAT_DXT1,
    FORMAT_DXT1NM,
    FORMAT_DXT1A,
    FORMAT_DXT3,
    FORMAT_DXT5,
    FORMAT_DXT5NM,
	
	FORMAT_ETC1,
	
	FORMAT_ATC_RGB,
	FORMAT_ATC_RGBA_EXPLICIT_ALPHA,
    FORMAT_ATC_RGBA_INTERPOLATED_ALPHA,

    FORMAT_COUNT,
    FORMAT_CLOSEST = 256
};
    
struct PixelFormatDescriptor
{
    GLenum format;
    GLenum internalformat;
    GLenum type;
    
    String name;
    int32 pixelSize;
    PixelFormat formatID;
    
    PixelFormatDescriptor()
    :   format(0), internalformat(0), type(0), pixelSize(0), formatID(FORMAT_INVALID)
    {
        
    }
};

    
enum eGPUFamily
{
    GPU_UNKNOWN = -1,
    
    GPU_POWERVR_IOS     =   0,
    GPU_POWERVR_ANDROID,
    GPU_TEGRA,
    GPU_MALI,
    GPU_ADRENO,
    
    GPU_FAMILY_COUNT
};
    
#if defined(__DAVAENGINE_OPENGL__)
extern const GLint BLEND_MODE_MAP[BLEND_MODE_COUNT];
#elif defined(__DAVAENGINE_DIRECTX9__)
extern const GLint BLEND_MODE_MAP[BLEND_MODE_COUNT];
#endif
  
enum eCmpFunc
{
    CMP_NEVER = 0,   // Never passes.
    CMP_LESS,    // Passes if the incoming value is less than the reference value.
    CMP_EQUAL,   // Passes if the incoming value is equal to the reference value.
    CMP_LEQUAL,  // Passes if the incoming value is less than or equal to the reference value.
    CMP_GREATER, // Passes if the incoming value is greater than the reference value.
    CMP_NOTEQUAL, // Passes if the incoming value is not equal to the reference value.
    CMP_GEQUAL,   // Passes if the incoming value is greater than or equal to the reference value.
    CMP_ALWAYS,
    CMP_TEST_MODE_COUNT, 
};

extern const String CMP_FUNC_NAMES[CMP_TEST_MODE_COUNT];

#if defined(__DAVAENGINE_OPENGL__)
extern const GLint COMPARE_FUNCTION_MAP[CMP_TEST_MODE_COUNT];
#elif defined(__DAVAENGINE_DIRECTX9__)  
extern const GLint COMPARE_FUNCTION_MAP[CMP_TEST_MODE_COUNT];
#endif
    
enum eVertexDataType
{
	TYPE_FLOAT = 0,
	TYPE_UNSIGNED_BYTE,

	TYPE_COUNT
};
    
#if defined(__DAVAENGINE_OPENGL__)
    static const GLint VERTEX_DATA_TYPE_TO_GL[TYPE_COUNT] = {GL_FLOAT, GL_UNSIGNED_BYTE};
#endif
    
enum eIndexDataType
{
    TYPE_UNSIGNED_SHORT = 0,
};

#if defined(__DAVAENGINE_OPENGL__)
    static const GLint INDEX_DATA_TYPE_TO_GL[TYPE_COUNT] = {GL_UNSIGNED_SHORT};
#endif

enum eIndexFormat
{
	EIF_16 = 0x0,
	EIF_32 = 0x1,
};
    
static const int32 INDEX_FORMAT_SIZE[2] = {2, 4};
    
enum eFace
{
    FACE_FRONT = 0,
    FACE_BACK,
    FACE_FRONT_AND_BACK,

    FACE_COUNT,
};

extern const String FACE_NAMES[FACE_COUNT];

#if defined(__DAVAENGINE_OPENGL__)
extern const GLint CULL_FACE_MAP[FACE_COUNT];
#elif defined(__DAVAENGINE_DIRECTX9__) 
extern const int32 CULL_FACE_MAP[FACE_COUNT];
#endif


enum eStencilOp
{
	STENCILOP_KEEP = 0,
	STENCILOP_ZERO,
	STENCILOP_REPLACE,
	STENCILOP_INCR,
	STENCILOP_INCR_WRAP,
	STENCILOP_DECR,
	STENCILOP_DECR_WRAP,
	STENCILOP_INVERT,

	STENCILOP_COUNT
};

extern const String STENCIL_OP_NAMES[STENCILOP_COUNT];

#if defined(__DAVAENGINE_OPENGL__)
extern const GLint STENCIL_OP_MAP[STENCILOP_COUNT];
#elif defined(__DAVAENGINE_DIRECTX9__) 
extern const int32 STENCIL_OP_MAP[STENCILOP_COUNT];
#endif

    
enum TextureWrap
{
    WRAP_CLAMP_TO_EDGE = 0,
    WRAP_REPEAT,
    
    WRAP_COUNT
};

#if defined(__DAVAENGINE_OPENGL__)
    extern const GLint TEXTURE_WRAP_MAP[WRAP_COUNT];
#elif defined(__DAVAENGINE_DIRECTX9__)
    extern const int32 TEXTURE_WRAP_MAP[WRAP_COUNT];
#endif

    
enum TextureFilter
{
    FILTER_NEAREST  = 0,
    FILTER_LINEAR,
    
    FILTER_NEAREST_MIPMAP_NEAREST,
    FILTER_LINEAR_MIPMAP_NEAREST,
    FILTER_NEAREST_MIPMAP_LINEAR,
    FILTER_LINEAR_MIPMAP_LINEAR,
    
    FILTER_COUNT
};

#if defined(__DAVAENGINE_OPENGL__)
    extern const GLint TEXTURE_FILTER_MAP[FILTER_COUNT];
#elif defined(__DAVAENGINE_DIRECTX9__)
    extern const int32 TEXTURE_FILTER_MAP[FILTER_COUNT];
#endif
    
    
    
enum eFillMode
{
	FILLMODE_POINT,
	FILLMODE_WIREFRAME,
	FILLMODE_SOLID,

	FILLMODE_COUNT
};

extern const String FILL_MODE_NAMES[FILLMODE_COUNT];

#if defined(__DAVAENGINE_OPENGL__) && (defined(__DAVAENGINE_MACOS__) || defined (__DAVAENGINE_WIN32__))
extern const GLint FILLMODE_MAP[FILLMODE_COUNT];
#elif defined(__DAVAENGINE_DIRECTX9__) 
extern const int32 FILLMODE_MAP[FILLMODE_COUNT];
#endif
    
enum ePrimitiveType
{
	PRIMITIVETYPE_POINTLIST = 0,
	PRIMITIVETYPE_LINELIST,
	PRIMITIVETYPE_LINESTRIP,
	PRIMITIVETYPE_TRIANGLELIST,
	PRIMITIVETYPE_TRIANGLESTRIP,
	PRIMITIVETYPE_TRIANGLEFAN,

	PRIMITIVETYPE_COUNT
};

    
// TODO: we have same structs & functions in PolygonGroup -- we should find a right place for them
enum eVertexFormat
{
    EVF_VERTEX			= 1,
    EVF_NORMAL			= 2,
    EVF_COLOR			= 4,
    EVF_TEXCOORD0		= 8,
    EVF_TEXCOORD1		= 16,
    EVF_TEXCOORD2		= 32,
    EVF_TEXCOORD3		= 64,
    EVF_TANGENT			= 128,
    EVF_BINORMAL		= 256,
    EVF_JOINTWEIGHT		= 512,
	EVF_TIME			= 1024,
	EVF_CUBETEXCOORD0	= 2048,
    EVF_CUBETEXCOORD1	= 4096,
    EVF_CUBETEXCOORD2	= 8192,
    EVF_CUBETEXCOORD3	= 16384,	
    EVF_LOWER_BIT		= EVF_VERTEX,
    EVF_HIGHER_BIT		= EVF_TIME, 
    EVF_NEXT_AFTER_HIGHER_BIT
    = (EVF_HIGHER_BIT << 1),
    EVF_FORCE_DWORD     = 0x7fffffff,
};
enum
{
    VERTEX_FORMAT_STREAM_MAX_COUNT = 11
};

inline int32 GetTexCoordCount(int32 vertexFormat)
{
	int32 ret = 0;
	for(int32 i = EVF_TEXCOORD0; i < EVF_TEXCOORD3+1; i = (i << 1))
	{
		if(vertexFormat & i)
		{
			ret++;
		}
	}

	return ret;
}
	
inline int32 GetCubeTexCoordCount(int32 vertexFormat)
{
	int32 ret = 0;
	for(int32 i = EVF_CUBETEXCOORD0; i < EVF_CUBETEXCOORD3+1; i = (i << 1))
	{
		if(vertexFormat & i)
		{
			ret++;
		}
	}
		
	return ret;
}

    
    
inline int32 GetVertexSize(int32 flags)
{
    int32 size = 0;
    if (flags & EVF_VERTEX) size += 3 * sizeof(float32);
    if (flags & EVF_NORMAL) size += 3 * sizeof(float32);
    if (flags & EVF_COLOR) size += 4;
    if (flags & EVF_TEXCOORD0) size += 2 * sizeof(float32);
    if (flags & EVF_TEXCOORD1) size += 2 * sizeof(float32);
    if (flags & EVF_TEXCOORD2) size += 2 * sizeof(float32);
    if (flags & EVF_TEXCOORD3) size += 2 * sizeof(float32);
    if (flags & EVF_TANGENT) size += 3 * sizeof(float32);
    if (flags & EVF_BINORMAL) size += 3 * sizeof(float32);
    
    if (flags & EVF_JOINTWEIGHT) size += 2 * sizeof(float32); // 4 * 3 + 4 * 3= 12 + 12
	
	if (flags & EVF_CUBETEXCOORD0) size += 3 * sizeof(float32);
    if (flags & EVF_CUBETEXCOORD1) size += 3 * sizeof(float32);
    if (flags & EVF_CUBETEXCOORD2) size += 3 * sizeof(float32);
    if (flags & EVF_CUBETEXCOORD3) size += 3 * sizeof(float32);

	if (flags & EVF_TIME) size+=sizeof(float32);
	
    return size;
}

eBlendMode GetBlendModeByName(const String & blendStr);
eCmpFunc GetCmpFuncByName(const String & cmpFuncStr);
eFace GetFaceByName(const String & faceStr);
eStencilOp GetStencilOpByName(const String & stencilOpStr);
eFillMode GetFillModeByName(const String & fillModeStr);

class RenderGuard
{
public:	
	RenderGuard();
	~RenderGuard();
	
	void LowLevelRenderCall();
	
	bool wrongCall;
};
	
};

#if defined(__DAVAENGINE_DEBUG__)
#define RENDER_GUARD RenderGuard renderGuard;
#else
#define RENDER_GUARD
#endif




#endif // __DAVAENGINE_RENDER_BASE_H__