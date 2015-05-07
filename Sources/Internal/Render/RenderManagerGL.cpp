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


#include "Render/RenderBase.h"
#include "Render/RenderManager.h"
#include "Render/Texture.h"
#include "Utils/Utils.h"
#include "Core/Core.h"
#include "Render/OGLHelpers.h"
#include "Render/Shader.h"
#include "Render/Image/Image.h"
#include "Render/Image/ImageSystem.h"
#include "FileSystem/FileSystem.h"
#include "Utils/StringFormat.h"
#include "Render/PixelFormatDescriptor.h"

#ifdef __DAVAENGINE_OPENGL__

namespace DAVA
{

#if defined(__DAVAENGINE_WINDOWS_STORE__)
using namespace Platform;
using namespace Windows::Foundation::Collections;

bool RenderManager::Create(Windows::UI::Core::CoreWindow^ window)
{
	const EGLint configAttributes[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};

	const EGLint contextAttributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	const EGLint surfaceAttributes[] =
	{
		// EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER is part of the same optimization as EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER (see above).
		// If you have compilation issues with it then please update your Visual Studio templates.
		EGL_ANGLE_SURFACE_RENDER_TO_BACK_BUFFER, EGL_TRUE,
		EGL_NONE
	};

	const EGLint defaultDisplayAttributes[] =
	{
		// These are the default display attributes, used to request ANGLE's D3D11 renderer.
		// eglInitialize will only succeed with these attributes if the hardware supports D3D11 Feature Level 10_0+.
		EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,

		// EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER is an optimization that can have large performance benefits on mobile devices.
		// Its syntax is subject to change, though. Please update your Visual Studio templates if you experience compilation issues with it.
		EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,

		// EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE is an option that enables ANGLE to automatically call 
		// the IDXGIDevice3::Trim method on behalf of the application when it gets suspended. 
		// Calling IDXGIDevice3::Trim when an application is suspended is a Windows Store application certification requirement.
		EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
		EGL_NONE,
	};

	const EGLint fl9_3DisplayAttributes[] =
	{
		// These can be used to request ANGLE's D3D11 renderer, with D3D11 Feature Level 9_3.
		// These attributes are used if the call to eglInitialize fails with the default display attributes.
		EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
		EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 9,
		EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 3,
		EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
		EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
		EGL_NONE,
	};

	const EGLint warpDisplayAttributes[] =
	{
		// These attributes can be used to request D3D11 WARP.
		// They are used if eglInitialize fails with both the default display attributes and the 9_3 display attributes.
		EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
		EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_DEVICE_TYPE_WARP_ANGLE,
		EGL_ANGLE_DISPLAY_ALLOW_RENDER_TO_BACK_BUFFER, EGL_TRUE,
		EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
		EGL_NONE,
	};

	EGLConfig config = NULL;

	// eglGetPlatformDisplayEXT is an alternative to eglGetDisplay. It allows us to pass in display attributes, used to configure D3D11.
	PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(eglGetProcAddress("eglGetPlatformDisplayEXT"));
	if (!eglGetPlatformDisplayEXT)
	{
		throw Exception::CreateException(E_FAIL, L"Failed to get function eglGetPlatformDisplayEXT");
	}

	//
	// To initialize the display, we make three sets of calls to eglGetPlatformDisplayEXT and eglInitialize, with varying 
	// parameters passed to eglGetPlatformDisplayEXT:
	// 1) The first calls uses "defaultDisplayAttributes" as a parameter. This corresponds to D3D11 Feature Level 10_0+.
	// 2) If eglInitialize fails for step 1 (e.g. because 10_0+ isn't supported by the default GPU), then we try again 
	//    using "fl9_3DisplayAttributes". This corresponds to D3D11 Feature Level 9_3.
	// 3) If eglInitialize fails for step 2 (e.g. because 9_3+ isn't supported by the default GPU), then we try again 
	//    using "warpDisplayAttributes".  This corresponds to D3D11 Feature Level 11_0 on WARP, a D3D11 software rasterizer.
	//

	// This tries to initialize EGL to D3D11 Feature Level 10_0+. See above comment for details.
	mEglDisplay = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, defaultDisplayAttributes);
	if (mEglDisplay == EGL_NO_DISPLAY)
	{
		throw Exception::CreateException(E_FAIL, L"Failed to get EGL display");
	}

