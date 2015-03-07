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
    Author:        Aknor Jaden, Reve
*/

#include "eve-server.h"
#include "EVEServerConfig.h"

#include "apiserver/APIServiceDB.h"

APIServiceDB::APIServiceDB()
{
	DBcore m_db = DBcore();
}

APIServiceDB::~APIServiceDB()
{
	m_db.Close();
}

bool APIServiceDB::GetAccountInformation( const std::string username, const std::string password, AccountInfo & account_info )
{
	_openDBCon();
	std::string _escaped_username;

	m_db.DoEscapeString(_escaped_username, username);

	DBQueryResult res;
	if( !m_db.RunQuery( res, "SELECT accountID, password, hash, role, online, banned, logonCount, lastLogin FROM account WHERE accountName = '%s'", _escaped_username.c_str() ) )
	{
		sLog.Error( "ServiceDB", "Error in query: %s.", res.error.c_str() );
		return false;
	}

	DBResultRow row;
	if (!res.GetRow( row )) {
		// account not found, create new one if autoAccountRole is not zero (0)
		return false;
	}

	/* when any of the text gets are NULL it will fail... I think.. */
	account_info.id         = row.GetUInt(0);

	if (!row.IsNull(1))
		account_info.password = row.GetText(1);

	if (!row.IsNull(2))
		account_info.hash   = row.GetText(2);

	account_info.name       = _escaped_username;
	account_info.role       = row.GetUInt64(3);
	account_info.online     = row.GetBool(4);
	account_info.banned     = row.GetBool(5);
	account_info.visits     = row.GetUInt(6);

	if (!row.IsNull(7))
		account_info.last_login = row.GetText(7);

	_closeDBCon();
	return true;
}

bool APIServiceDB::GetAccountIdFromUsername(std::string username, std::string * accountID)
{
	_openDBCon();
    DBQueryResult res;

    // Find accountID in 'account' table using accountName:
    if( !m_db.RunQuery(res,
        "SELECT"
        "    accountID "
        " FROM account "
        " WHERE accountName='%s'" , username.c_str() ))
    {
        sLog.Error( "APIServiceDB::GetAccountIdFromUsername()", "Cannot find accountID for username %s", username.c_str() );
        return false;
    }

    DBResultRow row;
    if( !res.GetRow(row) )
    {
        sLog.Error( "APIServiceDB::GetAccountIdFromUsername()", "res.GetRow(row) failed for unknown reason." );
        return false;
    }

    *accountID = row.GetText(0);        // Grab accountID from the retrieved row from the 'account' table
    
	_closeDBCon();
	return true;
}

bool APIServiceDB::GetAccountIdFromUserID(std::string userID, uint32 * accountID)
{
	_openDBCon();
    DBQueryResult res;

    // Find accountID in 'accountapi' table using userID:
    if( !m_db.RunQuery(res,
        "SELECT"
        "    accountID "
        " FROM accountApi "
        " WHERE userID='%s'" , userID.c_str() ))
    {
        sLog.Error( "APIServiceDB::GetAccountIdFromUserID()", "Cannot find accountID for userID %s", userID.c_str() );
        return false;
    }

    DBResultRow row;
    if( !res.GetRow(row) )
    {
        sLog.Error( "APIServiceDB::GetAccountIdFromUserID()", "res.GetRow(row) failed for unknown reason." );
        return false;
    }

    *accountID = row.GetUInt(0);        // Grab accountID from the retrieved row from the 'accountapi' table
	
	_closeDBCon();
	return true;
}

bool APIServiceDB::GetApiAccountInfoUsingAccountID(std::string accountID, uint32 * userID, std::string * apiFullKey,
    std::string * apiLimitedKey, uint32 * apiRole)
{
    _openDBCon();
	DBQueryResult res;

    // Find userID, fullKey, limitedKey, and apiRole from 'accountApi' table using accountID obtained from 'account' table:
    if( !m_db.RunQuery(res,
        "SELECT"
        "    userID, fullKey, limitedKey, apiRole "
        " FROM accountApi "
        " WHERE accountID='%s'" , accountID.c_str() ))
    {
        sLog.Error( "APIServiceDB::GetApiAccountInfoUsingAccountID()", "Cannot find accountID '%s' in 'accountApi' table", accountID.c_str() );
        return false;
    }

    DBResultRow row;
    if( !res.GetRow(row) )
    {
        sLog.Error( "APIServiceDB::GetApiAccountInfoUsingAccountID()", "res.GetRow(row) failed for unknown reason." );
        return false;
    }

    *userID = row.GetUInt(0);            // Grab userID from retrieved row from the 'accountApi' table
    *apiFullKey = row.GetText(1);        // Grab Full API Key from retrieved row from the 'accountApi' table
    *apiLimitedKey = row.GetText(2);    // Grab Limited API Key from retrieved row from the 'accountApi' table
    *apiRole = row.GetUInt(3);            // Grab API Role from retrieved row from the 'accountApi' table
    
	_closeDBCon();
	return true;
}

