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

#include "cppfilesettingspage.h"

#include "cpptoolsconstants.h"
#include "cpptoolsplugin.h"
#include <ui_cppfilesettingspage.h>

#include <coreplugin/icore.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/mimedatabase.h>
#include <cppeditor/cppeditorconstants.h>

#include <utils/environment.h>

#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDate>
#include <QLocale>
#include <QTextCodec>
#include <QTextStream>
#include <QFileDialog>

static const char headerPrefixesKeyC[] = "HeaderPrefixes";
static const char sourcePrefixesKeyC[] = "SourcePrefixes";
static const char headerSuffixKeyC[] = "HeaderSuffix";
static const char sourceSuffixKeyC[] = "SourceSuffix";
static const char headerSearchPathsKeyC[] = "HeaderSearchPaths";
static const char sourceSearchPathsKeyC[] = "SourceSearchPaths";
static const char licenseTemplatePathKeyC[] = "LicenseTemplate";

const char *licenseTemplateTemplate = QT_TRANSLATE_NOOP("CppTools::Internal::CppFileSettingsWidget",
"/**************************************************************************\n"
"** Qt Creator license header template\n"
"**   Special keywords: %USER% %DATE% %YEAR%\n"
"**   Environment variables: %$VARIABLE%\n"
"**   To protect a percent sign, use '%%'.\n"
"**************************************************************************/\n");