	if (eglInitialize(mEglDisplay, NULL, NULL) == EGL_FALSE)
	{
		// This tries to initialize EGL to D3D11 Feature Level 9_3, if 10_0+ is unavailable (e.g. on some mobile devices).
		mEglDisplay = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, fl9_3DisplayAttributes);
		if (mEglDisplay == EGL_NO_DISPLAY)
		{
			throw Exception::CreateException(E_FAIL, L"Failed to get EGL display");
		}

		if (eglInitialize(mEglDisplay, NULL, NULL) == EGL_FALSE)
		{
			// This initializes EGL to D3D11 Feature Level 11_0 on WARP, if 9_3+ is unavailable on the default GPU.
			mEglDisplay = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, warpDisplayAttributes);
			if (mEglDisplay == EGL_NO_DISPLAY)
			{
				throw Exception::CreateException(E_FAIL, L"Failed to get EGL display");
			}

			if (eglInitialize(mEglDisplay, NULL, NULL) == EGL_FALSE)
			{
				// If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
				throw Exception::CreateException(E_FAIL, L"Failed to initialize EGL");
			}
		}
	}

	EGLint numConfigs = 0;
	if ((eglChooseConfig(mEglDisplay, configAttributes, &config, 1, &numConfigs) == EGL_FALSE) || (numConfigs == 0))
	{
		throw Exception::CreateException(E_FAIL, L"Failed to choose first EGLConfig");
	}

	// Create a PropertySet and initialize with the EGLNativeWindowType.
	PropertySet^ surfaceCreationProperties = ref new PropertySet();
	surfaceCreationProperties->Insert(ref new Platform::String(EGLNativeWindowTypeProperty), window);

	//
	// A Custom render surface size can be specified by uncommenting the following lines.
	// The render surface will be automatically scaled to fit the entire window.  Using a
	// smaller sized render surface can result in a performance gain.
	//
	//mCustomRenderSurfaceSize = Size(800, 600);
	//mUseCustomRenderSurfaceSize = true;
	//surfaceCreationProperties->Insert(ref new String(EGLRenderSurfaceSizeProperty), PropertyValue::CreateSize(mCustomRenderSurfaceSize));
	//

	mEglSurface = eglCreateWindowSurface(mEglDisplay, config, reinterpret_cast<IInspectable*>(surfaceCreationProperties), surfaceAttributes);
	if (mEglSurface == EGL_NO_SURFACE)
	{
		throw Exception::CreateException(E_FAIL, L"Failed to create EGL fullscreen surface");
	}

	mEglContext = eglCreateContext(mEglDisplay, config, EGL_NO_CONTEXT, contextAttributes);
	if (mEglContext == EGL_NO_CONTEXT)
	{
		throw Exception::CreateException(E_FAIL, L"Failed to create EGL context");
	}

	if (eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext) == EGL_FALSE)
	{
		throw Exception::CreateException(E_FAIL, L"Failed to make fullscreen EGLSurface current");
	}
	return true;
}

void RenderManager::Release()
{
	Singleton<RenderManager>::Release();

	if (mEglDisplay != EGL_NO_DISPLAY && mEglSurface != EGL_NO_SURFACE)
	{
		eglDestroySurface(mEglDisplay, mEglSurface);
		mEglSurface = EGL_NO_SURFACE;
	}

	if (mEglDisplay != EGL_NO_DISPLAY && mEglContext != EGL_NO_CONTEXT)
	{
		eglDestroyContext(mEglDisplay, mEglContext);
		mEglContext = EGL_NO_CONTEXT;
	}

	if (mEglDisplay != EGL_NO_DISPLAY)
	{
		eglTerminate(mEglDisplay);
		mEglDisplay = EGL_NO_DISPLAY;
	}
}

