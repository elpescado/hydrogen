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

#include <hydrogen/automation/manager.h>
#include <hydrogen/automation/controller.h>
#include "fake_objects.h"

#include <memory>

using namespace H2Core;

class AutomationManagerTest : public CppUnit::TestCase {
	CPPUNIT_TEST_SUITE( AutomationManagerTest );
	CPPUNIT_TEST( test_create_manager );
	CPPUNIT_TEST( test_can_add_controllers );
	CPPUNIT_TEST( test_iterate );
	CPPUNIT_TEST( test_get_controller_by_index );
	CPPUNIT_TEST_SUITE_END();

	public:
	void test_create_manager()
	{
		AutomationManager man;
	}


	void test_can_add_controllers()
	{
		AutomationManager man;
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));
	}

	void test_iterate()
	{
		AutomationManager man;
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));
		
		int count = 0;
		for (auto &ctrl : man) {
			++count;
		}
		CPPUNIT_ASSERT_EQUAL(2, count);
	}

	void test_get_controller_by_index()
	{
		AutomationManager man;
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));
		man.add_controller( std::unique_ptr<AutomationController>( new FakeController ));

		AutomationController &ctrl = man.get_controller(0);
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( AutomationManagerTest );
