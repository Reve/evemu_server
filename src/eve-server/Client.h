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

#ifndef EVE_CLIENT_H
#define EVE_CLIENT_H

#include "NetService.h"

// account services
#include "account/AccountService.h"
#include "account/AuthService.h"
#include "account/BrowserLockdownSvc.h"
#include "account/ClientStatMgrService.h"
#include "account/InfoGatheringMgr.h"
#include "account/TutorialService.h"
#include "account/UserService.h"
// admin services
#include "admin/AlertService.h"
#include "admin/AllCommands.h"
#include "admin/ClientStatLogger.h"
#include "admin/CommandDispatcher.h"
#include "admin/DevToolsProviderService.h"
#include "admin/PetitionerService.h"
#include "admin/SlashService.h"
// cache services
#include "cache/BulkMgrService.h"
#include "cache/ObjCacheService.h"
// character services
#include "character/AggressionMgrService.h"
#include "character/CertificateMgrService.h"
#include "character/CharFittingMgrService.h"
#include "character/CharMgrService.h"
#include "character/CharUnboundMgrService.h"
#include "character/PaperDollService.h"
#include "character/PhotoUploadService.h"
#include "character/SkillMgrService.h"
// chat services
#include "chat/LookupService.h"
#include "chat/LSCService.h"
#include "chat/OnlineStatusService.h"
#include "chat/VoiceMgrService.h"
// config services
#include "config/ConfigService.h"
#include "config/LanguageService.h"
#include "config/LocalizationServerService.h"
// corporation services
#include "corporation/CorpBookmarkMgrService.h"
#include "corporation/CorpMgrService.h"
#include "corporation/CorporationService.h"
#include "corporation/CorpRegistryService.h"
#include "corporation/CorpStationMgrService.h"
#include "corporation/LPService.h"
// dogmaim services
#include "dogmaim/DogmaIMService.h"
#include "dogmaim/DogmaService.h"
// inventory services
#include "inventory/InvBrokerService.h"
// mail services
#include "mail/MailMgrService.h"
#include "mail/MailingListMgrService.h"
#include "mail/NotificationMgrService.h"
// manufacturing services
#include "manufacturing/FactoryService.h"
#include "manufacturing/RamProxyService.h"
// map services
#include "map/MapService.h"
// market services
#include "market/BillMgrService.h"
#include "market/ContractMgrService.h"
#include "market/ContractProxy.h"
#include "market/MarketProxyService.h"
// mining services
#include "mining/ReprocessingService.h"
// missions services
#include "missions/AgentMgrService.h"
#include "missions/DungeonExplorationMgrService.h"
#include "missions/MissionMgrService.h"
// pos services
#include "pos/PlanetMgr.h"
#include "pos/PosMgrService.h"
//Search Service
#include "search/SearchMgrService.h"
// ship services
#include "ship/BeyonceService.h"
#include "ship/FleetProxy.h"
#include "ship/InsuranceService.h"
#include "ship/RepairService.h"
#include "ship/ShipService.h"
#include "ship/modules/ModuleEffects.h"
// standing services
#include "standing/FactionWarMgrService.h"
#include "standing/SovereigntyMgrService.h"
#include "standing/Standing2Service.h"
#include "standing/WarRegistryService.h"
// station services
#include "station/HoloscreenMgrService.h"
#include "station/JumpCloneService.h"
#include "station/StationService.h"
#include "station/StationSvcService.h"
// system services
#include "system/BookmarkService.h"
#include "system/DungeonService.h"
#include "system/KeeperService.h"
#include "system/ScenarioService.h"
#include "system/WrecksAndLoot.h"

#include "ClientSession.h"
#include "LiveUpdateDB.h"
#include "ClientDB.h"
#include "ClientList.h"

class PySubStream;
class PyCallStream;
class PyTuple;
class PyAddress;
class PyList;
class PyDict;
class PyPacket;
class PyRep;

class Client: protected EVEClientSession, protected EVEPacketDispatcher
{
public:
	long id;
	ClientList m_clientList;
	DBcore m_db;
	LiveUpdateDB m_liveUpdate;

	Client(EVETCPConnection **con, ItemFactory &item_factory);
	virtual ~Client(void);
	bool operator==(const Client& other) const;

	void Disconnect();
	virtual void Process();
	bool ProcessNet();

	std::string GetLanguageID() const               { return mSession.GetCurrentString( "languageID" ); }

	uint32 GetAccountID() const                     { return mSession.GetCurrentInt( "userid" ); }
	uint64 GetAccountRole() const                   { return mSession.GetCurrentLong( "role" ); }
	
	std::string GetName() const						{ return "Dummy"; } //this is to suprimate the errors but it should be the chars name

	PyServiceMgr services() { return m_services; }

	// Packet stuff
	void _SendCallReturn( const PyAddress& source, uint64 callID, PyRep** return_value, const char* channel = NULL );
	void _SendException( const PyAddress& source, uint64 callID, MACHONETMSG_TYPE in_response_to, MACHONETERR_TYPE exception_type, PyRep** payload );
	void _SendSessionChange();
	void _SendPingRequest();
	void _SendPingResponse( const PyAddress& source, uint64 callID );

	//void _LoadSystems(EntityList &entityList, ItemFactory &itemFactory);

protected:
	static long nextID;
	ClientSession mSession;

	/********************************************************************/
	/* EVEClientSession interface                                       */
	/********************************************************************/
	void _GetVersion( VersionExchangeServer& version );
	uint32 _GetUserCount();
	uint32 _GetQueuePosition() { /* hack */ return 1; }

	/********************************************************************/
	/* EVEClientLogin state machine                                      */
	/********************************************************************/
	bool _VerifyVersion( VersionExchangeClient& version );
	bool _VerifyCrypto( CryptoRequestPacket& cr );
	bool _VerifyLogin( CryptoChallengePacket& ccp );
	bool _VerifyVIPKey( const std::string& vipKey ) { /* do nothing */ return true; }
	bool _VerifyFuncResult( CryptoHandshakeResult& result );

	/********************************************************************/
	/* EVEPacketDispatcher interface                                    */
	/********************************************************************/
	bool Handle_CallReq( PyPacket* packet, PyCallStream& req );
	bool Handle_Notify( PyPacket* packet );
	bool Handle_PingReq( PyPacket* packet ) { _SendPingResponse( packet->dest, packet->source.callID ); return true; }
	bool Handle_PingRsp( PyPacket* packet ) { /* do nothing */ return true; }

private:
	ClientDB m_clientDB;
	PyServiceMgr m_services;
	ItemFactory iFactory;
	Player *player;
	Timer m_pingTimer;

	void _LoadSystems();
};

#endif // !EVE_CLIENT_H