/*
 * Hydrogen
 * Copyright(c) 2017 by Sebastian Moors
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

#include <hydrogen/core_action_controller.h>
#include <hydrogen/event_queue.h>
#include <hydrogen/hydrogen.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>
#include <hydrogen/osc_server.h>
#include <hydrogen/midi_action.h>
#include <hydrogen/midi_map.h>

#include <hydrogen/IO/AlsaMidiDriver.h>
#include <hydrogen/IO/MidiOutput.h>

namespace H2Core
{

const char* CoreActionController::__class_name = "CoreActionController";


CoreActionController::CoreActionController() : Object( __class_name ),
												m_nDefaultMidiFeedbackChannel(0)
{
	//nothing
}

CoreActionController::~CoreActionController() {
	//nothing
}

void CoreActionController::setMasterVolume( float masterVolumeValue )
{
	Hydrogen* pEngine = Hydrogen::get_instance();
	pEngine->getSong()->set_volume( masterVolumeValue );
	
#ifdef H2CORE_HAVE_OSC
	Action FeedbackAction( "MASTER_VOLUME_ABSOLUTE" );
	FeedbackAction.setParameter2( QString("%1").arg( masterVolumeValue ) );
	OscServer::handleAction( &FeedbackAction );
#endif
	
	MidiMap*	pMidiMap = MidiMap::get_instance();
	
	int ccParamValue = pMidiMap->findCCValueByActionType( QString("MASTER_VOLUME_ABSOLUTE"));
	
	handleOutgoingControlChange( ccParamValue, (masterVolumeValue / 1.5) * 127 );
}

void CoreActionController::setStripVolume( int nStrip, float masterVolumeValue )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	pEngine->setSelectedInstrumentNumber( nStrip );
	
	Song *pSong = pEngine->getSong();
	InstrumentList *instrList = pSong->get_instrument_list();

	Instrument *pInstr = instrList->get( nStrip );
	pInstr->set_volume( masterVolumeValue );
	
#ifdef H2CORE_HAVE_OSC
	Action FeedbackAction( "STRIP_VOLUME_ABSOLUTE" );
	
	FeedbackAction.setParameter1( QString("%1").arg( nStrip + 1 ) );
	FeedbackAction.setParameter2( QString("%1").arg( masterVolumeValue ) );
	OscServer::handleAction( &FeedbackAction );
#endif

	MidiMap*	pMidiMap = MidiMap::get_instance();
	
	int ccParamValue = pMidiMap->findCCValueByActionParam1( QString("STRIP_VOLUME_ABSOLUTE"), QString("%1").arg( nStrip ) );
	

	handleOutgoingControlChange( ccParamValue, (masterVolumeValue / 1.5) * 127 );

}

void CoreActionController::setMetronomeIsActive( bool isActive )
{
	Preferences::get_instance()->m_bUseMetronome = isActive;
	
#ifdef H2CORE_HAVE_OSC
	Action FeedbackAction( "TOGGLE_METRONOME" );
	
	FeedbackAction.setParameter1( QString("%1").arg( (int) isActive ) );
	OscServer::handleAction( &FeedbackAction );
#endif
	
	MidiMap*	pMidiMap = MidiMap::get_instance();
	
	int ccParamValue = pMidiMap->findCCValueByActionType( QString("TOGGLE_METRONOME"));
	
	handleOutgoingControlChange( ccParamValue, (int) isActive * 127 );
}

void CoreActionController::setMasterIsMuted( bool isMuted )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	pEngine->getSong()->__is_muted = isMuted;
	
#ifdef H2CORE_HAVE_OSC
	Action FeedbackAction( "MUTE_TOGGLE" );
	
	FeedbackAction.setParameter1( QString("%1").arg( (int) isMuted ) );
	OscServer::handleAction( &FeedbackAction );
#endif

	MidiMap*	pMidiMap = MidiMap::get_instance();
	
	int ccParamValue = pMidiMap->findCCValueByActionType( QString("MUTE_TOGGLE") );

	handleOutgoingControlChange( ccParamValue, (int) isMuted * 127 );
}

void CoreActionController::setStripIsMuted( int nStrip, bool isMuted )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *pSong = pEngine->getSong();
	InstrumentList *pInstrList = pSong->get_instrument_list();

	Instrument *pInstr = pInstrList->get( nStrip );
	pInstr->set_muted( isMuted );
	
#ifdef H2CORE_HAVE_OSC
	Action FeedbackAction( "STRIP_MUTE_TOGGLE" );
	
	FeedbackAction.setParameter1( QString("%1").arg( nStrip + 1 ) );
	FeedbackAction.setParameter2( QString("%1").arg( (int) isMuted ) );
	OscServer::handleAction( &FeedbackAction );
#endif

	MidiMap*	pMidiMap = MidiMap::get_instance();
	
	int ccParamValue = pMidiMap->findCCValueByActionParam1( QString("STRIP_MUTE_TOGGLE"), QString("%1").arg( nStrip ) );
	
	handleOutgoingControlChange( ccParamValue, ((int) isMuted) * 127 );
}

void CoreActionController::setStripIsSoloed( int nStrip, bool isSoloed )
{
	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *pSong = pEngine->getSong();
	InstrumentList *pInstrList = pSong->get_instrument_list();
	
	if ( isSoloed ) {
		for ( int i = 0; i < pInstrList->size(); ++i ) {
			setStripIsMuted( i, true );
		}

		setStripIsMuted( nStrip, false );
	} else {
		for ( int i = 0; i < pInstrList->size(); ++i ) {
			setStripIsMuted( i, false );
		}
	}
	
#ifdef H2CORE_HAVE_OSC
	Action FeedbackAction( "STRIP_SOLO_TOGGLE" );
	
	FeedbackAction.setParameter1( QString("%1").arg( nStrip + 1 ) );
	FeedbackAction.setParameter2( QString("%1").arg( (int) isSoloed ) );
	OscServer::handleAction( &FeedbackAction );
#endif
	
	MidiMap*	pMidiMap = MidiMap::get_instance();
	
	int ccParamValue = pMidiMap->findCCValueByActionParam1( QString("STRIP_SOLO_TOGGLE"), QString("%1").arg( nStrip ) );
	
	handleOutgoingControlChange( ccParamValue, ((int) isSoloed) * 127 );
}



void CoreActionController::setStripPan( int nStrip, float panValue )
{
	float	pan_L;
	float	pan_R;

	if (panValue >= 0.5) {
		pan_L = (1.0 - panValue) * 2;
		pan_R = 1.0;
	}
	else {
		pan_L = 1.0;
		pan_R = panValue * 2;
	}

	Hydrogen *pEngine = Hydrogen::get_instance();
	pEngine->setSelectedInstrumentNumber( nStrip );
	
	Song *pSong = pEngine->getSong();
	InstrumentList *pInstrList = pSong->get_instrument_list();

	Instrument *pInstr = pInstrList->get( nStrip );
	pInstr->set_pan_l( pan_L );
	pInstr->set_pan_r( pan_R );

	pEngine->setSelectedInstrumentNumber( nStrip );
	
#ifdef H2CORE_HAVE_OSC
	Action FeedbackAction( "PAN_ABSOLUTE" );
	
	FeedbackAction.setParameter1( QString("%1").arg( nStrip + 1 ) );
	FeedbackAction.setParameter2( QString("%1").arg( panValue ) );
	OscServer::handleAction( &FeedbackAction );
#endif
	
	MidiMap*	pMidiMap = MidiMap::get_instance();
	
	int ccParamValue = pMidiMap->findCCValueByActionParam1( QString("PAN_ABSOLUTE"), QString("%1").arg( nStrip ) );
	

	handleOutgoingControlChange( ccParamValue, panValue * 127 );
}

void CoreActionController::handleOutgoingControlChange(int param, int value)
{
	Preferences *pPref = Preferences::get_instance();
	Hydrogen *pEngine = Hydrogen::get_instance();
	MidiOutput *pMidiDriver = pEngine->getMidiOutput();
	
	if(	pMidiDriver 
		&& pPref->m_bEnableMidiFeedback 
		&& param >= 0 ){
		pMidiDriver->handleOutgoingControlChange( param, value, m_nDefaultMidiFeedbackChannel );
	}
}

void CoreActionController::initExternalControlInterfaces()
{
	/*
	 * Push the current state of Hydrogen to the attached control interfaces (e.g. OSC clients)
	 */
	
	//MASTER_VOLUME_ABSOLUTE
	Hydrogen* pEngine = Hydrogen::get_instance();
	Song *pSong = pEngine->getSong();
	setMasterVolume( pSong->get_volume() );
	
	//PER-INSTRUMENT/STRIP STATES
	InstrumentList *pInstrList = pSong->get_instrument_list();
	for(int i=0; i < pInstrList->size(); i++){
		
			//STRIP_VOLUME_ABSOLUTE
			Instrument *pInstr = pInstrList->get( i );
			setStripVolume( i, pInstr->get_volume() );
			
			float fPan_L = pInstr->get_pan_l();
			float fPan_R = pInstr->get_pan_r();

			//PAN_ABSOLUTE
			float fPanValue = 0.0;
			if (fPan_R == 1.0) {
				fPanValue = 1.0 - (fPan_L / 2.0);
			}
			else {
				fPanValue = fPan_R / 2.0;
			}
		
			setStripPan( i, fPanValue );
			
			//STRIP_MUTE_TOGGLE
			setStripIsMuted( i, pInstr->is_muted() );
			
			//SOLO
			setStripIsSoloed( i, pInstr->is_soloed() );
	}
	
	//TOGGLE_METRONOME
	setMetronomeIsActive( Preferences::get_instance()->m_bUseMetronome );
	
	//MUTE_TOGGLE
	setMasterIsMuted( Hydrogen::get_instance()->getSong()->__is_muted );
}