#elif defined(__DAVAENGINE_WINDOWS_DESKTOP__)

static HDC hDC = nullptr;
static HGLRC hRC = nullptr;
static HWND hWnd = nullptr;
static HINSTANCE hInstance = nullptr;

bool RenderManager::Create(HINSTANCE _hInstance, HWND _hWnd)
{
	hInstance = _hInstance;
	hWnd = _hWnd;

	hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat( hDC, &pfd );
	SetPixelFormat( hDC, iFormat, &pfd );

	hRC = wglCreateContext(hDC);

	renderContextId = (uint64)hRC;
	
	wglMakeCurrent(hDC, hRC);

	glewInit();

	return true;
}

void RenderManager::Release()
{
	Singleton<RenderManager>::Release();

	wglMakeCurrent(0, 0);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);	
}

bool RenderManager::ChangeDisplayMode(DisplayMode mode, bool isFullscreen)
{
	hardwareState.Reset(false);
	currentState.Reset(true);
	return true;
}	
	
#else //#if defined(__DAVAENGINE_WINDOWS_DESKTOP__)

bool RenderManager::Create()
{
	return true;
}
	
void RenderManager::Release()
{
	
}

#endif //#if defined(__DAVAENGINE_WINDOWS_DESKTOP__)

#if defined(__DAVAENGINE_ANDROID__)
typedef void (GL_APIENTRYP PFNGLDISCARDFRAMEBUFFEREXTPROC) (GLenum target, GLsizei numAttachments, const GLenum *attachments);
static PFNGLDISCARDFRAMEBUFFEREXTPROC glDiscardFramebufferEXT = NULL;
#endif

