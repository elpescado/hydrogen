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
#ifndef EXPORTER_H
#define EXPORTER_H

#include <QString>


namespace H2Core {
class Song;
class Instrument;

class Exporter {
	public:
	static bool instrument_has_notes(Song *song, int instrument_id);
	static QString find_unique_export_filename_for_instrument(Song *, Instrument *);
	static QString export_filename(const QString &base, Song *pSong, Instrument *pInstrument);
	static QString remove_reserved_characters(QString fileName);
};

}

#endif
