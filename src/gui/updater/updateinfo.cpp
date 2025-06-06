// This file is generated by kxml_compiler from occinfo.xml.

#include "updateinfo.h"
#include "updater.h"

#include <QtDebug>
#include <QFile>
#include <QDomDocument>
#include <QtCore/QtDebug>
#include <QtCore/QFile>

namespace OCC {

void UpdateInfo::setVersion(const QString &v)
{
    mVersion = v;
}

QString UpdateInfo::version() const
{
    return mVersion;
}

void UpdateInfo::setVersionString(const QString &v)
{
    mVersionString = v;
}

QString UpdateInfo::versionString() const
{
    return mVersionString;
}

void UpdateInfo::setWeb(const QString &v)
{
    mWeb = v;
}

QString UpdateInfo::web() const
{
    return mWeb;
}

void UpdateInfo::setDownloadUrl(const QString &v)
{
    mDownloadUrl = v;
}

QString UpdateInfo::downloadUrl() const
{
    return mDownloadUrl;
}

UpdateInfo UpdateInfo::parseElement(const QDomElement &element, bool *ok)
{
    if (element.tagName() != QLatin1String("OpenCloud")) {
        qCCritical(lcUpdater) << "Expected 'OpenCloud', got '" << element.tagName() << "'.";
        if (ok)
            *ok = false;
        return UpdateInfo();
    }

    UpdateInfo result = UpdateInfo();

    QDomNode n;
    for (n = element.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement e = n.toElement();
        if (e.tagName() == QLatin1String("version")) {
            result.setVersion(e.text());
        } else if (e.tagName() == QLatin1String("versionstring")) {
            result.setVersionString(e.text());
        } else if (e.tagName() == QLatin1String("web")) {
            result.setWeb(e.text());
        } else if (e.tagName() == QLatin1String("downloadurl")) {
            result.setDownloadUrl(e.text());
        }
    }


    if (ok)
        *ok = true;
    return result;
}

UpdateInfo UpdateInfo::parseString(const QString &xml, bool *ok)
{
    QString errorMsg;
    int errorLine, errorCol;
    QDomDocument doc;
    if (!doc.setContent(xml, false, &errorMsg, &errorLine, &errorCol)) {
        qCWarning(lcUpdater).noquote().nospace() << errorMsg << " at " << errorLine << "," << errorCol << "\n"
                                                 << xml.split(QStringLiteral("\n")).value(errorLine - 1) << "\n"
                                                 << QStringLiteral(" ").repeated(errorCol - 1) << "^\n"
                                                 << "->" << xml << "<-";
        if (ok)
            *ok = false;
        return UpdateInfo();
    }

    bool documentOk;
    UpdateInfo c = parseElement(doc.documentElement(), &documentOk);
    if (ok) {
        *ok = documentOk;
    }
    return c;
}

} // namespace OCC