bool IsGLExtensionSupported(const String &extension)
{
    String::size_type spacePosition = extension.find(" ");
    if(String::npos != spacePosition || extension.empty())
    {
        /* Extension names should not have spaces. */
        Logger::FrameworkDebug("[IsGLExtensionSupported] extension %s isn't supported", extension.c_str());
        return false;
    }
    
    String extensions((const char8 *)glGetString(GL_EXTENSIONS));
    String::size_type extPosition = extensions.find(extension);
    return (String::npos != extPosition);
}

    
void RenderManager::DetectRenderingCapabilities()
{
#if defined(__DAVAENGINE_MACOS__)
	caps.isHardwareCursorSupported = true;
#elif defined (__DAVAENGINE_IPHONE__) || defined (__DAVAENGINE_ANDROID__)
	caps.isHardwareCursorSupported = false;
#endif

#if defined(__DAVAENGINE_IPHONE__)
    caps.isPVRTCSupported = IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc");
	caps.isPVRTC2Supported = IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc2");
    caps.isETCSupported = false;

	caps.isDXTSupported = false;
    caps.isBGRA8888Supported = IsGLExtensionSupported("GL_APPLE_texture_format_BGRA8888");
    caps.isFloat16Supported = IsGLExtensionSupported("GL_OES_texture_half_float");
    caps.isFloat32Supported = IsGLExtensionSupported("GL_OES_texture_float");
	caps.isATCSupported = IsGLExtensionSupported("GL_AMD_compressed_ATC_texture");
#elif defined(__DAVAENGINE_ANDROID__)
    //TODO: added correct
    caps.isPVRTCSupported = IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc");
	caps.isPVRTC2Supported = IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc2");
    caps.isETCSupported = IsGLExtensionSupported("GL_OES_compressed_ETC1_RGB8_texture");

	caps.isDXTSupported = IsGLExtensionSupported("GL_EXT_texture_compression_s3tc");
    caps.isBGRA8888Supported = false;
    caps.isFloat16Supported = IsGLExtensionSupported("GL_OES_texture_half_float");
    caps.isFloat32Supported = IsGLExtensionSupported("GL_OES_texture_float");
	caps.isATCSupported = IsGLExtensionSupported("GL_AMD_compressed_ATC_texture");
    caps.isGlDepth24Stencil8Supported = IsGLExtensionSupported("GL_DEPTH24_STENCIL8") || IsGLExtensionSupported("GL_OES_packed_depth_stencil");
    caps.isGlDepthNvNonLinearSupported = IsGLExtensionSupported("GL_DEPTH_COMPONENT16_NONLINEAR_NV");
    
    if (IsGLExtensionSupported("GL_EXT_discard_framebuffer"))
        glDiscardFramebufferEXT = (PFNGLDISCARDFRAMEBUFFEREXTPROC) eglGetProcAddress("glDiscardFramebufferEXT");

#   if (__ANDROID_API__ < 18)
    InitFakeOcclusion();
#   endif

#elif defined(__DAVAENGINE_MACOS__) || defined(__DAVAENGINE_WIN32__)

	caps.isPVRTCSupported = false;

#if defined(GL_COMPRESSED_RGB_S3TC_DXT1_EXT) || defined(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) || defined(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) || defined(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
	caps.isDXTSupported = IsGLExtensionSupported("GL_EXT_texture_compression_s3tc");
#else //DXT
	caps.isDXTSupported = false;
#endif //DXT

#if defined(GL_ETC1_RGB8_OES)
	caps.isETCSupported = IsGLExtensionSupported("GL_OES_compressed_ETC1_RGB8_texture");
#else //ETC1
	caps.isETCSupported = false;
#endif //ETC1

#if defined(GL_ATC_RGB_AMD) || defined(GL_ATC_RGBA_EXPLICIT_ALPHA_AMD) || defined(GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD)
	caps.isATCSupported = IsGLExtensionSupported("GL_AMD_compressed_ATC_texture");
#else //ATC
	caps.isATCSupported = false;
#endif //ATC

	caps.isBGRA8888Supported = IsGLExtensionSupported("GL_IMG_texture_format_BGRA8888");
    caps.isFloat16Supported = IsGLExtensionSupported("GL_ARB_half_float_pixel");
    caps.isFloat32Supported = IsGLExtensionSupported("GL_ARB_texture_float");
#endif

    caps.isOpenGLES3Supported = (renderer == Core::RENDERER_OPENGL_ES_3_0);

	PixelFormatDescriptor::InitializePixelFormatDescriptors();

    int maxVertexTextureUnits = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureUnits);
    caps.isVertexTextureUnitsSupported = (maxVertexTextureUnits > 0);
    caps.isFramebufferFetchSupported = IsGLExtensionSupported("GL_EXT_shader_framebuffer_fetch");
}

bool RenderManager::IsDeviceLost()
{
	return false;
}

void RenderManager::BeginFrame()
{
    stats.Clear();
    SetViewport(Rect(0, 0, (float32)frameBufferWidth, (float32)frameBufferHeight));
	
	Reset();
	isInsideDraw = true;
}

void RenderManager::EndFrame()
{
	isInsideDraw = false;
#if defined(__DAVAENGINE_WINDOWS_DESKTOP__)
	::SwapBuffers(hDC);
#endif //#if defined(__DAVAENGINE_WINDOWS_DESKTOP__)
	
	RENDER_VERIFY(;);	// verify at the end of the frame
    
    if(needGLScreenShot)
    {
        needGLScreenShot = false;
        MakeGLScreenShot();
    }
}
    
void RenderManager::MakeGLScreenShot()
{
    Logger::FrameworkDebug("RenderManager::MakeGLScreenShot");
#if defined(__DAVAENGINE_OPENGL__)
    if (!screenShotCallback)
    {
        Logger::FrameworkDebug("RenderManager::ScreenShot callback is empty.");
        return;
    }

    int32 width = frameBufferWidth;
    int32 height = frameBufferHeight;
    
    const PixelFormatDescriptor & formatDescriptor = PixelFormatDescriptor::GetPixelFormatDescriptor(FORMAT_RGBA8888);
    
    Logger::FrameworkDebug("RenderManager::MakeGLScreenShot w=%d h=%d", width, height);
    
    // picture is rotated (framebuffer coordinates start from bottom left)
    Image *image = Image::Create(width, height, formatDescriptor.formatID);
    uint8 *imageData = image->GetData();

    RENDER_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, fboViewRenderbuffer));
    
    RENDER_VERIFY(glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ));
	RENDER_VERIFY(glReadPixels(0, 0, width, height, formatDescriptor.format, formatDescriptor.type, (GLvoid *)imageData));

    image->FlipVertical();

	(*screenShotCallback)(image);
	SafeRelease(image);
    
