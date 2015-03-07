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
    Author:     Reve
*/

#include "eve-server.h"
#include "ClientDB.h"
#include "EVEServerConfig.h"

ClientDB::ClientDB(DBcore &db):
	m_db(db)
{

}

bool ClientDB::GetAccountInformation( const std::string username, const std::string password, AccountInfo & account_info )
{
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
		if(sConfig.account.autoAccountRole > 0) {
			uint32 accountID = CreateNewAccount(username, password, sConfig.account.autoAccountRole);
			if( accountID > 0 ) {
				// add new account successful, get account info again
				bool ret = GetAccountInformation(username, password, account_info);
				return ret;
			}
			else
				return false;
		}
		else
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

	return true;
}

bool ClientDB::UpdateAccountHash( const std::string username, std::string & hash )
{
	DBerror err;
	std::string escaped_hash;
	std::string escaped_username;

	m_db.DoEscapeString(escaped_hash, hash);
	m_db.DoEscapeString(escaped_username, username);

	if(!m_db.RunQuery(err, "UPDATE account SET password='',hash='%s' where accountName='%s'", escaped_hash.c_str(), escaped_username.c_str())) {

		sLog.Error( "AccountDB", "Unable to update account information for: %s.", username );
		return false;
	}

	return true;
}

bool ClientDB::UpdateAccountInformation( const std::string username, bool isOnline )
{
	DBerror err;
	std::string escaped_username;

	m_db.DoEscapeString(escaped_username, username);
	if(!m_db.RunQuery(err, "UPDATE account SET lastLogin=now(), logonCount=logonCount+1, online=%u where accountName='%s'", isOnline, escaped_username.c_str())) {
		sLog.Error( "AccountDB", "Unable to update account information for: %s.", username );
		return false;
	}

	return true;
}

uint32 ClientDB::CreateNewAccount( const std::string username, const std::string password, uint64 role )
{
	uint32 accountID;

	DBerror err;
	if( !m_db.RunQueryLID( err, accountID,
		"INSERT INTO account ( accountName, hash, role )"
		" VALUES ( '%s', '%s', %" PRIu64 " )",
		username.c_str(), password.c_str(), role ) )
	{
		sLog.Error( "ServiceDB", "Failed to create a new account '%s': %s.", username, err.c_str() );
		return 0;
	}

	return accountID;
}