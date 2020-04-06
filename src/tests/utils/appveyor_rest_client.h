#ifndef APPVEYOR_H
#define APPVEYOR_H

/*
 * Hydrogen
 * Copyright(c) 2002-2020 by the Hydrogen Team
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUrl>

namespace AppVeyor {

enum class TestOutcome {
    None,
    Running,
    Passed,
    Failed,
    Ignored,
    Skipped,
    Inconclusive,
    NotFound,
    Cancelled,
    NotRunnable
};


struct Test {
    QString name;
    QString framework;
    QString fileName;
    TestOutcome outcome;
    int duration = 0;
    QString errorMessage;
    QString errorStackTrace;
    QString stdOut;
    QString stdErr;

    Test();
    Test(const QString &name, const QString &framework, const QString &fileName, TestOutcome outcome = TestOutcome::None);
    void toJson(QJsonObject &json) const;
};


class BuildWorkerApiClient {
    public:
    BuildWorkerApiClient();
    void addTest(const Test &testData);
    void updateTest(const Test &testData);

    private:
    bool m_bEnabled;
    QUrl m_ApiRoot;

    void doSyncRequest(QByteArray method, QString endpoint, const QJsonDocument &payload);
};

};

#endif
