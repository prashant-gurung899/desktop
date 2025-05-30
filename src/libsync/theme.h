/*
 * Copyright (C) by Klaas Freitag <freitag@owncloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#ifndef _THEME_H
#define _THEME_H

#include "common/utility.h"
#include "resources/resources.h"
#include "syncresult.h"

#include <QFileInfo>
#include <QObject>
#include <QPalette>
#include <qquickwindow.h>

namespace OCC {

class SyncResult;

/**
 * @brief The Theme class
 * @ingroup libsync
 */

class QmlUrlButton
{
    Q_GADGET
    Q_PROPERTY(QString icon MEMBER icon CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(QUrl url MEMBER url CONSTANT)
    QML_VALUE_TYPE(urlbuttondata)

public:
    QmlUrlButton();
    QmlUrlButton(const std::tuple<QString, QString, QUrl> &tuple);

    QString icon;
    QString name;
    QUrl url;
};

class QmlButtonColor
{
    Q_GADGET
    Q_PROPERTY(QColor color MEMBER color CONSTANT)
    Q_PROPERTY(QColor textColor MEMBER textColor CONSTANT)
    Q_PROPERTY(QColor textColorDisabled MEMBER textColorDisabled CONSTANT)
    Q_PROPERTY(bool valid READ valid CONSTANT)
    QML_VALUE_TYPE(buttonColor)

public:
    QColor color = {};
    QColor textColor = {};
    QColor textColorDisabled = {};

    bool valid() const;
};

class OPENCLOUD_SYNC_EXPORT Theme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool multiAccount READ multiAccount FINAL CONSTANT)
    Q_PROPERTY(QList<QmlUrlButton> urlButtons READ qmlUrlButtons FINAL CONSTANT)
    Q_PROPERTY(QColor avatarColor READ avatarColor NOTIFY themeChanged)
    Q_PROPERTY(QColor avatarColorChecked READ avatarColorChecked NOTIFY themeChanged)
    Q_PROPERTY(QColor brandedBackgoundColor READ wizardHeaderBackgroundColor CONSTANT)
    Q_PROPERTY(QColor brandedForegroundColor READ wizardHeaderTitleColor CONSTANT)
    Q_PROPERTY(QmlButtonColor primaryButtonColor READ primaryButtonColor CONSTANT)
    Q_PROPERTY(QmlButtonColor secondaryButtonColor READ secondaryButtonColor CONSTANT)
    QML_SINGLETON
    QML_ELEMENT