namespace CppTools {
namespace Internal {

CppFileSettings::CppFileSettings() :
    lowerCaseFiles(false)
{
}

void CppFileSettings::toSettings(QSettings *s) const
{
    s->beginGroup(QLatin1String(Constants::CPPTOOLS_SETTINGSGROUP));
    s->setValue(QLatin1String(headerPrefixesKeyC), headerPrefixes);
    s->setValue(QLatin1String(sourcePrefixesKeyC), sourcePrefixes);
    s->setValue(QLatin1String(headerSuffixKeyC), headerSuffix);
    s->setValue(QLatin1String(sourceSuffixKeyC), sourceSuffix);
    s->setValue(QLatin1String(headerSearchPathsKeyC), headerSearchPaths);
    s->setValue(QLatin1String(sourceSearchPathsKeyC), sourceSearchPaths);
    s->setValue(QLatin1String(Constants::LOWERCASE_CPPFILES_KEY), lowerCaseFiles);
    s->setValue(QLatin1String(licenseTemplatePathKeyC), licenseTemplatePath);
    s->endGroup();
}

void CppFileSettings::fromSettings(QSettings *s)
{
    const QStringList defaultHeaderSearchPaths = QStringList()
            << QLatin1String("include")
            << QLatin1String("Include")
            << QDir::toNativeSeparators(QLatin1String("../include"))
            << QDir::toNativeSeparators(QLatin1String("../Include"));
    const QStringList defaultSourceSearchPaths = QStringList()
            << QDir::toNativeSeparators(QLatin1String("../src"))
            << QDir::toNativeSeparators(QLatin1String("../Src"))
            << QLatin1String("..");
    s->beginGroup(QLatin1String(Constants::CPPTOOLS_SETTINGSGROUP));
    headerPrefixes = s->value(QLatin1String(headerPrefixesKeyC)).toStringList();
    sourcePrefixes = s->value(QLatin1String(sourcePrefixesKeyC)).toStringList();
    headerSuffix = s->value(QLatin1String(headerSuffixKeyC), QLatin1String("h")).toString();
    sourceSuffix = s->value(QLatin1String(sourceSuffixKeyC), QLatin1String("cpp")).toString();
    headerSearchPaths = s->value(QLatin1String(headerSearchPathsKeyC), defaultHeaderSearchPaths)
            .toStringList();
    sourceSearchPaths = s->value(QLatin1String(sourceSearchPathsKeyC), defaultSourceSearchPaths)
            .toStringList();
    const bool lowerCaseDefault = Constants::lowerCaseFilesDefault;
    lowerCaseFiles = s->value(QLatin1String(Constants::LOWERCASE_CPPFILES_KEY), QVariant(lowerCaseDefault)).toBool();
    licenseTemplatePath = s->value(QLatin1String(licenseTemplatePathKeyC), QString()).toString();
    s->endGroup();
}

bool CppFileSettings::applySuffixesToMimeDB()
{
    return Core::MimeDatabase::setPreferredSuffix(QLatin1String(CppTools::Constants::CPP_SOURCE_MIMETYPE), sourceSuffix)
            && Core::MimeDatabase::setPreferredSuffix(QLatin1String(CppTools::Constants::CPP_HEADER_MIMETYPE), headerSuffix);
}

bool CppFileSettings::equals(const CppFileSettings &rhs) const
{
    return lowerCaseFiles == rhs.lowerCaseFiles
           && headerPrefixes == rhs.headerPrefixes
           && sourcePrefixes == rhs.sourcePrefixes
           && headerSuffix == rhs.headerSuffix
           && sourceSuffix == rhs.sourceSuffix
           && headerSearchPaths == rhs.headerSearchPaths
           && sourceSearchPaths == rhs.sourceSearchPaths
           && licenseTemplatePath == rhs.licenseTemplatePath;
}

// Replacements of special license template keywords.
static bool keyWordReplacement(const QString &keyWord,
                               const QString &file,
                               const QString &className,
                               QString *value)
{
    if (keyWord == QLatin1String("%YEAR%")) {
        *value = QString::number(QDate::currentDate().year());
        return true;
    }
    if (keyWord == QLatin1String("%MONTH%")) {
        *value = QString::number(QDate::currentDate().month());
        return true;
    }
    if (keyWord == QLatin1String("%DAY%")) {
        *value = QString::number(QDate::currentDate().day());
        return true;
    }
    if (keyWord == QLatin1String("%CLASS%")) {
        *value = className;
        return true;
    }
    if (keyWord == QLatin1String("%FILENAME%")) {
        *value = QFileInfo(file).fileName();
        return true;
    }
    if (keyWord == QLatin1String("%DATE%")) {
        static QString format;
        // ensure a format with 4 year digits. Some have locales have 2.
        if (format.isEmpty()) {
            QLocale loc;
            format = loc.dateFormat(QLocale::ShortFormat);
            const QChar ypsilon = QLatin1Char('y');
            if (format.count(ypsilon) == 2)
                format.insert(format.indexOf(ypsilon), QString(2, ypsilon));
        }
        *value = QDate::currentDate().toString(format);
        return true;
    }
    if (keyWord == QLatin1String("%USER%")) {
        *value = Utils::Environment::systemEnvironment().userName();
        return true;
    }
    // Environment variables (for example '%$EMAIL%').
    if (keyWord.startsWith(QLatin1String("%$"))) {
        const QString varName = keyWord.mid(2, keyWord.size() - 3);
        *value = QString::fromLocal8Bit(qgetenv(varName.toLocal8Bit()));
        return true;
    }
    return false;
}

// Parse a license template, scan for %KEYWORD% and replace if known.
// Replace '%%' by '%'.
static void parseLicenseTemplatePlaceholders(QString *t, const QString &file, const QString &className)
{
    int pos = 0;
    const QChar placeHolder = QLatin1Char('%');
    do {
        const int placeHolderPos = t->indexOf(placeHolder, pos);
        if (placeHolderPos == -1)
            break;
        const int endPlaceHolderPos = t->indexOf(placeHolder, placeHolderPos + 1);
        if (endPlaceHolderPos == -1)
            break;
        if (endPlaceHolderPos == placeHolderPos + 1) { // '%%' -> '%'
            t->remove(placeHolderPos, 1);
            pos = placeHolderPos + 1;
        } else {
            const QString keyWord = t->mid(placeHolderPos, endPlaceHolderPos + 1 - placeHolderPos);
            QString replacement;
            if (keyWordReplacement(keyWord, file, className, &replacement)) {
                t->replace(placeHolderPos, keyWord.size(), replacement);
                pos = placeHolderPos + replacement.size();
            } else {
                // Leave invalid keywords as is.
                pos = endPlaceHolderPos + 1;
            }
        }
    } while (pos < t->size());
}

// Convenience that returns the formatted license template.
QString CppFileSettings::licenseTemplate(const QString &fileName, const QString &className)
{

    const QSettings *s = Core::ICore::settings();
    QString key = QLatin1String(Constants::CPPTOOLS_SETTINGSGROUP);
    key += QLatin1Char('/');
    key += QLatin1String(licenseTemplatePathKeyC);
    const QString path = s->value(key, QString()).toString();
    if (path.isEmpty())
        return QString();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qWarning("Unable to open the license template %s: %s", qPrintable(path), qPrintable(file.errorString()));
        return QString();
    }

    QTextStream licenseStream(&file);
    licenseStream.setCodec(Core::EditorManager::defaultTextCodec());
    licenseStream.setAutoDetectUnicode(true);
    QString license = licenseStream.readAll();

    parseLicenseTemplatePlaceholders(&license, fileName, className);
    // Ensure exactly one additional new line separating stuff
    const QChar newLine = QLatin1Char('\n');
    if (!license.endsWith(newLine))
        license += newLine;
    license += newLine;
    return license;
}

// ------------------ CppFileSettingsWidget

CppFileSettingsWidget::CppFileSettingsWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Internal::Ui::CppFileSettingsPage)
{
    m_ui->setupUi(this);
    // populate suffix combos
    if (const Core::MimeType sourceMt = Core::MimeDatabase::findByType(QLatin1String(CppTools::Constants::CPP_SOURCE_MIMETYPE)))
        foreach (const QString &suffix, sourceMt.suffixes())
            m_ui->sourceSuffixComboBox->addItem(suffix);

    if (const Core::MimeType headerMt = Core::MimeDatabase::findByType(QLatin1String(CppTools::Constants::CPP_HEADER_MIMETYPE)))
        foreach (const QString &suffix, headerMt.suffixes())
            m_ui->headerSuffixComboBox->addItem(suffix);
    m_ui->licenseTemplatePathChooser->setExpectedKind(Utils::PathChooser::File);
    m_ui->licenseTemplatePathChooser->setHistoryCompleter(QLatin1String("Cpp.LicenseTemplate.History"));
    m_ui->licenseTemplatePathChooser->addButton(tr("Edit..."), this, SLOT(slotEdit()));
}