bool CoreActionController::newSong( const QString& songPath ) {
	
	auto pHydrogen = Hydrogen::get_instance();

	if ( pHydrogen->getState() == STATE_PLAYING ) {
		// Stops recording, all queued MIDI notes, and the playback of
		// the audio driver.
		pHydrogen->sequencer_stop();
	}
	
	// Remove all BPM tags on the Timeline.
	pHydrogen->getTimeline()->m_timelinevector.clear();
	
	// Create an empty Song.
	auto pSong = Song::get_empty_song();
	
	// Check whether the provided path is valid.
	if ( !isSongPathValid( songPath ) ) {
		// isSongPathValid takes care of the error log message.

		return false;
	}
	
	pSong->set_filename( songPath );
	
	if ( pHydrogen->getActiveGUI() ) {
		
		// Store the prepared Song for the GUI to access after the
		// EVENT_UPDATE_SONG event was triggered.
		pHydrogen->setNextSong( pSong );
		
		// If the GUI is active, the Song *must not* be set by the
		// core part itself.
		// Triggers an update of the Qt5 GUI and tells it to update
		// the song itself.
		EventQueue::get_instance()->push_event( EVENT_UPDATE_SONG, 0 );
		
	} else {

		// Update the Song.
		pHydrogen->setSong( pSong );
		
	}
	
	return true;
}