public:
    enum class VersionFormat {
        Plain,
        Url,
        RichText,
        OneLiner
    };
    Q_ENUM(VersionFormat);

    /* returns a singleton instance. */
    static Theme *instance();
    static Theme *create(QQmlEngine *qmlEngine, QJSEngine *);

    ~Theme() override;

    /**
     * @brief appNameGUI - Human readable application name.
     *
     * Use and redefine this if the human readable name contains spaces,
     * special chars and such.
     *
     * By default, the name is derived from the APPLICATION_NAME
     * cmake variable.
     *
     * @return QString with human readable app name.
     */
    virtual QString appNameGUI() const;

    /**
     * @brief appName - Application name (short)
     *
     * Use and redefine this as an application name. Keep it straight as
     * it is used for config files etc. If you need a more sophisticated
     * name in the GUI, redefine appNameGUI.
     *
     * By default, the name is derived from the APPLICATION_SHORTNAME
     * cmake variable, and should be the same. This method is only
     * reimplementable for legacy reasons.
     *
     * Warning: Do not modify this value, as many things, e.g. settings
     * depend on it! You most likely want to modify \ref appNameGUI().
     *
     * @return QString with app name.
     */
    QString appName() const;

    QString orgDomainName() const;

    QString vendor() const;

    /**
     * @brief configFileName
     * @return the name of the config file.
     */
    virtual QString configFileName() const;

    /**
     * get an sync state icon
     */

    QIcon themeTrayIcon(const SyncResult &result, Resources::IconType iconType = Resources::IconType::BrandedIconWithFallbackToVanillaIcon) const;

    QString syncStateIconName(const SyncResult &result) const;

    virtual QIcon applicationIcon() const;
    virtual QString applicationIconName() const;
    virtual QIcon aboutIcon() const;

    /**
     * When true, client works with multiple accounts.
     */
    virtual bool multiAccount() const;

    /**
     * URL to documentation.
     *
     * This is opened in the browser when the "Help" action is selected from the tray menu.
     *
     * If the function is overridden to return an empty string the action is removed from
     * the menu.
     */
    virtual QUrl helpUrl() const;

    /**
     * Setting a value here will pre-define the server url.
     *
     * The respective UI controls will be disabled
     *  setting $OPENCLOUD_OVERRIDE_SERVER_URL
     */
    virtual QUrl overrideServerUrl() const;

    /** @return color for the setup wizard */
    virtual QColor wizardHeaderTitleColor() const;

    /** @return color for the setup wizard. */
    virtual QColor wizardHeaderBackgroundColor() const;
    virtual QmlButtonColor primaryButtonColor() const;
    virtual QmlButtonColor secondaryButtonColor() const;

    /** @return logo for the setup wizard. */
    virtual QIcon wizardHeaderLogo() const;

    virtual QColor avatarColor() const;

    virtual QColor avatarColorChecked() const;

    /**
     * The SHA sum of the released git commit
     */
    QString gitSHA1(VersionFormat format = VersionFormat::Plain) const;

    /**
     * The used library versions
     */
    QString aboutVersions(VersionFormat format = VersionFormat::Plain) const;

    /**
     * About dialog contents
     */
    virtual QString about() const;
    virtual bool aboutShowCopyright() const;


    /**
     * @brief Where to check for new Updates.
     */
    QUrl updateCheckUrl() const;

    /**
     * Skip the advanced page and create a sync with the default settings
     */
    virtual bool wizardSkipAdvancedPage() const;

    /**
     * @brief Sharing options
     *
     * Allow link sharing and or user/group sharing
     */
    virtual bool linkSharing() const;
    virtual bool userGroupSharing() const;

    /**
     * If this returns true, the user cannot configure the proxy in the network settings.
     * The proxy settings will be disabled in the configuration dialog.
     * Default returns false.
     */
    virtual bool forceSystemNetworkProxy() const;


    /**
     * @brief Postfix that will be enforced in a URL. e.g.
     *        ".myhosting.com".
     *
     * @return An empty string, unless reimplemented
     */
    virtual QString wizardUrlPostfix() const;


    /**
     * The OAuth client_id, secret pair.
     * Note that client that change these value cannot connect to un-branded OpenCloud.
     */
    virtual QString oauthClientId() const;
    virtual QString oauthClientSecret() const;


    /**
     * By default the client tries to get the OAuth access endpoint and the OAuth token endpoint from /.well-known/openid-configuration
     * Setting this allow authentication without a well known url
     *
     * @return QPair<OAuth access endpoint, OAuth token endpoint>
     */
    virtual QPair<QString, QString> oauthOverrideAuthUrl() const;

    /**
     * List of ports to use for the local redirect server
     */
    virtual QVector<quint16> oauthPorts() const;

    /**
     * Returns the required opeidconnect scopes
     */
    virtual QString openIdConnectScopes() const;

    /**
     * Returns the openidconnect promt type
     * It is supposed to be "consent select_account".
     * For Konnect it currently needs to be select_account,
     * which is the current default.
     */
    virtual QString openIdConnectPrompt() const;

    /**
     * Defines whether the client attempts danamic registration with the IdP or uses the
     * oauthClientId() and oauthClientSecret()
     * Default: True
     */
    virtual bool oidcEnableDynamicRegistration() const;

    /**
     * @brief What should be output for the --version command line switch.
     *
     * By default, it's a combination of appName(), version(), the GIT SHA1 and some
     * important dependency versions.
     */
    virtual QString versionSwitchOutput() const;

    /**
     * @brief Disables all vfs related options, if vfs is still wanted we recommend to set forceVirtualFilesOption
     *
     * default: true
     */
    virtual bool showVirtualFilesOption() const;

    virtual bool forceVirtualFilesOption() const;

    /**
     * Whether to clear cookies before checking status.php
     * This is used with F5 BIG-IP seups.
     */
    virtual bool connectionValidatorClearCookies() const;


    /**
     * Enables the response of V2/GET_CLIENT_ICON, default true.
     * See #9167
     */
    virtual bool enableSocketApiIconSupport() const;


    /**
     * Warn if we find multiple db files in the sync root.
     * This can indicate that the sync dir is shared between multiple clients or accounts
     */
    virtual bool warnOnMultipleDb() const;


    /**
     * Whether to or not to allow multiple sync folder pairs for the same remote folder.
     * Default: true
     */
    virtual bool allowDuplicatedFolderSyncPair() const;

    /**
     * Returns a list of IconName, Name, Url pairs that will be displayed as buttons on AccountSettings.
     * For each url there must be an icon provided in the form of #IconName.svg or multiple #IconName-#resolution.png like for the other theme icons.
     * */
    virtual QVector<std::tuple<QString, QString, QUrl>> urlButtons() const;


    /**
     * Whether or not to enable move-to-trash instead of deleting files that are gone from the server.
     * Default: true
     */
    virtual bool enableMoveToTrash() const;

    /**
     * Whether to enable the special code for cernbox
     * This includes:
     * - spaces migration
     * - support for .sys.admin#recall#
     */
    bool enableCernBranding() const;

    bool withCrashReporter() const;

protected:
    Theme();

    // compat with legacy themes
    [[deprecated("Use Resources::themeUniversalIcon")]] auto themeUniversalIcon(const QString &iconName) const
    {
        return Resources::themeUniversalIcon(iconName);
    }

Q_SIGNALS:
    void themeChanged();

private:
    QList<QmlUrlButton> qmlUrlButtons() const;
    Theme(Theme const &);
    Theme &operator=(Theme const &);

    static Theme *_instance;
};
}
#endif // _THEME_H
