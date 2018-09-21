/*
 * Hydrogen
 * Copyright(c) 2002-2018 by the Hydrogen team
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
#include <hydrogen/basics/pattern.h>
#include <hydrogen/exporter.h>
#include <hydrogen/basics/instrument.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/pattern_list.h>
#include <hydrogen/basics/song.h>

#include <QFileInfo>
#include <QString>

using namespace H2Core;

bool Exporter::instrument_has_notes(Song *song, int instrument_id)
{
	unsigned nPatterns = song->get_pattern_list()->size();
	
	bool bInstrumentHasNotes = false;
	
	for ( unsigned i = 0; i < nPatterns; i++ ) {
		Pattern *pPattern = song->get_pattern_list()->get( i );
		const Pattern::notes_t* notes = pPattern->get_notes();
		FOREACH_NOTE_CST_IT_BEGIN_END(notes,it) {
			Note *pNote = it->second;
			assert( pNote );

			if( pNote->get_instrument()->get_id() == song->get_instrument_list()->get(instrument_id)->get_id() ){
				bInstrumentHasNotes = true;
				break;
			}
		}
	}
	
	return bInstrumentHasNotes;
}


QString Exporter::find_unique_export_filename_for_instrument(Song *pSong, Instrument *pInstrument)
{
	QString uniqueInstrumentName;
	
	int instrumentOccurence = 0;
	for(int i=0; i  < pSong->get_instrument_list()->size(); i++ ){
		if( pSong->get_instrument_list()->get(i)->get_name() == pInstrument->get_name()){
			instrumentOccurence++;
		}
	}
	
	if(instrumentOccurence >= 2){
		uniqueInstrumentName = pInstrument->get_name() + QString("_") + QString::number( pInstrument->get_id() );
	} else {
		uniqueInstrumentName = pInstrument->get_name();
	}
	
	return uniqueInstrumentName;
}


QString Exporter::export_filename(const QString &base, Song *pSong, Instrument *pInstrument)
{
	QFileInfo fi( base );
	auto baseName = fi.completeBaseName();
	auto instrName = find_unique_export_filename_for_instrument( pSong, pInstrument );
	auto ext = fi.suffix();
	return QString( "%1-%2.%3" ).arg( baseName ).arg( instrName ).arg( ext );
}


QString Exporter::remove_reserved_characters(QString fileName)
{
	return fileName.remove( QRegularExpression( "[\\\\/:*\"<>|]" ) );
}