CppFileSettingsWidget::~CppFileSettingsWidget()
{
    delete m_ui;
}

QString CppFileSettingsWidget::licenseTemplatePath() const
{
    return m_ui->licenseTemplatePathChooser->path();
}

void CppFileSettingsWidget::setLicenseTemplatePath(const QString &lp)
{
    m_ui->licenseTemplatePathChooser->setPath(lp);
}

static QStringList trimmedPaths(const QString &paths)
{
    QStringList res;
    foreach (const QString &path, paths.split(QLatin1Char(','), QString::SkipEmptyParts))
        res << path.trimmed();
    return res;
}

CppFileSettings CppFileSettingsWidget::settings() const
{
    CppFileSettings rc;
    rc.lowerCaseFiles = m_ui->lowerCaseFileNamesCheckBox->isChecked();
    rc.headerPrefixes = trimmedPaths(m_ui->headerPrefixesEdit->text());
    rc.sourcePrefixes = trimmedPaths(m_ui->sourcePrefixesEdit->text());
    rc.headerSuffix = m_ui->headerSuffixComboBox->currentText();
    rc.sourceSuffix = m_ui->sourceSuffixComboBox->currentText();
    rc.headerSearchPaths = trimmedPaths(m_ui->headerSearchPathsEdit->text());
    rc.sourceSearchPaths = trimmedPaths(m_ui->sourceSearchPathsEdit->text());
    rc.licenseTemplatePath = licenseTemplatePath();
    return rc;
}

static inline void setComboText(QComboBox *cb, const QString &text, int defaultIndex = 0)
{
    const int index = cb->findText(text);
    cb->setCurrentIndex(index == -1 ? defaultIndex: index);
}

void CppFileSettingsWidget::setSettings(const CppFileSettings &s)
{
    m_ui->lowerCaseFileNamesCheckBox->setChecked(s.lowerCaseFiles);
    m_ui->headerPrefixesEdit->setText(s.headerPrefixes.join(QLatin1String(",")));
    m_ui->sourcePrefixesEdit->setText(s.sourcePrefixes.join(QLatin1String(",")));
    setComboText(m_ui->headerSuffixComboBox, s.headerSuffix);
    setComboText(m_ui->sourceSuffixComboBox, s.sourceSuffix);
    m_ui->headerSearchPathsEdit->setText(s.headerSearchPaths.join(QLatin1String(",")));
    m_ui->sourceSearchPathsEdit->setText(s.sourceSearchPaths.join(QLatin1String(",")));
    setLicenseTemplatePath(s.licenseTemplatePath);
}

void CppFileSettingsWidget::slotEdit()
{
    QString path = licenseTemplatePath();
    if (path.isEmpty()) {
        // Pick a file name and write new template, edit with C++
        path = QFileDialog::getSaveFileName(this, tr("Choose Location for New License Template File"));
        if (path.isEmpty())
            return;
        Utils::FileSaver saver(path, QIODevice::Text);
        saver.write(tr(licenseTemplateTemplate).toUtf8());
        if (!saver.finalize(this))
            return;
        setLicenseTemplatePath(path);
    }
    // Edit (now) existing file with C++
    Core::EditorManager::openEditor(path, CppEditor::Constants::CPPEDITOR_ID);
}

// --------------- CppFileSettingsPage
CppFileSettingsPage::CppFileSettingsPage(QSharedPointer<CppFileSettings> &settings,
                                         QObject *parent) :
    Core::IOptionsPage(parent),
    m_settings(settings)
{
    setId(Constants::CPP_FILE_SETTINGS_ID);
    setDisplayName(QCoreApplication::translate("CppTools", Constants::CPP_FILE_SETTINGS_NAME));
    setCategory(Constants::CPP_SETTINGS_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("CppTools", Constants::CPP_SETTINGS_TR_CATEGORY));
    setCategoryIcon(QLatin1String(Constants::SETTINGS_CATEGORY_CPP_ICON));
}

QWidget *CppFileSettingsPage::widget()
{

    if (!m_widget) {
        m_widget = new CppFileSettingsWidget;
        m_widget->setSettings(*m_settings);
    }
    return m_widget;
}

void CppFileSettingsPage::apply()
{
    if (m_widget) {
        const CppFileSettings newSettings = m_widget->settings();
        if (newSettings != *m_settings) {
            *m_settings = newSettings;
            m_settings->toSettings(Core::ICore::settings());
            m_settings->applySuffixesToMimeDB();
            CppToolsPlugin::clearHeaderSourceCache();
        }
    }
}

void CppFileSettingsPage::finish()
{
    delete m_widget;
}

} // namespace Internal
} // namespace CppTools