#endif //#if defined(__DAVAENGINE_OPENGL__)
}
    
void RenderManager::SetViewport(const Rect & rect)
{
    Rect viewportRect = viewport = rect;

    if (currentRenderTarget == nullptr)
    {
        viewportRect.y = frameBufferHeight - viewportRect.y - viewportRect.dy;
    }
    
    RENDER_VERIFY(glViewport((int32)viewportRect.x, (int32)viewportRect.y, (int32)viewportRect.dx, (int32)viewportRect.dy));
}

// Viewport management
void RenderManager::SetCullOrder(eCullOrder cullOrder)
{
    glFrontFace(cullOrder);
}
    
void RenderManager::FlushState()
{
    currentState.Flush(&hardwareState);
}

void RenderManager::FlushState(RenderState * stateBlock)
{
	stateBlock->Flush(&hardwareState);
}

void RenderManager::HWDrawArrays(ePrimitiveType type, int32 first, int32 count)
{
	static const int32 types[PRIMITIVETYPE_COUNT] = 
	{
		GL_POINTS,			// 		PRIMITIVETYPE_POINTLIST = 0,
		GL_LINES,			// 		PRIMITIVETYPE_LINELIST,
		GL_LINE_STRIP,		// 		PRIMITIVETYPE_LINESTRIP,
		GL_TRIANGLES,		// 		PRIMITIVETYPE_TRIANGLELIST,
		GL_TRIANGLE_STRIP,	// 		PRIMITIVETYPE_TRIANGLESTRIP,
		GL_TRIANGLE_FAN,	// 		PRIMITIVETYPE_TRIANGLEFAN,
	};

	GLuint mode = types[type];

	if(debugEnabled)
	{
		Logger::FrameworkDebug("Draw arrays texture stated: id %d", currentState.textureState);
	}

    RENDER_VERIFY(glDrawArrays(mode, first, count));
    stats.drawArraysCalls++;
    switch(type)
    {
        case PRIMITIVETYPE_POINTLIST: 
            stats.primitiveCount[type] += count;
            break;
        case PRIMITIVETYPE_LINELIST:
            stats.primitiveCount[type] += count / 2;
            break;
        case PRIMITIVETYPE_LINESTRIP:
            stats.primitiveCount[type] += count - 1;
            break;
        case PRIMITIVETYPE_TRIANGLELIST:
            stats.primitiveCount[type] += count / 3;
            break;
        case PRIMITIVETYPE_TRIANGLEFAN:
        case PRIMITIVETYPE_TRIANGLESTRIP:
            stats.primitiveCount[type] += count - 2;
            break;
        default:
            break;
    };
}

