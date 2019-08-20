/*
 * Hydrogen
 * Copyright(c) 2002-2019 by the Hydrogen Team
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

#ifndef ASSERTIONS_XML_H
#define ASSERTIONS_XML_H

#include <QString>
#include <cppunit/SourceLine.h>
#include <QTemporaryfile>

namespace H2Test {

	class XmlDiff
	{
		CppUnit::SourceLine m_sourceLine;
		QString m_expected;
		QString m_actual;
		bool m_equal;
		bool m_ignoreErrors;
		QString m_diff;

		public:
		XmlDiff(const QString &expected, const QString &actual, CppUnit::SourceLine sourceLine=CppUnit::SourceLine());

		/** \brief Return whether two documents are equal */
		bool equal() const { return m_equal; }

		/** \brief Differences in unified diff format */
		QString diff() const { return m_diff; }

		private:
		QString normalize(const QString &in, const QString &ctx );
		void dump_xml( const QString &xml, QTemporaryFile &file );
		QString diffFiles( QString f1, QString f2);
		void fail( const QString &msg );
	};
	
	void checkXmlEqual(const QString &expected, const QString &actual, CppUnit::SourceLine sourceLine);
}


/**
 * \brief Perform XML equality assertion
 */
#define H2TEST_ASSERT_XML_EQUAL(expected, actual) \
	H2Test::checkXmlEqual(expected, actual, CPPUNIT_SOURCELINE())

#endif
