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
#ifndef H2CORE_AUTOMATION_FACTORY_H
#define H2CORE_AUTOMATION_FACTORY_H

#include <hydrogen/automation/controller.h>

#include <QString>
#include <memory>
#include <functional>
#include <vector>


namespace H2Core {

class Constructor {
	public:
	using controller_t = std::unique_ptr<AutomationController>;
	
	public:

	template<class T>
	Constructor( QString label, T ctor )
		: m_label{ label }, m_constructor{ ctor }
	{
	}

	QString label() const { return m_label; }
	controller_t operator()() const { return m_constructor(); }
	
	private:
	QString m_label;
	std::function<controller_t()> m_constructor;
};


class AutomationFactory {
	public:
	std::vector<Constructor> enumerate();

	private:
	std::vector<Constructor> m_ctors;

	void update();

	template<class C, class... Args>
	void register_ctrl(Args... a)
	{
		m_ctors.emplace_back(C::static_label(a...), [=](){ return std::unique_ptr<AutomationController>( new C(a...) ); });
	}
};

}

#endif
