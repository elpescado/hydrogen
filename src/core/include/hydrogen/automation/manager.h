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
#ifndef H2CORE_AUTOMATION_MANAGER_H
#define H2CORE_AUTOMATION_MANAGER_H

#include <memory>
#include <vector>

#include <hydrogen/automation/controller.h>

#include <QDebug>
#include <QDomNode>

namespace H2Core {

class AutomationManager {
	std::vector<std::unique_ptr<AutomationController>> m_controllers;

	public:
	
	int add_controller(std::unique_ptr<AutomationController> &&ctrl)
	{
		m_controllers.push_back(std::move(ctrl));
		return m_controllers.size() - 1;
	}

	int add_controller(std::unique_ptr<AutomationController> &&ctrl, int idx)
	{
		m_controllers.insert(m_controllers.begin() + idx, std::move(ctrl) );
		return idx;
	}

	void process(float ticks);

	decltype(m_controllers.begin()) begin() { return m_controllers.begin(); }
	decltype(m_controllers.end()) end() { return m_controllers.end(); }
	decltype(m_controllers.cbegin()) begin() const { return m_controllers.begin(); }
	decltype(m_controllers.cend()) end() const { return m_controllers.end(); }
	decltype(m_controllers.cbegin()) cbegin() const { return m_controllers.cbegin(); }
	decltype(m_controllers.cend()) cend() const { return m_controllers.cend(); }

	int size() const { return m_controllers.size(); }
	AutomationController& get_controller(int idx) { return *m_controllers[idx]; }
	
	std::unique_ptr<AutomationController> remove( int idx ) {
		auto ctrl = std::move( m_controllers[idx] ); 
		m_controllers.erase( m_controllers.begin() + idx );
		return ctrl;
	}
};

}

#endif
