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

#ifndef RESIZETOOL_H
#define RESIZETOOL_H

#include "abstractformeditortool.h"
#include "selectionindicator.h"
#include "resizeindicator.h"
#include "anchorindicator.h"
#include "resizemanipulator.h"


namespace QmlDesigner {

class ResizeTool : public AbstractFormEditorTool
{
public:
    ResizeTool(FormEditorView* editorView);
    ~ResizeTool();

    void mousePressEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneMouseEvent *event) QTC_OVERRIDE;
    void mouseMoveEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneMouseEvent *event) QTC_OVERRIDE;
    void mouseReleaseEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneMouseEvent *event) QTC_OVERRIDE;
    void mouseDoubleClickEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneMouseEvent *event) QTC_OVERRIDE;

    void hoverMoveEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneMouseEvent *event) QTC_OVERRIDE;

    void dragLeaveEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneDragDropEvent *event) QTC_OVERRIDE;
    void dragMoveEvent(const QList<QGraphicsItem*> &itemList, QGraphicsSceneDragDropEvent *event) QTC_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) QTC_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *keyEvent) QTC_OVERRIDE;

    void itemsAboutToRemoved(const QList<FormEditorItem*> &itemList) QTC_OVERRIDE;

    void selectedItemsChanged(const QList<FormEditorItem*> &itemList) QTC_OVERRIDE;

    void clear() QTC_OVERRIDE;

    void formEditorItemsChanged(const QList<FormEditorItem*> &itemList) QTC_OVERRIDE;
    void instancesParentChanged(const QList<FormEditorItem *> &itemList) QTC_OVERRIDE;

    void instancesCompleted(const QList<FormEditorItem*> &itemList) QTC_OVERRIDE;
    void instancePropertyChange(const QList<QPair<ModelNode, PropertyName> > &propertyList) QTC_OVERRIDE;


private:
    SelectionIndicator m_selectionIndicator;
    ResizeIndicator m_resizeIndicator;
    AnchorIndicator m_anchorIndicator;
    ResizeManipulator m_resizeManipulator;
};

}

#endif // RESIZETOOL_H