void RenderManager::HWDrawElements(ePrimitiveType type, int32 count, eIndexFormat indexFormat, void * indices)
{
	static const int32 types[PRIMITIVETYPE_COUNT] = 
	{
		GL_POINTS,			// 		PRIMITIVETYPE_POINTLIST = 0,
		GL_LINES,			// 		PRIMITIVETYPE_LINELIST,
		GL_LINE_STRIP,		// 		PRIMITIVETYPE_LINESTRIP,
		GL_TRIANGLES,		// 		PRIMITIVETYPE_TRIANGLELIST,
		GL_TRIANGLE_STRIP,	// 		PRIMITIVETYPE_TRIANGLESTRIP,
		GL_TRIANGLE_FAN,	// 		PRIMITIVETYPE_TRIANGLEFAN,
	};
	
	GLuint mode = types[type];
	
	if(debugEnabled)
	{
		Logger::FrameworkDebug("Draw arrays texture state: id %d", currentState.textureState);
	}
#if defined(__DAVAENGINE_IPHONE__)
#if not defined(GL_UNSIGNED_INT)
#define GL_UNSIGNED_INT 0
#endif //not defined(GL_UNSIGNED_INT)
#endif // __DAVAENGINE_IPHONE__

	static const int32 indexTypes[2] = 
	{
		GL_UNSIGNED_SHORT, 
		GL_UNSIGNED_INT,
	};
	
	RENDER_VERIFY(glDrawElements(mode, count, indexTypes[indexFormat], indices));
    stats.drawElementsCalls++;
    switch(type)
    {
        case PRIMITIVETYPE_POINTLIST: 
            stats.primitiveCount[type] += count;
            break;
        case PRIMITIVETYPE_LINELIST:
            stats.primitiveCount[type] += count / 2;
            break;
        case PRIMITIVETYPE_LINESTRIP:
            stats.primitiveCount[type] += count - 1;
            break;
        case PRIMITIVETYPE_TRIANGLELIST:
            stats.primitiveCount[type] += count / 3;
            break;
        case PRIMITIVETYPE_TRIANGLEFAN:
        case PRIMITIVETYPE_TRIANGLESTRIP:
            stats.primitiveCount[type] += count - 2;
            break;
        default:
            break;
    };
}

void RenderManager::ClearWithColor(float32 r, float32 g, float32 b, float32 a)
{
	RENDER_VERIFY(glClearColor(r, g, b, a));
	RENDER_VERIFY(glClear(GL_COLOR_BUFFER_BIT));
}

void RenderManager::ClearDepthBuffer(float32 depth)
{
#if defined(__DAVAENGINE_IPHONE__) || \
    defined (__DAVAENGINE_ANDROID__) || \
    defined (__DAVAENGINE_WINDOWS_STORE__)
    RENDER_VERIFY(glClearDepthf(depth));
#else //#if defined(__DAVAENGINE_IPHONE__) || defined (__DAVAENGINE_ANDROID__)
    RENDER_VERIFY(glClearDepth(depth));
#endif //#if defined(__DAVAENGINE_IPHONE__) || defined (__DAVAENGINE_ANDROID__)
    RENDER_VERIFY(glClear(GL_DEPTH_BUFFER_BIT));
}

void RenderManager::ClearStencilBuffer(int32 stencil)
{
	RENDER_VERIFY(glClearStencil(stencil));
	RENDER_VERIFY(glClear(GL_STENCIL_BUFFER_BIT));
}
    
