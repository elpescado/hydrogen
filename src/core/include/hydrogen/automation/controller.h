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
#ifndef H2CORE_AUTOMATION_CONTROLLER_H
#define H2CORE_AUTOMATION_CONTROLLER_H

#include <memory>

#include <hydrogen/basics/automation_path.h>
#include <QString>

namespace H2Core {

class AutomationController {

	std::unique_ptr<AutomationPath> m_pPath;

	public:
	AutomationController();
	virtual ~AutomationController() {}

	AutomationPath &path() const;

	void process( float ticks );
	
	virtual void adjust( float value ) = 0;
	virtual QString name() const = 0;
	virtual QString label() const;
};

}

#endif
