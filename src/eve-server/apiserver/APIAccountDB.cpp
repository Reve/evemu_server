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

#include "eve-server.h"
#include "EVEServerConfig.h"
#include "apiserver/APIAccountDB.h"

APIAccountDB::APIAccountDB()
{
	DBcore m_db = DBcore();
}

bool APIAccountDB::GetCharactersList(uint32 accountID, std::vector<std::string> & charIDList, std::vector<std::string> & charNameList,
    std::vector<std::string> & charCorpIDList, std::vector<std::string> & charCorpNameList)
{
	_openDBCon();
    DBQueryResult res;

    // Get list of characters and their corporation info from the accountID:
    if( !m_db.RunQuery(res,
        " SELECT "
        "   character_.characterID, "
        "   character_.corporationID, "
        "   corporation.corporationName, "
        "   entity.itemName AS name "
        " FROM `character_` "
        "   LEFT JOIN corporation ON corporation.corporationID = character_.corporationID "
        "   LEFT JOIN entity ON entity.itemID = character_.characterID "
        " WHERE `accountID` = %u ", accountID ))
    {
        sLog.Error( "APIAccountDB::GetCharactersList()", "Cannot find accountID %u", accountID );
		_closeDBCon();
        return false;
    }

    DBResultRow row;
    std::map<std::string, std::string> charInfo;
    while( res.GetRow( row ) )
    {
        charIDList.push_back( std::string(row.GetText(0)) );
        charCorpIDList.push_back( std::string(row.GetText(1)) );
        charCorpNameList.push_back( std::string(row.GetText(2)) );
        charNameList.push_back( std::string(row.GetText(3)) );
    }

	_closeDBCon();
    return true;
}


bool APIAccountDB::GetAccountInfo(uint32 accountID, std::vector<std::string> & accountInfoList)
{
	_openDBCon();
    DBQueryResult res;

    // Get account table info using the accountID:
    if( !m_db.RunQuery(res,
        " SELECT "
        "   online, "
        "   banned, "
        "   logonCount, "
        "   lastLogin "
        " FROM account "
        " WHERE `accountID` = %u ", accountID ))
    {
        sLog.Error( "APIAccountDB::GetAccountInfo()", "Cannot find accountID %u", accountID );
        return false;
    }

    DBResultRow row;
    if( !res.GetRow(row) )
    {
        sLog.Error( "APIServiceDB::GetAccountInfo()", "res.GetRow(row) failed for unknown reason." );
		_closeDBCon();
        return false;
    }

    accountInfoList.push_back( std::string(row.GetText(0)) );
    accountInfoList.push_back( std::string(row.GetText(1)) );
    accountInfoList.push_back( std::string(row.GetText(2)) );
    accountInfoList.push_back( std::string(row.GetText(3)) );

	_closeDBCon();
    return true;
}

void APIAccountDB::_openDBCon()
{
	//create a connection to the database
	DBerror err;
	if( !m_db.Open( err,
		sConfig.database.host.c_str(),
		sConfig.database.username.c_str(),
		sConfig.database.password.c_str(),
		sConfig.database.db.c_str(),
		sConfig.database.port ) )
	{
		sLog.Error( "client init", "Unable to connect to the database: %s", err.c_str() );
	}
}
