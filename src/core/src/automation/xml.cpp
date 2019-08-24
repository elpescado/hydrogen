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
#include <hydrogen/automation/xml.h>

#include <QDomDocument>
#include <QDomElement>

#include <hydrogen/basics/automation_path.h>
#include <hydrogen/automation_path_serializer.h>
#include <hydrogen/automation/controller_library.h>

namespace H2Core {

AutomationWriter::AutomationWriter( QDomNode &parent )
	: m_parentNode{ parent }
{
	m_doc = m_parentNode.ownerDocument();
}

void AutomationWriter::save( const AutomationManager &mgr)
{
	AutomationPathSerializer pathSerializer;
	QDomNode automationPathsTag = m_doc.createElement( "automationPaths" );
	for ( auto const &ctrl : mgr ) {
		QDomElement path = create_path_node( *ctrl );
		pathSerializer.write_automation_path( path, ctrl->path() );
		automationPathsTag.appendChild( path );
	}
	m_parentNode.appendChild( automationPathsTag );
}
QDomElement AutomationWriter::create_path_node( const AutomationController &ctrl)
{
	QDomElement path = m_doc.createElement( "path" );
	path.setAttribute( "adjust", ctrl.name() );

	if ( const InstrumentVolumeController *ivc = dynamic_cast<const InstrumentVolumeController*>(&ctrl) ) {
		int instrument_id = ivc->instrument_id();
		path.setAttribute( "instr", QString::number( instrument_id ));
	}

	if ( const InstrumentPanController *ipc = dynamic_cast<const InstrumentPanController*>(&ctrl) ) {
		int instrument_id = ipc->instrument_id();
		path.setAttribute( "instr", QString::number( instrument_id ));
	}

	return path;
}


};
