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

#ifndef QTOUTPUTFORMATTER_H
#define QTOUTPUTFORMATTER_H

#include "qtsupport_global.h"

#include <utils/outputformatter.h>
#include <utils/fileinprojectfinder.h>

#include <QRegExp>
#include <QPointer>

QT_FORWARD_DECLARE_CLASS(QTextCursor)

namespace ProjectExplorer { class Project; }

namespace QtSupport {

struct LinkResult
{
    int start;
    int end;
    QString href;
};

namespace Internal {
    class QtSupportPlugin;
}

class QTSUPPORT_EXPORT QtOutputFormatter
    : public Utils::OutputFormatter
{
    Q_OBJECT
public:
    explicit QtOutputFormatter(ProjectExplorer::Project *project);

    void appendMessage(const QString &text,
        Utils::OutputFormat format);
    void handleLink(const QString &href);

protected:
    void clearLastLine();
    virtual void openEditor(const QString &fileName, int line, int column = -1);

private slots:
    void updateProjectFileList();

private:
    LinkResult matchLine(const QString &line) const;
    void appendLine(QTextCursor &cursor, const LinkResult &lr, const QString &line,
                    Utils::OutputFormat);

    mutable QRegExp m_qmlError;
    mutable QRegExp m_qtError;
    mutable QRegExp m_qtAssert;
    mutable QRegExp m_qtAssertX;
    mutable QRegExp m_qtTestFail;
    QPointer<ProjectExplorer::Project> m_project;
    QString m_lastLine;
    Utils::FileInProjectFinder m_projectFinder;

    // for testing
    friend class Internal::QtSupportPlugin;
};


} // namespace QtSupport

#endif // QTOUTPUTFORMATTER_H
