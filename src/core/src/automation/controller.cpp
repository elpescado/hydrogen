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
#include <hydrogen/automation/controller.h>

#include <QDebug>

namespace H2Core {

AutomationController::AutomationController()
	: m_pPath{ new AutomationPath( 0.0f, 1.0f, 1.0f ) }
{
}

AutomationPath& AutomationController::path() const
{
	return *m_pPath;
}


void AutomationController::process( float ticks )
{
	float value = m_pPath->get_value( ticks );
	adjust( value );
}

QString AutomationController::label() const {
	return "";
}


}
