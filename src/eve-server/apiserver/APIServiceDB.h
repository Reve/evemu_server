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

#ifndef __APISERVICEDB_H_INCL__
#define __APISERVICEDB_H_INCL__

struct AccountInfo
{
	std::string name;
	std::string password;
	std::string hash;
	uint32 id;
	uint64 role;
	uint32 visits;
	std::string last_login;
	bool online;
	bool banned;
};

class APIServiceDB
{
public:
	// we create own instance of db so we don't share the connection with other services
	// we will also open and close the connection for each operation to limit the connections
	DBcore m_db;

    APIServiceDB();
	~APIServiceDB();

	bool GetAccountInformation( const std::string username, const std::string password, AccountInfo & account_info );
    bool GetAccountIdFromUsername(std::string username, std::string * accountID);
    bool GetAccountIdFromUserID(std::string userID, uint32 * accountID);
    bool GetApiAccountInfoUsingAccountID(std::string accountID, uint32 * userID, std::string * apiFullKey,
        std::string * apiLimitedKey, uint32 * apiRole);
    bool GetApiAccountInfoUsingUserID(std::string userID, std::string * apiFullKey, std::string * apiLimitedKey, uint32 * apiRole);
    bool UpdateUserIdApiKeyDatabaseRow(uint32 userID, std::string apiFullKey, std::string apiLimitedKey);
    bool InsertNewUserIdApiKeyInfoToDatabase(uint32 accountID, std::string apiFullKey, std::string apiLimitedKey, uint32 apiRole);
    bool UpdateUserIdApiRole(uint32 userID, uint32 apiRole);

private:
	void _openDBCon();
	void _closeDBCon() { m_db.Close(); }
};

#endif    //__APISERVICEDB_H_INCL__
