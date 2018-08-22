#include <cppunit/extensions/HelperMacros.h>
#include <hydrogen/exporter.h>

#include <hydrogen/basics/instrument.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/song.h>

#include <memory>

#include "test_helper.h"

using namespace H2Core;


/**
 * \brief Test Case for Exporter class
 */
class ExporterTestCase : public CppUnit::TestCase {
	CPPUNIT_TEST_SUITE( ExporterTestCase );
	CPPUNIT_TEST( test_remove_metacharacters );
	CPPUNIT_TEST( test_remove_many_metacharacters );
	CPPUNIT_TEST( test_remove_all_metacharacters );
	CPPUNIT_TEST_SUITE_END();

	public:

	void test_remove_metacharacters()
	{
		auto filtered = Exporter::remove_reserved_characters( "Tom 12\"" );
		CPPUNIT_ASSERT_EQUAL( QString( "Tom 12" ), filtered );
	}

	void test_remove_many_metacharacters()
	{
		auto filtered = Exporter::remove_reserved_characters( "<hihat/open>" );
		CPPUNIT_ASSERT_EQUAL( QString( "hihatopen" ), filtered );
	}

	void test_remove_all_metacharacters()
	{
		auto filtered = Exporter::remove_reserved_characters( "*Test<>\"|\\:/" );
		CPPUNIT_ASSERT_EQUAL( QString( "Test" ), filtered );
	}
};


/**
 * \brief Test Case for Exporter::instrument_has_notes function
 */
class ExporterNoteFinderTestCase : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( ExporterNoteFinderTestCase );
	CPPUNIT_TEST( test_notes_in_song );
	CPPUNIT_TEST( test_notes_in_unused_pattern );
	CPPUNIT_TEST( test_notes_not_present_in_song );
	CPPUNIT_TEST_SUITE_END();

	std::unique_ptr<Song>song;

	public:
	void setUp() override
	{
		song.reset( Song::load( H2TEST_FILE( "exporter.h2song" ) ) );
	}

	void test_notes_in_song()
	{
		/* Both Kick and Snare are used in pattern that is used in song */
		auto snare = get_instrument_id( "Snare" );
		CPPUNIT_ASSERT( Exporter::instrument_has_notes( song.get(), snare ));

		auto kick = get_instrument_id( "Kick" );
		CPPUNIT_ASSERT( Exporter::instrument_has_notes( song.get(), snare ));
	}

	void test_notes_in_unused_pattern()
	{
		/* Ride is present on pattern that is not used in song */
		auto ride = get_instrument_id( "Ride" );
		CPPUNIT_ASSERT( Exporter::instrument_has_notes( song.get(), ride ));
	}

	void test_notes_not_present_in_song()
	{
		/* Crash has no notes anywhere */
		auto crash = get_instrument_id( "Crash" );
		CPPUNIT_ASSERT( !Exporter::instrument_has_notes( song.get(), crash ));
	}


	private:
	// utilities

	int get_instrument_id( const QString &name )
	{
		return song->get_instrument_list()->find( name )->get_id();
	}
};


/**
 * \brief Test Case for Exporter::find_unique_export_filename_for_instrument function
 */
class ExporterFindUniqueInstrumentNameTestCase : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( ExporterFindUniqueInstrumentNameTestCase );
	CPPUNIT_TEST( test_single_instrument );
	CPPUNIT_TEST( test_duplicated_instrument );
	CPPUNIT_TEST_SUITE_END();

	std::unique_ptr<Song>song;
	Instrument *kick;
	Instrument *snare;
	Instrument *snare_rimshot;

	public:
	void setUp() override
	{
		song.reset( Song::load( H2TEST_FILE( "exporter.h2song" ) ) );
		kick = get_instrument( "Kick" );
		snare = get_instrument( "Snare" );
		snare_rimshot = get_instrument( "Snare Rimshot" );
		snare_rimshot->set_name( "Snare" );
	}

	void test_single_instrument()
	{
		CPPUNIT_ASSERT_EQUAL(
			QString( "Kick" ),
			Exporter::find_unique_export_filename_for_instrument( song.get(), kick )
		);
	}

	void test_duplicated_instrument()
	{
		CPPUNIT_ASSERT_EQUAL(
			QString( "Snare_2" ),
			Exporter::find_unique_export_filename_for_instrument( song.get(), snare )
		);
		CPPUNIT_ASSERT_EQUAL(
			QString( "Snare_4" ),
			Exporter::find_unique_export_filename_for_instrument( song.get(), snare_rimshot )
		);
	}

	private:
	// utilities

	Instrument * get_instrument( const QString &name )
	{
		auto instr = song->get_instrument_list()->find( name );
		CPPUNIT_ASSERT( instr );
		return instr;
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( ExporterTestCase );
CPPUNIT_TEST_SUITE_REGISTRATION( ExporterNoteFinderTestCase );
CPPUNIT_TEST_SUITE_REGISTRATION( ExporterFindUniqueInstrumentNameTestCase );
