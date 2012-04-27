/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef SYMBIANIDEVICE_H
#define SYMBIANIDEVICE_H

#include "../qt4projectmanager_global.h"

#include <projectexplorer/devicesupport/idevice.h>

namespace Qt4ProjectManager {
namespace Internal { class S60Manager; }

// TODO: Make the idevice interface powerful enough that this class is no longer
// needed in other plugins and then move this into Internal namespace and
// do not export it.
class QT4PROJECTMANAGER_EXPORT SymbianIDevice : public ProjectExplorer::IDevice
{
public:
    typedef QSharedPointer<const SymbianIDevice> ConstPtr;

    enum CommunicationChannel {
        CommunicationCodaSerialConnection,
        CommunicationCodaTcpConnection
    };

    SymbianIDevice();
    SymbianIDevice(const QVariantMap &map);

    DeviceInfo deviceInformation() const;

    ProjectExplorer::IDevice::Ptr clone() const;

    QString serialPortName() const;
    void setSerialPortName(const QString &name);

    QString address() const;
    void setAddress(const QString &address);

    void setPort(const QString &port);
    QString port() const;

    void setCommunicationChannel(CommunicationChannel channel);
    CommunicationChannel communicationChannel() const;

    void fromMap(const QVariantMap &map);

    QString displayType() const;
    ProjectExplorer::IDeviceWidget* createWidget();
    QStringList actionIds() const;
    QString displayNameForActionId(const QString&actionId) const;
    QDialog* createAction(const QString&, QWidget*parent) const;

protected:
    SymbianIDevice(const SymbianIDevice &other);
    SymbianIDevice &operator=(const SymbianIDevice &); // no impl.

    QVariantMap toMap() const;

private:
    void updateState();

    QString m_serialPortName;
    QString m_address;
    QString m_port;
    CommunicationChannel m_communicationChannel;

    friend class Internal::S60Manager;
};

} // namespace Qt4ProjectManager

#endif // SYMBIANIDEVICE_H
