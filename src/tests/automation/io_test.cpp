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

#include <QDomDocument>
#include <QDomElement>

#include <hydrogen/automation/controller.h>
#include <hydrogen/automation/manager.h>
#include <hydrogen/automation/xml.h>
#include "fake_objects.h"
#include "test_helper.h"
#include "assertions/xml.h"

using namespace H2Core;

static const char *EXPECTED_SIMPLE_XML =
"<song>"
"  <automationPaths>"
"     <path adjust='fake'/>"
"     <path adjust='fake'/>"
"  </automationPaths>"
"</song>";

static const char *EXPECTED_XML_WITH_POINTS =
"<song>"
"  <automationPaths>"
"     <path adjust='fake'>"
"       <point x='2' y='1'/>"
"     </path>"
"  </automationPaths>"
"</song>";


class TestAutomationReader : public AutomationReader {
	public:
	TestAutomationReader( QDomElement node ) : AutomationReader( node ) {}

	protected:
	std::unique_ptr<AutomationController> create_controller( QDomElement node ) override
	{
		return std::unique_ptr<AutomationController>( new FakeController() );
	}
};


class AutomationIOTest : public CppUnit::TestCase {
	CPPUNIT_TEST_SUITE( AutomationIOTest );
	CPPUNIT_TEST( test_write );
	CPPUNIT_TEST( test_read );
	CPPUNIT_TEST( test_read_points );
	CPPUNIT_TEST_SUITE_END();

    public:
    void test_write()
    {
		AutomationManager man;
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));

        QDomDocument doc;
        QDomElement root = doc.createElement( "song" );
        doc.appendChild( root );

        AutomationWriter writer( root );
        writer.save( man );

        H2TEST_ASSERT_XML_EQUAL( EXPECTED_SIMPLE_XML, doc.toString());
    }

	void test_read()
	{
        QDomDocument doc;
		doc.setContent( QString( EXPECTED_SIMPLE_XML ) );

		QDomElement el = doc.documentElement().firstChildElement( "automationPaths" );

		TestAutomationReader reader ( el );
		auto mgr = reader.read();

		CPPUNIT_ASSERT( mgr != nullptr );
		CPPUNIT_ASSERT_EQUAL( 2, mgr->size() );
		CPPUNIT_ASSERT_EQUAL( QString("fake").toStdString(), mgr->get_controller(0).name().toStdString() );
		CPPUNIT_ASSERT_EQUAL( QString("fake").toStdString(), mgr->get_controller(1).name().toStdString() );
	}

	void test_read_points()
	{
        QDomDocument doc;
		doc.setContent( QString( EXPECTED_XML_WITH_POINTS ) );

		QDomElement el = doc.documentElement().firstChildElement( "automationPaths" );

		TestAutomationReader reader ( el );
		auto mgr = reader.read();

		CPPUNIT_ASSERT( mgr != nullptr );
		CPPUNIT_ASSERT_EQUAL( 1, mgr->size() );
		auto &ctrl = mgr->get_controller(0);
		CPPUNIT_ASSERT_EQUAL( QString("fake").toStdString(), ctrl.name().toStdString() );
		//get point
		auto p = ctrl.path().begin();
		CPPUNIT_ASSERT( p != ctrl.path().end() );
		typedef std::pair<const float,float> pair;
		CPPUNIT_ASSERT_EQUAL(pair(2.f, 1.f), *p);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( AutomationIOTest );
