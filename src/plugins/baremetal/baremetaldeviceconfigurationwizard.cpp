/****************************************************************************
**
** Copyright (C) 2014 Tim Sander <tim@krieglstein.org>
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

#include "baremetaldeviceconfigurationwizard.h"
#include "baremetaldeviceconfigurationwizardpages.h"
#include "baremetaldevice.h"
#include "baremetalconstants.h"
#include "ssh/sshconnection.h"

using namespace ProjectExplorer;

namespace BareMetal {
namespace Internal {

enum PageId { SetupPageId };

BareMetalDeviceConfigurationWizard::BareMetalDeviceConfigurationWizard(QWidget *parent) :
   Utils::Wizard(parent),
   m_setupPage(new BareMetalDeviceConfigurationWizardSetupPage(this))
{
    setWindowTitle(tr("New Bare Metal Device Configuration Setup"));
    setPage(SetupPageId, m_setupPage);
    m_setupPage->setCommitPage(true);
}

IDevice::Ptr BareMetalDeviceConfigurationWizard::device() const
{
    //sshParams is not really used as ssh parameters but as debugger parameters
    QSsh::SshConnectionParameters sshParams;
    sshParams.host = m_setupPage->gdbHostname();
    sshParams.port = m_setupPage->gdbPort();
    BareMetalDevice::Ptr device = BareMetalDevice::create(m_setupPage->configurationName(),
                                                          Constants::BareMetalOsType,
                                                          IDevice::Hardware);
    device->setSshParameters(sshParams);
    device->setGdbResetCommands(m_setupPage->gdbResetCommands());
    device->setGdbInitCommands(m_setupPage->gdbInitCommands());
    return device;
}

} // namespace Internal
} // namespace BareMetal
