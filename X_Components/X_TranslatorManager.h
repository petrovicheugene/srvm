//================================================
#ifndef X_LANGUAGEMANAGER_H
#define X_LANGUAGEMANAGER_H
//================================================
#include <QObject>

#include <QApplication>
#include <QMap>
#include <QSettings>
//================================================
class X_TranslatorManager : public QObject
{
    Q_OBJECT
public:
    explicit X_TranslatorManager(QObject* parent = nullptr,
            QString  resoucesTranslatorDirName = ":translators",
                              QString internalTranslatorDir = qApp->applicationDirPath(),
                              QString prefix = "_");

    bool xp_installTranslatorsToApplication();
    QString xp_lastError() const;

    QString xp_resourcesTranslatorDirName() const;
    QString internalTranslatorDir() const;
    QString prefix() const;


public slots:

    void xp_availableLanguageNameList(QStringList& languageList) const;
    void xp_setApplicationLanguage(const QString& languageName, bool *ok = nullptr);
    void xp_currentLanguageName(QString& languageName, bool *ok = nullptr);
    void xp_currentLocaleName(QString& localeName, bool *ok = nullptr);

private:
    // VARS
    QList<QLocale> xv_availableLocaleList;
    const QString xv_appLanguageSettingsName = QString("appLanguage");
    QString xv_resoucesTranslatorDirName;
    QString xv_internalTranslatorDir;
    QString xv_prefix;
    QString xv_errorMessage;

    // FUNCS
    void zh_createAvailableLocaleList();
    bool zh_nativeLanguageNameForTranslatorFile(const QString& translatorFileName,
                                                QString& languageName) const;
    void zh_getApplicationLocale(QLocale &locale);
    bool zh_getApplicationLocaleFromSettings(QLocale &locale);

    void zh_openSettings(QSettings& settings) const;
    void zh_closeSettings(QSettings& settings) const;
    void zh_setErrorMessage(const QString &msg);
    QString zh_localeNameForTranslator(const QString& translatorFileName) const;

};
//================================================
#endif // X_LANGUAGEMANAGER_H