void RenderManager::Clear(const Color & color, float32 depth, int32 stencil)
{
    RENDER_VERIFY(glClearColor(color.r, color.g, color.b, color.a));
#if defined(__DAVAENGINE_IPHONE__) || \
    defined (__DAVAENGINE_ANDROID__) || \
    defined (__DAVAENGINE_WINDOWS_STORE__)
    RENDER_VERIFY(glClearDepthf(depth));
#else //#if defined(__DAVAENGINE_IPHONE__) || defined (__DAVAENGINE_ANDROID__)
    RENDER_VERIFY(glClearDepth(depth));
#endif //#if defined(__DAVAENGINE_IPHONE__) || defined (__DAVAENGINE_ANDROID__)
    RENDER_VERIFY(glClearStencil(stencil));

    
    RENDER_VERIFY(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void RenderManager::SetClip(const Rect &rect)
{
	if(rect.dx < 0 || rect.dy < 0)
	{
		RENDER_VERIFY(glDisable(GL_SCISSOR_TEST));
		return;
	}
	int32 x = (int32)(rect.x);
	int32 y = (int32)(rect.y);
	int32 x2= (int32)ceilf((rect.dx + rect.x));
	int32 y2= (int32)ceilf((rect.dy + rect.y));
	int32 width = x2 - x;
	int32 height = y2 - y;
    
    if (currentRenderTarget == nullptr)
    {
        y = frameBufferHeight - y - height;
    }
	
	RENDER_VERIFY(glEnable(GL_SCISSOR_TEST));
	RENDER_VERIFY(glScissor(x, y, width, height));
}

void RenderManager::SetRenderTarget(Texture * renderTarget)
{
    currentRenderTarget = renderTarget;
    if (renderTarget)
	    HWglBindFBO(renderTarget->fboID);
    else
        HWglBindFBO(fboViewFramebuffer);
}

void RenderManager::DiscardFramebufferHW(uint32 attachments)
{
#if defined (__DAVAENGINE_IPHONE__) || defined (__DAVAENGINE_ANDROID__)
    if (!attachments) 
      return;
#if defined (__DAVAENGINE_ANDROID__)
    if (glDiscardFramebufferEXT == NULL)
        return;
#endif

    GLenum discards[3];
    int32 discardsCount=0;
    if (attachments&COLOR_ATTACHMENT)
        discards[discardsCount++]=GL_COLOR_ATTACHMENT0;
    if (attachments&DEPTH_ATTACHMENT)
        discards[discardsCount++]=GL_DEPTH_ATTACHMENT;
    if (attachments&STENCIL_ATTACHMENT)
        discards[discardsCount++]=GL_STENCIL_ATTACHMENT;
    RENDER_VERIFY(glDiscardFramebufferEXT(GL_FRAMEBUFFER, discardsCount, discards));
#endif
}
    
void RenderManager::HWglDeleteBuffers(GLsizei count, const GLuint * buffers)
{
    // TODO: this is, probably, temporary fix.
    for(uint32 n = 0; n < (uint32)count; ++n)
    {
        if(bufferBindingId[0] == buffers[n])
        {
            RENDER_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, 0));
            bufferBindingId[0] = 0;
        }
        else if (bufferBindingId[1] == buffers[n])
        {
            RENDER_VERIFY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            bufferBindingId[1] = 0;
        }
    }
#if defined(__DAVAENGINE_OPENGL_ARB_VBO__)
    RENDER_VERIFY(glDeleteBuffersARB(count, buffers));
#else
    RENDER_VERIFY(glDeleteBuffers(count, buffers));
#endif
}
    
void RenderManager::HWglBindBuffer(GLenum target, GLuint buffer)
{
    DVASSERT(target - GL_ARRAY_BUFFER <= 1);
    if (bufferBindingId[target - GL_ARRAY_BUFFER] != buffer)
    {
        RENDER_VERIFY(glBindBuffer(target, buffer));
        bufferBindingId[target - GL_ARRAY_BUFFER] = buffer;
    }
}
    
