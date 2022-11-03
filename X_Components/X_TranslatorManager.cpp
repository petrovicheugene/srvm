//================================================
#include "X_TranslatorManager.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QTextCodec>
#include <QTranslator>
//================================================
X_TranslatorManager::X_TranslatorManager(QObject* parent,
                                       QString resoucesTranslatorDirName,
                                       QString internalTranslatorDir,
                                       QString prefix)
    : QObject(parent)
{
    xv_resoucesTranslatorDirName = resoucesTranslatorDirName;
    xv_internalTranslatorDir = internalTranslatorDir;
    xv_prefix = prefix;
    zh_createAvailableLocaleList();
}
//================================================
bool X_TranslatorManager::xp_installTranslatorsToApplication()
{
    xv_errorMessage.clear();

    if(!qApp )
    {
        zh_setErrorMessage(tr("Cannot install application translator. "
                              "The pointer of the application is invalid!"));
        return false;
    }

    // get language name
    QLocale appLocale;
    zh_getApplicationLocale(appLocale);

    QTranslator* appTranslator = new QTranslator(qApp);
    QDir appDir(QApplication::applicationDirPath());

    bool res = appTranslator->load(appLocale, qApp->applicationName(), xv_prefix, xv_internalTranslatorDir);
    if(!res)
    {
        res = appTranslator->load(appLocale, qApp->applicationName(), xv_prefix, xv_resoucesTranslatorDirName);
    }

    if(res)
    {
        res = qApp->installTranslator(appTranslator);
    }

    if(!res)
    {
        zh_setErrorMessage(tr("Cannot install application translator. Unknown error."));
        delete appTranslator;
        return false;
    }

    QTranslator* qtTranslator = new QTranslator(qApp);;

    res = qtTranslator->load(appLocale, "qtbase", xv_prefix, xv_internalTranslatorDir);
    if(!res)
    {
        res = qtTranslator->load(appLocale, "qtbase", xv_prefix, xv_resoucesTranslatorDirName);
    }

    if(res)
    {
        res = qApp->installTranslator(qtTranslator);
    }

    if(!res)
    {
        zh_setErrorMessage(tr("Cannot install Qt translator. Unknown error."));
        delete qtTranslator;
        return false;
    }

    return true;
}
//================================================
QString X_TranslatorManager::xp_resourcesTranslatorDirName() const
{
    return xv_resoucesTranslatorDirName;
}
//================================================
QString X_TranslatorManager::internalTranslatorDir() const
{
    return xv_internalTranslatorDir;
}
//================================================
QString X_TranslatorManager::prefix() const
{
    return xv_prefix;
}
//================================================
QString X_TranslatorManager::xp_lastError() const
{
    return xv_errorMessage;
}
//================================================
void X_TranslatorManager::zh_createAvailableLocaleList()
{
    // application base name
    QString appBaseName = qApp->applicationName().isEmpty()?
                "*" : qApp-> applicationName();

    // get all qm file list
    QDir dir(qApp->applicationDirPath());
    QStringList translatorFileNameStringList;
    translatorFileNameStringList = dir.entryList(QStringList(QString("%1.qm").arg(appBaseName+"*")));

    QDir resDir(xv_resoucesTranslatorDirName);
    //QStringList resFiles;
    translatorFileNameStringList.append(resDir.entryList());

    QString localeName;
    foreach(QString translatorFileName, translatorFileNameStringList)
    {
        QLocale locale(QLocale::c());
        localeName = zh_localeNameForTranslator(translatorFileName);
        locale = QLocale(localeName);

        if(locale == QLocale::c())
        {
            continue;
        }

        if(!xv_availableLocaleList.contains(locale))
        {
            xv_availableLocaleList.append(locale);
        }
    }
}
//================================================
bool X_TranslatorManager::zh_nativeLanguageNameForTranslatorFile(const QString& translatorFileName,
                                                                QString& languageName) const
{
    QString localeName = zh_localeNameForTranslator(translatorFileName);
    if(localeName.isEmpty())
    {
        return false;
    }

    languageName = QLocale(localeName).nativeLanguageName();
    if(languageName.isEmpty())
    {
        return false;
    }

    languageName[0] = languageName[0].toUpper();
    return true;
}
//================================================
void X_TranslatorManager::xp_availableLanguageNameList(QStringList& languageList) const
{
    QString languageName;
    foreach(QLocale locale, xv_availableLocaleList)
    {
        languageName = locale.nativeLanguageName();
        languageName[0] = languageName[0].toUpper();
        languageList.append(languageName);
    }

    std::sort(languageList.begin(), languageList.end());
}
//================================================
void X_TranslatorManager::xp_setApplicationLanguage (const QString& languageName,  bool* ok)
{
    foreach(QLocale locale, xv_availableLocaleList)
    {
        if(locale.nativeLanguageName().compare(languageName, Qt::CaseInsensitive) != 0)
        {
            continue;
        }

        QLocale currentLocale;
        zh_getApplicationLocale(currentLocale);

        if(currentLocale == locale)
        {
            // locale for languageName is already set
            if(ok)
            {
                *ok = false;
            }
            return;
        }

        QSettings settings;
        zh_openSettings(settings);
        settings.setValue(xv_appLanguageSettingsName, QVariant(locale.name()));
        zh_closeSettings(settings);
        if(ok)
        {
            *ok = true;
        }
        return;
    }

    zh_setErrorMessage(tr("Cannot set the selected langusge. Translator not found!"));
    if(ok)
    {
        *ok = false;
    }
}
//================================================
void X_TranslatorManager::xp_currentLanguageName(QString& languageName, bool *ok)
{
    QLocale locale;
    zh_getApplicationLocale(locale);

    if(locale == QLocale::c())
    {
        if(ok)
        {
            *ok = false;
        }
        return;
    }

    languageName = locale.nativeLanguageName();
    languageName[0] = languageName[0].toUpper();

    if(ok)
    {
        *ok = true;
    }
}
//================================================
void X_TranslatorManager::xp_currentLocaleName(QString& localeName, bool *ok)
{
    QLocale locale;
    zh_getApplicationLocale(locale);

    if(locale == QLocale::c())
    {
        if(ok)
        {
            *ok = false;
        }
        return;
    }

    localeName = locale.name();

    if(ok)
    {
        *ok = true;
    }
}
//================================================
void X_TranslatorManager::zh_getApplicationLocale(QLocale& locale)
{
    if(!zh_getApplicationLocaleFromSettings(locale))
    {
        locale = QLocale::system();
    }
}
//================================================
bool X_TranslatorManager::zh_getApplicationLocaleFromSettings(QLocale &locale)
{
    QSettings settings;
    zh_openSettings(settings);
    if(!settings.contains(xv_appLanguageSettingsName))
    {
        zh_setErrorMessage(tr("Language for application is not set!"));
        return false;
    }

    QVariant vData = settings.value(xv_appLanguageSettingsName);
    zh_closeSettings(settings);

    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        zh_setErrorMessage(tr("Cannot get translator file name from settings!"));
        return false;
    }

    QString localeName = vData.toString();
    locale = QLocale(localeName);

    return true;
}
//================================================
void X_TranslatorManager::zh_openSettings(QSettings &settings) const
{
    QString version = qApp->applicationVersion();
    if(!version.isEmpty())
    {
        settings.beginGroup(version);
    }
}
//================================================
void X_TranslatorManager::zh_closeSettings(QSettings& settings) const
{
    while (!settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//================================================
void X_TranslatorManager::zh_setErrorMessage(const QString& msg)
{
    xv_errorMessage = msg;
    qWarning().noquote() << xv_errorMessage;
}
//================================================
QString X_TranslatorManager::zh_localeNameForTranslator(const QString& translatorFileName) const
{
    QString localeName = translatorFileName;
    localeName.truncate(localeName.lastIndexOf('.')); // "TranslationExample_de"
    localeName.remove(0, localeName.lastIndexOf(xv_prefix) + 1); // "de"
    return localeName;
}
//================================================
