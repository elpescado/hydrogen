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
#include "xml.h"

#include <cppunit/extensions/HelperMacros.h>

#include <QDomDocument>
#include <QProcess>
#include <QTemporaryfile>
#include <QTextStream>

#include "hydrogen/object.h"


/**
 * \brief Compare result of two XML strings
 * \param expected Expected value
 * \param actual   Actual value
 **/
namespace H2Test {
XmlDiff::XmlDiff( const QString &expected, const QString &actual, CppUnit::SourceLine sourceLine )
	: m_sourceLine{ sourceLine }, m_ignoreErrors{ true }
{
	m_expected = normalize( expected, "expected" );
	m_actual = normalize( actual, "actual" );

	m_equal = m_expected == m_actual;
	if (! m_equal ) {
		QTemporaryFile expectedFile;
		dump_xml( m_expected, expectedFile );

		QTemporaryFile actualFile;
		dump_xml( m_actual, actualFile );

		m_diff = diffFiles( expectedFile.fileName(), actualFile.fileName() );
	}
}


/**
 * \brief Normalize XML documents
 * \param in  Input XML document
 * \param ctx Name of document, eg. "actual" or "expected"
 * \throws CppUnit::Exception when input data is malformed
 */
QString XmlDiff::normalize( const QString &in, const QString &ctx )
{
	QDomDocument doc;
	QString errorStr;
	int errorLine;
	int errorCol;
	if(! doc.setContent( in, false, &errorStr, &errorLine, &errorCol )) {
		CppUnit::Message msg(
				QString( "Invalid %1 XML: line %2 col %3" )
					.arg( ctx ).arg( errorLine ).arg( errorCol ).toStdString(),
				errorStr.toStdString());
		throw CppUnit::Exception( msg, m_sourceLine );
	}
	return doc.toString( 2 );
}


/**
 * \brief Write XML data to temporary file
 * \param xml  XML data
 * \param file output file
 */
void XmlDiff::dump_xml( const QString &xml, QTemporaryFile &file )
{
	file.open();
	file.setAutoRemove( false );
	if ( file.isOpen() ) {
		QTextStream outStream ( &file );
		outStream << xml;
	}
	file.flush();
}


/**
 * \brief Create textual diff between two files
 * \param f1 Path to first file (expected)
 * \param f2 Path to second file (actual)
 * \returns QString texual diff in unified diff format
 *
 * Run external command to obtain unified diff between two
 * files. This function tries to run `colordiff` and `diff`
 * utilities, in that order.
 **/
QString XmlDiff::diffFiles( QString f1, QString f2 )
{
	const char *commands[]= { "colordiff", "diff" };

	for ( auto diffCmd : commands ) {
		QProcess proc;
		proc.start( diffCmd, { "-u", "--label", "expected", "--label", "actual", f1, f2 } );
		proc.waitForFinished();
		int exitCode = proc.exitCode();
		if ( exitCode == 255 ) { // command not found
			continue;
		}
		else if ( exitCode > 1 ) { // diff failure
			fail( QString( "%1 returned %1 code" ).arg( diffCmd ).arg( exitCode ) );
			continue;
		}
		auto diffs = proc.readAllStandardOutput();
		return QString( diffs ).trimmed();
	}
	fail( "No suitable diff utility found" );
	return "";
}


/**
 * \brief Signal a failure
 * \param msg Error message
 **/
void XmlDiff::fail( const QString &msg )
{
	if ( m_ignoreErrors ) {
		___WARNINGLOG( QString( msg ) );
	}
	else {
		___ERRORLOG( QString( msg ) );
		throw std::runtime_error( msg.toLocal8Bit() );
	}
}


void checkXmlEqual( const QString &expected, const QString &actual, CppUnit::SourceLine sourceLine )
{
	XmlDiff diff( expected, actual, sourceLine );

	if (! diff.equal() ) {
		QString diffData = diff.diff();

		CppUnit::Message msg( QString( "XML equality assertion failed:\n%1" )
								.arg( diffData ).toStdString());
		throw CppUnit::Exception( msg, sourceLine );
	}
}



}