void RenderManager::AttachRenderData()
{
    if (!currentRenderData)return;
    RENDERER_UPDATE_STATS(attachRenderDataCount++);
    
	Shader * shader = hardwareState.shader;
    uint32 currentAttributeMask = shader->GetRequiredVertexFormat();
    
    if (attachedRenderData == currentRenderData && cachedAttributeMask == currentAttributeMask)
    {
        if ((attachedRenderData->vboBuffer != 0) && (attachedRenderData->indexBuffer != 0))
        {
            RENDERER_UPDATE_STATS(attachRenderDataSkipCount++);
            return;
        }
    }
    
    bool vboChanged = ((NULL == attachedRenderData) || (NULL == currentRenderData) || (!currentRenderData->HasVertexAttachment()) || (attachedRenderData->vboBuffer != currentRenderData->vboBuffer) || (0 == currentRenderData->vboBuffer));
    bool iboChanged = ((NULL == attachedRenderData) || (NULL == currentRenderData) || (!currentRenderData->HasVertexAttachment()) || (attachedRenderData->indexBuffer != currentRenderData->indexBuffer) || (0 == currentRenderData->indexBuffer));
    
    attachedRenderData = currentRenderData;
    
    const int DEBUG = 0;
    
    {
        if(iboChanged)
        {
            HWglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentRenderData->indexBuffer);
        }
    
    
        if(vboChanged)
        {
            int32 currentEnabledStreams = 0;
            
            HWglBindBuffer(GL_ARRAY_BUFFER, currentRenderData->vboBuffer);
            
            int32 size = (int32)currentRenderData->streamArray.size();
            
            //DVASSERT(size >= shader->GetAttributeCount() && "Shader attribute count higher than model attribute count");
            if (size < shader->GetAttributeCount())
            {
                // Logger::Error("Shader attribute count higher than model attribute count");
            }
            
            for (int32 k = 0; k < size; ++k)
            {
                RenderDataStream * stream = currentRenderData->streamArray[k];
                GLboolean normalized = GL_FALSE;
                
                int32 attribIndex = shader->GetAttributeIndex(stream->formatMark);
                if (attribIndex != -1)
                {
                    int32 attribIndexBitPos = (1 << attribIndex);
                    
                    if(stream->formatMark == EVF_COLOR || stream->formatMark == EVF_JOINTWEIGHT)
                    {
                        normalized = GL_TRUE;
                    }
                    RENDER_VERIFY(glVertexAttribPointer(attribIndex, stream->size, VERTEX_DATA_TYPE_TO_GL[stream->type], normalized, stream->stride, stream->pointer));
                    if (DEBUG)Logger::FrameworkDebug("shader glVertexAttribPointer: %d", attribIndex);
                    
                    if (!(cachedEnabledStreams & attribIndexBitPos))  // enable only if it was not enabled on previous step
                    {
                        RENDER_VERIFY(glEnableVertexAttribArray(attribIndex));
                        if (DEBUG)Logger::FrameworkDebug("shader glEnableVertexAttribArray: %d", attribIndex);
                    }
                    
                    currentEnabledStreams |= attribIndexBitPos;
                }
            }
            
            if(cachedEnabledStreams != currentEnabledStreams)
            {
                // now we should disable all attribs, that are was enable previously and should not be enabled now
                int attribIndex = 0;
                int32 streamsToDisable = (cachedEnabledStreams ^ currentEnabledStreams) & cachedEnabledStreams;
                while(0 != streamsToDisable)
                {
                    if(streamsToDisable & 0x1)
                    {
                        if(DEBUG)Logger::FrameworkDebug("shader glDisableVertexAttribArray: %d", attribIndex);
                        RENDER_VERIFY(glDisableVertexAttribArray(attribIndex));
                    }
                    
                    streamsToDisable = streamsToDisable >> 1;
                    attribIndex++;
                }
                
                cachedEnabledStreams = currentEnabledStreams;
            }
        }
        
        cachedAttributeMask = currentAttributeMask;
    }
}
    
int32 RenderManager::HWglGetLastTextureID(int textureType)
{
    int32 ret = 0;
    RENDER_VERIFY(glGetIntegerv((Texture::TEXTURE_2D == textureType) ? GL_TEXTURE_BINDING_2D : GL_TEXTURE_BINDING_CUBE_MAP, &ret));
    return ret;
}
	
void RenderManager::HWglBindTexture(int32 tId, uint32 textureType)
{
    RENDER_VERIFY(glBindTexture((Texture::TEXTURE_2D == textureType) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, tId));
}

int32 RenderManager::HWglGetLastFBO()
{
    return lastBindedFBO;
}

void RenderManager::HWglBindFBO(const int32 fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    lastBindedFBO = fbo;
}
    
void RenderManager::DiscardDepth()
{
#if defined (__DAVAENGINE_IPHONE__) || defined (__DAVAENGINE_ANDROID__)
#if defined (__DAVAENGINE_ANDROID__)
    if (glDiscardFramebufferEXT == NULL)
        return;
#endif
    static const GLenum discards[]  = {GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT};
    RENDER_VERIFY(glDiscardFramebufferEXT(GL_FRAMEBUFFER,2,discards));
#endif
}

#if defined(__DAVAENGINE_ANDROID__)
void RenderManager::Lost()
{
    cachedEnabledStreams = 0;
    cachedAttributeMask = 0;
    
    bufferBindingId[0] = 0;
    bufferBindingId[1] = 0;

	lastBindedFBO = 0;
}

void RenderManager::Invalidate()
{

}
#endif


};

#endif // __DAVAENGINE_OPENGL__
