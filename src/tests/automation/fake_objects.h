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
#ifndef TEST_FAKE_OBJECTS_H
#define TEST_FAKE_OBJECTS_H

#include <hydrogen/automation/controller.h>

class FakeController : public H2Core::AutomationController
{
	bool _was_called = false;
	public:
	void adjust(float value) override
	{
		_was_called = true;
	}

	QString name() const override { return "fake"; }

	bool was_called() const { return _was_called; };
};


#endif
