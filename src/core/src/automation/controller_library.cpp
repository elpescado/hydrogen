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
#include <hydrogen/automation/controller_library.h>

#include <hydrogen/hydrogen.h>
#include <hydrogen/basics/song.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>

namespace H2Core {

MasterVolumeController::MasterVolumeController()
	: AutomationController{}
{
}

const char* MasterVolumeController::m_name = "master_volume";


void MasterVolumeController::adjust( float value )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *song = pEngine->getSong();
	song->set_volume( value );
}


QString MasterVolumeController::label() const
{
	return static_label();
}

QString MasterVolumeController::name() const
{
	return m_name;
}


QString MasterVolumeController::static_label()
{
	return "Master volume";
}



InstrumentVolumeController::InstrumentVolumeController(int id)
	: AutomationController{}, m_nInstrId{ id }
{
}

const char* InstrumentVolumeController::m_name = "instrument_volume";


void InstrumentVolumeController::adjust( float value )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *song = pEngine->getSong();
	InstrumentList *instrList = song->get_instrument_list();
	Instrument *instr = instrList->get( m_nInstrId );
	instr->set_volume( value );
}


QString InstrumentVolumeController::label() const
{
	return static_label( m_nInstrId );
}

QString InstrumentVolumeController::name() const
{
	return m_name;
}


QString InstrumentVolumeController::static_label( int id )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *song = pEngine->getSong();
	InstrumentList *instrList = song->get_instrument_list();
	Instrument *instr = instrList->get( id );
	return QString("%1 volume").arg(instr->get_name());
}


InstrumentPanController::InstrumentPanController(int id)
	: AutomationController{}, m_nInstrId{ id }
{
}

const char* InstrumentPanController::m_name = "instrument_pan";


void InstrumentPanController::adjust( float panValue )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *song = pEngine->getSong();
	InstrumentList *instrList = song->get_instrument_list();
	Instrument *instr = instrList->get( m_nInstrId );

	if (panValue > 0.5) {
		instr->set_pan_l( (1.0 - panValue) * 2.0 );
		instr->set_pan_r( 1.0 );
	} else {
		instr->set_pan_l( 1.0 );
		instr->set_pan_r( panValue * 2.0 );
	}
}


QString InstrumentPanController::label() const
{
	return static_label( m_nInstrId );
}

QString InstrumentPanController::name() const
{
	return m_name;
}

QString InstrumentPanController::static_label( int id )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *song = pEngine->getSong();
	InstrumentList *instrList = song->get_instrument_list();
	Instrument *instr = instrList->get( id );
	return QString( "%1 pan" ).arg(instr->get_name());
}


}