bool CoreActionController::openSong (const QString& songPath ) {
	
	auto pHydrogen = Hydrogen::get_instance();
 
	if ( pHydrogen->getState() == STATE_PLAYING ) {
		// Stops recording, all queued MIDI notes, and the playback of
		// the audio driver.
		pHydrogen->sequencer_stop();
	}
	
	// Remove all BPM tags on the Timeline.
	pHydrogen->getTimeline()->m_timelinevector.clear();
	
	// Check whether the provided path is valid.
	if ( !isSongPathValid( songPath ) ) {
		// isSongPathValid takes care of the error log message.
		return false;
	}
	
	QFileInfo songFileInfo = QFileInfo( songPath );
	if ( !songFileInfo.exists() ) {
		ERRORLOG( QString( "Selected song [%1] does not exist." )
				 .arg( songPath ) );
		return false;
	}
	
	// Create an empty Song.
	auto pSong = Song::load( songPath );
	
	if ( pSong == nullptr ) {
		ERRORLOG( QString( "Unable to open song [%1]." )
				  .arg( songPath ) );
		
		return false;
	}
	
	if ( pHydrogen->getActiveGUI() ) {
		
		// Store the prepared Song for the GUI to access after the
		// EVENT_UPDATE_SONG event was triggered.
		pHydrogen->setNextSong( pSong );
		
		// If the GUI is active, the Song *must not* be set by the
		// core part itself.
		// Triggers an update of the Qt5 GUI and tells it to update
		// the song itself.
		EventQueue::get_instance()->push_event( EVENT_UPDATE_SONG, 0 );
		
	} else {

		// Update the Song.
		pHydrogen->setSong( pSong );
		
	}
	
	return true;
}

