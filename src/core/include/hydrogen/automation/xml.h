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
#ifndef H2CORE_AUTOMATION_XML_H
#define H2CORE_AUTOMATION_XML_H


#include <QDomElement>

#include <hydrogen/automation/manager.h>

namespace H2Core {

class AutomationReader {
	public:
	AutomationReader( QDomElement node );
	virtual ~AutomationReader() {}
	std::unique_ptr<AutomationManager> read();

	protected:
	virtual std::unique_ptr<AutomationController> create_controller( QDomElement node );

	private:
	QDomElement m_Node;

	int read_attribute( QDomElement node, const QString &name );
};


class AutomationWriter {
	public:
	AutomationWriter( QDomNode &parent );
	void save( const AutomationManager& mgr );

	private:
	QDomNode m_parentNode;
	QDomDocument m_doc;
	QDomElement create_path_node( const AutomationController &ctrl);
};

}

#endif
