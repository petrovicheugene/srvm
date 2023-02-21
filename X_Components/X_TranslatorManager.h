//================================================
#ifndef X_LANGUAGEMANAGER_H
#define X_LANGUAGEMANAGER_H
//================================================
#include <QObject>

#include <QApplication>
#include <QDir>
#include <QMap>
#include <QSettings>
//================================================
class X_TranslatorManager : public QObject
{
    Q_OBJECT
public:
    explicit X_TranslatorManager(QObject* parent = nullptr,
                                 QString  resoucesTranslatorDirName = ":translators",
                                 QString internalTranslatorDir = QDir(qApp->applicationDirPath()).absoluteFilePath("translations"),
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
    void xh_createAvailableLocaleList();
    bool xh_nativeLanguageNameForTranslatorFile(const QString& translatorFileName,
                                                QString& languageName) const;
    void xh_getApplicationLocale(QLocale &locale);
    bool xh_getApplicationLocaleFromSettings(QLocale &locale);

    void xh_openSettings(QSettings& settings) const;
    void xh_closeSettings(QSettings& settings) const;
    void xh_setErrorMessage(const QString &msg);
    QString xh_localeNameForTranslator(const QString& translatorFileName) const;

};
//================================================
#endif // X_LANGUAGEMANAGER_H
