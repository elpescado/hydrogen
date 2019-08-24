#include <hydrogen/automation/factory.h>
#include <hydrogen/automation/controller_library.h>

#include <hydrogen/hydrogen.h>
#include <hydrogen/basics/song.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>

namespace H2Core {


std::vector<Constructor> AutomationFactory::enumerate()
{
	update();
	return m_ctors;
}

void AutomationFactory::update()
{

//	m_ctors.emplace_back("Master volume", [](){ return std::unique_ptr<AutomationController>(new MasterVolumeController()); });
	register_ctrl<MasterVolumeController>();

	Hydrogen *pEngine = Hydrogen::get_instance();
	Song *song = pEngine->getSong();
	InstrumentList *instrList = song->get_instrument_list();
	int nSize = instrList->size();
	for (int i = 0; i < nSize; ++i) {
		register_ctrl<InstrumentVolumeController>(i);
		register_ctrl<InstrumentPanController>(i);
//		ctors.emplace_back("Instr volume", [=](){ return new InstrumentVolumeController(i); });
//		ctors.emplace_back("Instr pan", [=](){ return new InstrumentPanController(i); });
	}
}


}

