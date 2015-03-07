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

#ifndef EVE_CLIENT_DB_H
#define EVE_CLIENT_DB_H

#include "eve-server.h"

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

class ClientDB 
{
public:
	DBcore m_db;

	ClientDB(DBcore &db);
	bool GetAccountInformation(const std::string username, const std::string password, AccountInfo &accountInfo);
	bool UpdateAccountHash( const std::string username, std::string & hash );
	bool UpdateAccountInformation( const std::string username, bool isOnline );

	/**
     * CreateNewAccount
     *
     * This method is part of the "autoAccount" creation patch by firefoxpdm. This
     * will insert a new account row into the database if the account name doesn't
     * exist at login.
     *
     * @param login is a const char string containing the name.
     * @param pass is a const char string containing the password.
     * @param role is the users role in the game.
     * @author firefoxpdm, xanarox
     */
	uint32 CreateNewAccount( const std::string username, const std::string password, uint64 role );
};

#endif