bool CoreActionController::saveSong() {
	
	auto pHydrogen = Hydrogen::get_instance();

	// Get the current Song which is about to be saved.
	auto pSong = pHydrogen->getSong();
	
	// Extract the path to the associate .h2song file.
	QString songPath = pSong->get_filename();
	
	if ( songPath.isEmpty() ) {
		ERRORLOG( "Unable to save song. Empty filename!" );
		return false;
	}
	
	// Actual saving
	bool saved = pSong->save( songPath );
	if ( !saved ) {
		ERRORLOG( QString( "Current song [%1] could not be saved!" )
				  .arg( songPath ) );
		return false;
	}
	
	// Update the status bar.
	if ( pHydrogen->getActiveGUI() ) {
		EventQueue::get_instance()->push_event( EVENT_UPDATE_SONG, 1 );
	}
	
	return true;
}

bool CoreActionController::saveSongAs( const QString& songPath ) {
	
	auto pHydrogen = Hydrogen::get_instance();
	
	// Get the current Song which is about to be saved.
	auto pSong = pHydrogen->getSong();
	
	// Check whether the provided path is valid.
	if ( !isSongPathValid( songPath ) ) {
		// isSongPathValid takes care of the error log message.
		return false;
	}
	
	if ( songPath.isEmpty() ) {
		ERRORLOG( "Unable to save song. Empty filename!" );
		return false;
	}
	
	// Actual saving
	bool saved = pSong->save( songPath );
	if ( !saved ) {
		ERRORLOG( QString( "Current song [%1] could not be saved!" )
				  .arg( songPath ) );
		return false;
	}
	
	// Update the status bar.
	if ( pHydrogen->getActiveGUI() ) {
		EventQueue::get_instance()->push_event( EVENT_UPDATE_SONG, 1 );
	}
	
	return true;
}

bool CoreActionController::quit() {
	
	auto pHydrogen = Hydrogen::get_instance();
	
	EventQueue::get_instance()->push_event( EVENT_QUIT, 0 );
	
	return true;
}


bool CoreActionController::isSongPathValid( const QString& songPath ) {
	
	QFileInfo songFileInfo = QFileInfo( songPath );

	if ( !songFileInfo.isAbsolute() ) {
		ERRORLOG( QString( "Error: Unable to handle path [%1]. Please provide an absolute file path!" )
						.arg( songPath.toLocal8Bit().data() ));
		return false;
	}
	
	if ( songFileInfo.exists() ) {
		if ( !songFileInfo.isWritable() ) {
			ERRORLOG( QString( "Error: Unable to handle path [%1]. You must have permissions to write the file!" )
						.arg( songPath.toLocal8Bit().data() ));
			return false;
		}
	}
	
	if ( songFileInfo.suffix() != "h2song" ) {
		ERRORLOG( QString( "Error: Unable to handle path [%1]. The provided file must have the suffix '.h2song'!" )
					.arg( songPath.toLocal8Bit().data() ));
		return false;
	}
	
	return true;
}
	
	
}
