/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef QMLPROFILEREVENTTYPES_H
#define QMLPROFILEREVENTTYPES_H

#include <QtGlobal>

namespace QmlDebug {

enum Message {
    Event,
    RangeStart,
    RangeData,
    RangeLocation,
    RangeEnd,
    Complete,
    PixmapCacheEvent,
    SceneGraphFrame,
    MemoryAllocation,

    MaximumMessage
};

enum EventType {
    FramePaint, // unused
    Mouse,
    Key,
    AnimationFrame, // new Qt5 paint events
    EndTrace,
    StartTrace,

    MaximumEventType
};

enum RangeType {
    Painting, // old Qt4 paint events
    Compiling,
    Creating,
    Binding,
    HandlingSignal,
    Javascript,

    MaximumRangeType
};

enum BindingType {
    QmlBinding,
    V8Binding,
    OptimizedBinding,
    QPainterEvent,

    MaximumBindingType
};

enum PixmapEventType {
    PixmapSizeKnown,
    PixmapReferenceCountChanged,
    PixmapCacheCountChanged,
    PixmapLoadingStarted,
    PixmapLoadingFinished,
    PixmapLoadingError,

    MaximumPixmapEventType
};

enum SceneGraphFrameType {
    SceneGraphRendererFrame,
    SceneGraphAdaptationLayerFrame,
    SceneGraphContextFrame,
    SceneGraphRenderLoopFrame,
    SceneGraphTexturePrepare,
    SceneGraphTextureDeletion,
    SceneGraphPolishAndSync,
    SceneGraphWindowsRenderShow,
    SceneGraphWindowsAnimations,
    SceneGraphWindowsPolishFrame,

    MaximumSceneGraphFrameType
};

enum MemoryType {
    HeapPage,
    LargeItem,
    SmallItem,

    MaximumMemoryType
};

enum AnimationThread {
    GuiThread,
    RenderThread,

    MaximumAnimationThread
};

namespace Constants {
const int QML_MIN_LEVEL = 1; // Set to 0 to remove the empty line between models in the timeline
}

} // namespace QmlDebug

#endif //QMLPROFILEREVENTTYPES_H
