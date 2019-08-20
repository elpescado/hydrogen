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
#include <cppunit/extensions/HelperMacros.h>

#include "assertions/xml.h"

// Tests for various test utilities

bool xml_equal(const QString &expected, const QString &actual)
{
	return H2Test::XmlDiff( expected, actual ).equal();
}


/** \brief Test XML assertions */
class XmlTestUtilsTest : public CppUnit::TestCase {
	CPPUNIT_TEST_SUITE( XmlTestUtilsTest );
	CPPUNIT_TEST( test_simple_equal );
	CPPUNIT_TEST( test_simple_differ );
	CPPUNIT_TEST( test_ignore_whitespace );
	CPPUNIT_TEST( test_ignore_attr_order );
	CPPUNIT_TEST( test_invalid_xml );
	CPPUNIT_TEST( test_ignore_quotation_marks );
	CPPUNIT_TEST_SUITE_END();

	public:
	void test_simple_equal()
	{
		CPPUNIT_ASSERT( xml_equal( "<a/>", "<a/>" ) );
	}


	void test_simple_differ()
	{
		CPPUNIT_ASSERT(! xml_equal( "<a/>", "<b/>" ) );
	}


	void test_ignore_whitespace()
	{
		CPPUNIT_ASSERT( xml_equal ("<a><b/></a>", "<a>\n  <b/>\n</a>") );
	}


	void test_ignore_attr_order()
	{
		CPPUNIT_ASSERT( xml_equal ("<a foo='1' bar='2'/>", "<a bar='2' foo='1'/>") );
	}


	void test_ignore_quotation_marks()
	{
		CPPUNIT_ASSERT( xml_equal ("<a foo='1'/>", "<a foo=\"1\"/>") );
	}


	void test_invalid_xml()
	{
		CPPUNIT_ASSERT_THROW(
			xml_equal( "<a", "<a/>"),
			CppUnit::Exception);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( XmlTestUtilsTest );
