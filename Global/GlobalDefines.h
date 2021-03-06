//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
*Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012. 
*contact: immarespond at gmail dot com
*
*/

#ifndef NATRON_GLOBAL_GLOBALDEFINES_H_
#define NATRON_GLOBAL_GLOBALDEFINES_H_

#include <utility>
#if defined(_WIN32)
#include <string>
#include <windows.h>
#endif

#include "Global/Macros.h"
#ifndef Q_MOC_RUN
#include <boost/cstdint.hpp>
#endif
#include <QtCore/QForeachContainer>
CLANG_DIAG_OFF(deprecated)
#include <QtCore/QMetaType>
CLANG_DIAG_ON(deprecated)
#include "Global/Enums.h"

#undef foreach
#define foreach Q_FOREACH


typedef boost::uint32_t U32;
typedef boost::uint64_t U64;
typedef boost::uint8_t U8;
typedef boost::uint16_t U16;

#include <ofxhImageEffect.h>
#include <ofxPixels.h>

typedef int SequenceTime;

Q_DECLARE_METATYPE(SequenceTime)

typedef OfxPointD RenderScale;

namespace Natron {
typedef OfxPointD Point;
}

typedef OfxRGBAColourF RGBAColourF;

typedef OfxRangeD RangeD;

///these are used between process to communicate via the pipes
#define kRenderingStartedLong "Rendering started"
#define kRenderingStartedShort "-b"

#define kFrameRenderedStringLong "Frame rendered: "
#define kFrameRenderedStringShort "-r"

#define kProgressChangedStringLong "Progress changed: "
#define kProgressChangedStringShort "-p"

#define kRenderingFinishedStringLong "Rendering finished"
#define kRenderingFinishedStringShort "-e"

#define kAbortRenderingStringLong "Abort rendering"
#define kAbortRenderingStringShort "-a"

#define kBgProcessServerCreatedShort "--bg_server_created"


#define kNodeGraphObjectName "NodeGraph"
#define kCurveEditorObjectName "CurveEditor"

#define kCurveEditorMoveMultipleKeysCommandCompressionID 2
#define kKnobUndoChangeCommandCompressionID 3
#define kNodeGraphMoveNodeCommandCompressionID 4
#define kRotoMoveControlPointsCompressionID 5
#define kRotoMoveTangentCompressionID 6
#define kRotoCuspSmoothCompressionID 7
#define kRotoMoveFeatherBarCompressionID 8
#define kRotoMakeBezierCompressionID 9
#define kRotoMakeEllipseCompressionID 10
#define kRotoMakeRectangleCompressionID 11
#define kRotoTransformCompressionID 12
#define kMultipleKnobsUndoChangeCommandCompressionID 13
#define kNodeGraphMoveNodeBackDropCommandCompressionID 14
#define kNodeGraphResizeNodeBackDropCommandCompressionID 15

#ifdef __NATRON_WIN32__
namespace NatronWindows{
    /*Converts a std::string to wide string*/
    inline std::wstring s2ws(const std::string& s)
    {
        int len;
        int slength = (int)s.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }
}
#endif

#endif
