/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2011 The EVEmu Team
    For the latest information visit http://evemu.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:        Aknor Jaden
*/

#ifndef __APIACCOUNTDB_H_INCL__
#define __APIACCOUNTDB_H_INCL__



class APIAccountDB
{
public:
    APIAccountDB();

    bool GetCharactersList(uint32 accountID, std::vector<std::string> & charIDList, std::vector<std::string> & charNameList,
    std::vector<std::string> & charCorpIDList, std::vector<std::string> & charCorpNameList);
    bool GetAccountInfo(uint32 accountID, std::vector<std::string> & accountInfoList);

private:
	DBcore m_db;

	void _openDBCon();
	void _closeDBCon(){ m_db.Close(); }
};

#endif    //__APIACCOUNTDB_H_INCL__


