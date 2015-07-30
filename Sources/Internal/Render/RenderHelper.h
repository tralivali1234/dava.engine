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


#ifndef __DAVAENGINE_RENDER_HELPER_H__
#define __DAVAENGINE_RENDER_HELPER_H__

#include "Base/BaseTypes.h"
#include "Base/BaseMath.h"
#include "Render/RenderBase.h"

#include "Animation/Interpolation.h"
#include "Render/Renderer.h"
#include "Render/Material/NMaterial.h"
#include "Render/Highlevel/Camera.h"

namespace DAVA
{	
    
/**
    \brief You can use this class to perform various important drawing operations in 3D.
    In most cases we use these functions for debug rendering, but in some cases it can be usefull in production. 
    Keep in mind that output of all line-drawing functions can depend on hardware and look differently on different systems
 */


    class RenderHelper
    {
        enum eDrawFlags
        {
            FLAG_DRAW_SOLID     = 1,
            FLAG_DRAW_NO_DEPTH  = 2,
        };

    public:
        enum eDrawType
        {
            DRAW_WIRE_DEPTH      = 0,
            DRAW_SOLID_DEPTH     = FLAG_DRAW_SOLID,
            DRAW_WIRE_NO_DEPTH   = FLAG_DRAW_NO_DEPTH,
            DRAW_SOLID_NO_DEPTH  = FLAG_DRAW_SOLID | FLAG_DRAW_NO_DEPTH,

            DRAW_TYPE_COUNT
        };

        RenderHelper();
        ~RenderHelper();

        void Present(rhi::HPacketList packetList, const Matrix4 * view, const Matrix4 * projection);
        void Clear();
        bool IsEmpty();

        void DrawLine(const Vector3 & pt1, const Vector3 & pt2, const Color & color, eDrawType drawType = DRAW_WIRE_DEPTH);
        void DrawPolygon(const Polygon3 & polygon, const Color & color, eDrawType drawType);
        void DrawAABox(const AABBox3 & box, const Color & color, eDrawType drawType);
        void DrawOBox(const AABBox3 & box, const Matrix4 & matrix, const Color & color, eDrawType drawType);
        void DrawIcosahedron(const Vector3 & position, float32 radius, const Color & color, eDrawType drawType);
        void DrawArrow(const Vector3 & from, const Vector3 & to, float32 arrowLength, const Color & color, eDrawType drawType);
        void DrawCornerAABox(const AABBox3 & box, const Color & color, eDrawType drawType);
        void DrawCircle(const Vector3 & center, const Vector3 &directionVector, float32 radius, const Color & color, eDrawType drawType);
        void DrawBSpline(BezierSpline3 * bSpline, int segments, float ts, float te, const Color & color, eDrawType drawType);
        void DrawInterpolationFunc(Interpolation::Func func, const Rect & destRect, const Color & color, eDrawType drawType);

        static void CreateClearPass(rhi::HTexture handle, int32 passPriority, const Color & clearColor, const rhi::Viewport & viewport);

    protected:
        enum eDrawCommandID
        {
            COMMAND_DRAW_LINE = 0,
            COMMAND_DRAW_POLYGON,
            COMMAND_DRAW_BOX,
            COMMAND_DRAW_ICOSA,
            COMMAND_DRAW_ARROW,

            COMMAND_COUNT
        };
        struct DrawCommand
        {
            DrawCommand(eDrawCommandID _id, eDrawType _drawType, const Vector<float32> & _params)
                : id(_id), drawType(_drawType), params(_params)
            {}

            eDrawCommandID id;
            eDrawType drawType;
            Vector<float32> params;
        };
        struct ColoredVertex
        {
            Vector3 position;
            uint32 color;
        };

        void QueueCommand(const DrawCommand && command);
        void GetRequestedVertexCount(const DrawCommand & command, uint32 & vertexCount, uint32 & indexCount);
        void PreparePacket(rhi::Packet & packet, NMaterial * material, const std::pair<uint32, uint32> & buffersCount, ColoredVertex ** vBufferDataPtr, uint16 ** iBufferDataPtr);

        template <size_t size> 
        uint32 FillIndeciesFromArray(uint16 * buffer, uint16 baseIndex, const std::array<uint16, size> & indecies);
        void MakeCirclePolygon(Polygon3 & outPolygon, const Vector3 & center, const Vector3 &directionVector, float32 radius);

        uint32 coloredVertexLayoutUID;

        Vector<DrawCommand> commandQueue;
        std::array< std::pair<uint32, uint32>, DRAW_TYPE_COUNT> buffersElemCount; //first - VertexBuffer, second - IndexBuffer
        NMaterial * materials[DRAW_TYPE_COUNT];
    };

    template <size_t size>
    uint32 RenderHelper::FillIndeciesFromArray(uint16 * buffer, uint16 baseIndex, const std::array<uint16, size> & indecies)
    {
        uint16 * bufferPtr = buffer;
        for (const uint16 & index : indecies)
        {
            *bufferPtr = baseIndex + index;
            ++bufferPtr;
        }
        return indecies.size();
    }
}

#endif // __DAVAENGINE_OBJC_FRAMEWORK_RENDER_HELPER_H__