bool APIServiceDB::GetApiAccountInfoUsingUserID(std::string userID, std::string * apiFullKey, std::string * apiLimitedKey, uint32 * apiRole)
{
	_openDBCon();
    DBQueryResult res;

    // Find fullKey, limitedKey, and apiRole from 'accountApi' table using userID supplied from an API query string:
    if( !m_db.RunQuery(res,
        "SELECT"
        "    fullKey, limitedKey, apiRole "
        " FROM accountApi "
        " WHERE userID='%s'" , userID.c_str() ))
    {
        sLog.Error( "APIServiceDB::GetApiAccountInfoUsingUserID()", "Cannot find userID '%s' in 'accountApi' table", userID.c_str() );
        return false;
    }

    DBResultRow row;
    if( !res.GetRow(row) )
    {
        sLog.Error( "APIServiceDB::GetApiAccountInfoUsingUserID()", "res.GetRow(row) failed for unknown reason." );
        return false;
    }

    *apiFullKey = row.GetText(0);        // Grab Full API Key from retrieved row from the 'accountApi' table
    *apiLimitedKey = row.GetText(1);    // Grab Limited API Key from retrieved row from the 'accountApi' table
    *apiRole = row.GetUInt(2);            // Grab API Role from retrieved row from the 'accountApi' table
   
	_closeDBCon();
	return true;
}

bool APIServiceDB::UpdateUserIdApiKeyDatabaseRow(uint32 userID, std::string apiFullKey, std::string apiLimitedKey)
{
    // Check key lengths and report error and return if either are incorrect:
    if( apiLimitedKey.length() != 64 )
    {
        sLog.Error( "APIServiceDB::UpdateUserIdApiKeyDatabaseRow()", "limitedApiKey length != 64, but rather %u", apiLimitedKey.length() );
        return false;
    }

    if( apiFullKey.length() != 64 )
    {
        sLog.Error( "APIServiceDB::UpdateUserIdApiKeyDatabaseRow()", "fullApiKey length != 64, but rather %u", apiFullKey.length() );
        return false;
    }

	_openDBCon();

    // Update fullKey and limitedKey in the 'accountApi' table using userID:
    DBerror err;

    if( !m_db.RunQuery(err,
        "UPDATE"
        " accountApi"
        " SET fullKey = '%s', limitedKey = '%s'"
        " WHERE userID = %u",
        apiFullKey.c_str(), apiLimitedKey.c_str(), userID ))
    {
        sLog.Error( "", "Error in query: %s.", err.c_str());
		_closeDBCon();
        return false;
    }
    else
	{
		_closeDBCon();
        return true;
	}
}

bool APIServiceDB::InsertNewUserIdApiKeyInfoToDatabase(uint32 accountID, std::string apiFullKey, std::string apiLimitedKey, uint32 apiRole)
{
    // Check key lengths and report error and return if either are incorrect:
    if( apiLimitedKey.length() != 64 )
    {
        sLog.Error( "APIServiceDB::UpdateUserIdApiKeyDatabaseRow()", "limitedApiKey length != 64, but rather %u", apiLimitedKey.length() );
        return false;
    }

    if( apiFullKey.length() != 64 )
    {
        sLog.Error( "APIServiceDB::UpdateUserIdApiKeyDatabaseRow()", "fullApiKey length != 64, but rather %u", apiFullKey.length() );
        return false;
    }

	_openDBCon();
    DBerror err;

    if( !m_db.RunQuery(err,
        "INSERT INTO"
        " accountApi ("
        "    accountID, fullKey, limitedKey, apiRole"
        " ) VALUES ("
        "    %u, '%s', '%s', %u"
        " )",
            accountID, apiFullKey.c_str(), apiLimitedKey.c_str(), apiRole
            ))
    {
        sLog.Error( "", "Error in query: %s.", err.c_str());
		_closeDBCon();
        return false;
    }
    else
	{
		_closeDBCon();
		return true;
	}
}

bool APIServiceDB::UpdateUserIdApiRole(uint32 userID, uint32 apiRole)
{
	_openDBCon();
    // Update fullKey and limitedKey in the 'accountApi' table using userID:
    DBerror err;

    if( !m_db.RunQuery(err,
        "UPDATE"
        " accountApi"
        " SET apiRole = %u"
        " WHERE userID = %u",
        apiRole, userID ))
    {
        sLog.Error( "", "Error in query: %s.", err.c_str());
		_closeDBCon();
        return false;
    }
    else
	{
		_closeDBCon();
        return true;
	}
}

void APIServiceDB::_openDBCon()
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