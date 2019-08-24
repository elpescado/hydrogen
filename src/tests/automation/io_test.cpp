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
#include "assertions/xml.h"

using namespace H2Core;

const char *expected_xml = 
"<song>"
"  <automationPaths>"
"     <path adjust='fake'/>"
"     <path adjust='fake'/>"
"  </automationPaths>"
"</song>";



class AutomationIOTest : public CppUnit::TestCase {
	CPPUNIT_TEST_SUITE( AutomationIOTest );
	CPPUNIT_TEST( test_write );
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

        H2TEST_ASSERT_XML_EQUAL( expected_xml, doc.toString());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( AutomationIOTest );
