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

#include <hydrogen/automation/controller.h>

using namespace H2Core;


class AutomationControllerStub : public AutomationController
{
	float _value = -42;

	public:
	void adjust(float value) override
	{
		_value = value;
	}
    QString name() const override { return ""; }

	float value() const { return _value; }
};


class AutomationControllerTest : public CppUnit::TestCase {
	CPPUNIT_TEST_SUITE( AutomationControllerTest );
	CPPUNIT_TEST( test_nothing );
	CPPUNIT_TEST_SUITE_END();

	const double delta = 0.0001;

	public:
	void test_nothing()
	{
		AutomationControllerStub controller;

		CPPUNIT_ASSERT( controller.path().get_max() );

		controller.process(0.0f);

		CPPUNIT_ASSERT_DOUBLES_EQUAL( 
				1.0,
				static_cast<double>(controller.value()),
				delta
		);
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( AutomationControllerTest );
