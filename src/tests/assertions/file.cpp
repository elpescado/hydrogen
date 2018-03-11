/*
 * Hydrogen
 * Copyright(c) 2002-2018 by the Hydrogen Team
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

#include "file.h"

#include <QFile>

static constexpr qint64 BUFFER_SIZE = 5;


void H2Test::checkFilesEqual(const QString &expected, const QString &actual, CppUnit::SourceLine sourceLine)
{
	QFile f1(expected);
	QFile f2(actual);

	if (! f1.open(QIODevice::ReadOnly)) {
		CppUnit::Message msg(
			std::string("Can't open reference file: ") + f1.errorString().toStdString(),
			std::string("Expected: ") + expected.toStdString() );
		throw CppUnit::Exception(msg, sourceLine);
	}
	if (! f2.open(QIODevice::ReadOnly)) {
		CppUnit::Message msg(
			std::string("Can't open result file: ") + f2.errorString().toStdString(),
			std::string("Actual  : ") + actual.toStdString() );
		throw CppUnit::Exception(msg, sourceLine);
	}
	if ( f1.size() != f2.size() ) {
		CppUnit::Message msg(
			"File size differ",
			std::string("Expected: ") + expected.toStdString(),
			std::string("Actual  : ") + actual.toStdString() );
		throw CppUnit::Exception(msg, sourceLine);
	}

	auto remaining = f1.size();
	qint64 offset = 0;
	while ( remaining > 0 ) {
		char buf1[BUFFER_SIZE];
		char buf2[BUFFER_SIZE];

		qint64 toRead = qMin( remaining, (qint64)BUFFER_SIZE );
		auto r1 = f1.read( buf1, toRead );
		auto r2 = f2.read( buf2, toRead );

		for (int i = 0; i < r1; i++) {
			if ( buf1[i] != buf2[i] ) {
				auto diffLocation = offset + i + 1;
				CppUnit::Message msg(
					std::string("Files differ at byte ") + std::to_string(diffLocation),
					std::string("Expected: ") + expected.toStdString(),
					std::string("Actual  : ") + actual.toStdString() );
				throw CppUnit::Exception(msg, sourceLine);
			}
		}

		offset += r1;
		remaining -= r1;
	}

	if ( code != 0 ) {
		CppUnit::Message msg(
			"files differ",
			std::string("Expected: ") + expected.toStdString(),
			std::string("Actual  : ") + actual.toStdString() );
		throw CppUnit::Exception(msg, sourceLine);
	}
}
