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

#include "Client.h"
#include "ClientList.h"
#include "PyServiceMgr.h"
#include "EVEServerConfig.h"
#include "imageserver/ImageServer.h"

long Client::nextID = 0;

Client::Client(EVETCPConnection **con):
	// EVEConnection should be in here
	EVEClientSession(con),
	m_clientList(ClientList::get()),
	m_db(DBcore()),
	m_liveUpdate(m_db),
	m_clientDB(m_db),
	services(NULL)
{
	//get unique id for this class
	id = ++nextID;

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

Client::~Client(void)
{
}

bool Client::operator==(const Client &other) const
{
	if(other.id == id)
		return true;
	else
		return false;
}

void Client::Disconnect()
{
	//sClientList.Remove(const_cast<Client&>(*this));
}

void Client::Process()
{
	ProcessNet();
}

bool Client::ProcessNet()
{
	// Simulate disconnect
	if(true)
		return false;

	return true;
}

/************************************************************************/
/* EVEClientSession interface                                           */
/************************************************************************/
void Client::_GetVersion( VersionExchangeServer& version )
{
    version.birthday = EVEBirthday;
    version.macho_version = MachoNetVersion;
    version.user_count = _GetUserCount();
    version.version_number = EVEVersionNumber;
    version.build_version = EVEBuildVersion;
    version.project_version = EVEProjectVersion;
}

uint32 Client::_GetUserCount()
{
	ClientList c_list = ClientList::get();
    return c_list.getCount();
}

bool Client::_VerifyVersion( VersionExchangeClient& version )
{
    sLog.Log("Client","%s: Received Low Level Version Exchange:", GetAddress().c_str());
    version.Dump(NET__PRES_REP, "    ");

    if( version.birthday != EVEBirthday )
        sLog.Error("Client","%s: Client's birthday does not match ours!", GetAddress().c_str());

    if( version.macho_version != MachoNetVersion )
        sLog.Error("Client","%s: Client's macho_version not match ours!", GetAddress().c_str());

    if( version.version_number != EVEVersionNumber )
        sLog.Error("Client","%s: Client's version_number not match ours!", GetAddress().c_str());

    if( version.build_version != EVEBuildVersion )
        sLog.Error("Client","%s: Client's build_version not match ours!", GetAddress().c_str());

    if( version.project_version != EVEProjectVersion )
        sLog.Error("Client","%s: Client's project_version not match ours!", GetAddress().c_str());


    return true;
}

bool Client::_VerifyCrypto( CryptoRequestPacket& cr )
{
    if( cr.keyVersion != "placebo" )
    {
        //I'm sure cr.keyVersion can specify either CryptoAPI or PyCrypto, but its all binary so im not sure how.
        CryptoAPIRequestParams car;
        if( !car.Decode( cr.keyParams ) )
        {
            sLog.Error("Client","%s: Received invalid CryptoAPI request!", GetAddress().c_str());
        }
        else
        {
            sLog.Error("Client","%s: Unhandled CryptoAPI request: hashmethod=%s sessionkeylength=%d provider=%s sessionkeymethod=%s", GetAddress().c_str(), car.hashmethod.c_str(), car.sessionkeylength, car.provider.c_str(), car.sessionkeymethod.c_str());
            sLog.Error("Client","%s: You must change your client to use Placebo crypto in common.ini to talk to this server!\n", GetAddress().c_str());
        } 

        return false;
    }
    else
    {
        sLog.Debug("Client","%s: Received Placebo crypto request, accepting.", GetAddress().c_str());

        //send out accept response
        PyRep* rsp = new PyString( "OK CC" );
        mNet->QueueRep( rsp );
        PyDecRef( rsp );

        return true;
    }
}

bool Client::_VerifyLogin( CryptoChallengePacket& ccp )
{
    std::string account_hash;
    std::string transport_closed_msg = "LoginAuthFailed";

    AccountInfo account_info;
    CryptoServerHandshake server_shake;

    /* send passwordVersion required: 1=plain, 2=hashed */
    PyRep* rsp = new PyInt( 2 );

    //sLog.Debug("Client","%s: Received Client Challenge.", GetAddress().c_str());
    //sLog.Debug("Client","Login with %s:", ccp.user_name.c_str());
    
    if (!m_clientDB.GetAccountInformation( 
				ccp.user_name.c_str(), 
				ccp.user_password_hash.c_str(), 
				account_info)) 
	{
        goto error_login_auth_failed;
    }

    /* check wether the account has been banned and if so send the semi correct message */
    if (account_info.banned) {
        transport_closed_msg = "ACCOUNTBANNED";
        goto error_login_auth_failed;
    }

    /* if we have stored a password we need to create a hash from the username and pass and remove the pass */
    if( account_info.password.empty() )
        account_hash = account_info.hash;
    else
    {
        /* here we generate the password hash ourselves */
        std::string password_hash;
        if( !PasswordModule::GeneratePassHash(
                ccp.user_name,
                account_info.password,
                password_hash ) )
        {
            sLog.Error("Client", "unable to generate password hash, sending LoginAuthFailed");
            goto error_login_auth_failed;
        }

        if( !m_clientDB.UpdateAccountHash(
                ccp.user_name.c_str(),
                password_hash ) )
        {
            sLog.Error("Client", "unable to update account hash, sending LoginAuthFailed");
            goto error_login_auth_failed;
        }

        account_hash = password_hash;
    }

    /* here we check if the user successfully entered his password or if he failed */
    if (account_hash != ccp.user_password_hash) {
        goto error_login_auth_failed;
    }

    /* Check if we already have a client online and if we do disconnect it
     * @note we should send GPSTransportClosed with reason "The user's connection has been usurped on the proxy"
     */
    if (account_info.online) {
        Client* client = m_clientList.findAccount(account_info.id);
        if (client != NULL)
            client->Disconnect();
    }

    mNet->QueueRep( rsp );
    PyDecRef( rsp );

    sLog.Log("Client","successful");

    /* update account information, increase login count, last login timestamp and mark account as online */
    m_clientDB.UpdateAccountInformation( account_info.name.c_str(), true );

    /* marshaled Python string "None" */
    static const uint8 handshakeFunc[] = { 0x74, 0x04, 0x00, 0x00, 0x00, 0x4E, 0x6F, 0x6E, 0x65 };

    /* send our handshake */

    server_shake.serverChallenge = "";
    server_shake.func_marshaled_code = new PyBuffer( handshakeFunc, handshakeFunc + sizeof( handshakeFunc ) );
    server_shake.verification = new PyBool( false );
    server_shake.cluster_usercount = _GetUserCount();
    server_shake.proxy_nodeid = 0xFFAA;
    server_shake.user_logonqueueposition = _GetQueuePosition();
    // binascii.crc_hqx of marshaled single-element tuple containing 64 zero-bytes string
    server_shake.challenge_responsehash = "55087";

    // the image server used by the client to download images
    server_shake.imageserverurl = sImageServer.url();

    server_shake.macho_version = MachoNetVersion;
    server_shake.boot_version = EVEVersionNumber;
    server_shake.boot_build = EVEBuildVersion;
    server_shake.boot_codename = EVEProjectCodename;
    server_shake.boot_region = EVEProjectRegion;

    rsp = server_shake.Encode();
    mNet->QueueRep( rsp );
    PyDecRef( rsp );

    // Setup session, but don't send the change yet.
    mSession.SetString( "address", EVEClientSession::GetAddress().c_str() );
    mSession.SetString( "languageID", ccp.user_languageid.c_str() );

    //user type 1 is normal user, type 23 is a trial account user.
    mSession.SetInt( "userType", 1 );
    mSession.SetInt( "userid", account_info.id );
    mSession.SetLong( "role", account_info.role );

    return true;

error_login_auth_failed:

    GPSTransportClosed* except = new GPSTransportClosed( transport_closed_msg );
    mNet->QueueRep( except );
    PyDecRef( except );

    return false;
}

bool Client::_VerifyFuncResult( CryptoHandshakeResult& result )
{
    _log(NET__PRES_DEBUG, "%s: Handshake result received.", GetAddress().c_str());

    //send this before session change
    CryptoHandshakeAck ack;
    ack.jit = GetLanguageID();
    ack.userid = GetAccountID();
    ack.maxSessionTime = new PyNone;
    ack.userType = 1;
    ack.role = GetAccountRole();
    ack.address = GetAddress();
    ack.inDetention = new PyNone;
    // no client update available
    ack.client_hash = new PyNone;
    ack.user_clientid = GetAccountID();
    ack.live_updates = m_liveUpdate.GetUpdates();

    PyRep* r = ack.Encode();
    mNet->QueueRep( r );
    PyDecRef( r );

	// Load services into memory
	_LoadSystems();

    // Send out the session change
    _SendSessionChange();

    return true;
}

/************************************************************************/
/* EVEPacketDispatcher interface                                        */
/************************************************************************/
bool Client::Handle_CallReq( PyPacket* packet, PyCallStream& req )
{
	PyCallable* dest;
	if( packet->dest.service.empty() )
	{
		//bound object
		uint32 nodeID, bindID;
		if( sscanf( req.remoteObjectStr.c_str(), "N=%u:%u", &nodeID, &bindID ) != 2 )
		{
			sLog.Error("Client","Failed to parse bind string '%s'.", req.remoteObjectStr.c_str());
			return false;
		}

		if( nodeID != services.GetNodeID() )
		{
			sLog.Error("Client","Unknown nodeID %u received (expected %u).", nodeID, services.GetNodeID());
			return false;
		}

		dest = services.FindBoundObject( bindID );
		if( dest == NULL )
		{
			sLog.Error("Client", "Failed to find bound object %u.", bindID);
			return false;
		}
	}
	else
	{
		//service
		dest = services.LookupService( packet->dest.service );
		if( dest == NULL )
		{
			sLog.Error("Client","Unable to find service to handle call to: %s", packet->dest.service.c_str());
			packet->dest.Dump(CLIENT__ERROR, "    ");

#pragma message( "TODO: throw proper exception to client (exceptions.ServiceNotFound)." )
			throw PyException( new PyNone );
		}
	}

	//Debug code
	if( req.method == "BeanCount" )
		sLog.Error("Client","BeanCount");
	else
		//this should be sLog.Debug, but because of the number of messages, I left it as .Log for readability, and ease of finding other debug messages
		sLog.Log("Server", "%s call made to %s",req.method.c_str(),packet->dest.service.c_str());

	//build arguments
	PyCallArgs args( this, req.arg_tuple, req.arg_dict );

	//parts of call may be consumed here
	PyResult result = dest->Call( req.method, args );

	_SendSessionChange();  //send out the session change before the return.
	_SendCallReturn( packet->dest, packet->source.callID, &result.ssResult );

	return true;
}

bool Client::Handle_Notify( PyPacket* packet )
{
	//turn this thing into a notify stream:
	ServerNotification notify;
	if( !notify.Decode( packet->payload ) )
	{
		sLog.Error("Client","Failed to convert rep into a notify stream");
		return false;
	}

	if(notify.method == "ClientHasReleasedTheseObjects")
	{
		ServerNotification_ReleaseObj element;

		PyList::const_iterator cur, end;
		cur = notify.elements->begin();
		end = notify.elements->end();
		for(; cur != end; cur++)
		{
			if(!element.Decode( *cur )) {
				sLog.Error("Client","Notification '%s' from %s: Failed to decode element. Skipping.", notify.method.c_str(), GetName());
				continue;
			}

			uint32 nodeID, bindID;
			if(sscanf(element.boundID.c_str(), "N=%u:%u", &nodeID, &bindID) != 2) {
				sLog.Error("Client","Notification '%s' from %s: Failed to parse bind string '%s'. Skipping.",
					notify.method.c_str(), GetName(), element.boundID.c_str());
				continue;
			}

			if(nodeID != services.GetNodeID()) {
				sLog.Error("Client","Notification '%s' from %s: Unknown nodeID %u received (expected %u). Skipping.",
					notify.method.c_str(), GetName(), nodeID, services.GetNodeID());
				continue;
			}

			services.ClearBoundObject(bindID);
		}
	}
	else
	{
		sLog.Error("Client","Unhandled notification from %s: unknown method '%s'", GetName(), notify.method.c_str());
		return false;
	}

	_SendSessionChange();  //just for good measure...
	return true;
}


void Client::_LoadSystems()
{
	// instantiate the service manager
	PyServiceMgr services( 888444);//, entityList, itemFactory );

	// setup the command dispatcher
	CommandDispatcher command_dispatcher( services );
	RegisterAllCommands( command_dispatcher );

	// Please keep the services list clean so it's easier to find something
	services.RegisterService(new AccountService(&services));
	services.RegisterService(new AgentMgrService(&services));
	services.RegisterService(new AggressionMgrService(&services));
	services.RegisterService(new AlertService(&services));
	services.RegisterService(new AuthService(&services));
	services.RegisterService(new BillMgrService(&services));
	services.RegisterService(new BeyonceService(&services));
	services.RegisterService(new BookmarkService(&services));
	services.RegisterService(new BrowserLockdownService(&services));
	services.RegisterService(new BulkMgrService(&services));
	services.RegisterService(new CertificateMgrService(&services));
	services.RegisterService(new CharFittingMgrService(&services));
	services.RegisterService(new CharUnboundMgrService(&services));
	services.RegisterService(new CharMgrService(&services));
	services.RegisterService(new ClientStatLogger(&services));
	services.RegisterService(new ClientStatsMgr(&services));
	services.RegisterService(new ConfigService(&services));
	services.RegisterService(new CorpBookmarkMgrService(&services));
	services.RegisterService(new CorpMgrService(&services));
	services.RegisterService(new CorporationService(&services));
	services.RegisterService(new CorpRegistryService(&services));
	services.RegisterService(new CorpStationMgrService(&services));
	services.RegisterService(new ContractMgrService(&services));
	services.RegisterService(new ContractProxyService(&services));
	services.RegisterService(new DevToolsProviderService(&services));
	services.RegisterService(new DogmaIMService(&services));
	services.RegisterService(new DogmaService(&services));
	services.RegisterService(new DungeonExplorationMgrService(&services));
	services.RegisterService(new DungeonService(&services));
	services.RegisterService(new FactionWarMgrService(&services));
	services.RegisterService(new FactoryService(&services));
	services.RegisterService(new FleetProxyService(&services));
	services.RegisterService(new HoloscreenMgrService(&services));
	services.RegisterService(new InfoGatheringMgr(&services));
	services.RegisterService(new InsuranceService(&services));
	services.RegisterService(new InvBrokerService(&services));
	services.RegisterService(new JumpCloneService(&services));
	services.RegisterService(new KeeperService(&services));
	services.RegisterService(new LanguageService(&services));
	services.RegisterService(new LocalizationServerService(&services));
	services.RegisterService(new LookupService(&services));
	services.RegisterService(new LPService(&services));
	services.RegisterService(services.lsc_service = new LSCService(&services, &command_dispatcher));
	services.RegisterService(new MailMgrService(&services));
	services.RegisterService(new MailingListMgrService(&services));
	services.RegisterService(new MapService(&services));
	services.RegisterService(new MarketProxyService(&services));
	services.RegisterService(new MissionMgrService(&services));
	services.RegisterService(new NetService(&services));
	services.RegisterService(new NotificationMgrService(&services));
	services.RegisterService(services.cache_service = new ObjCacheService(&services, sConfig.files.cacheDir.c_str()));
	services.RegisterService(new OnlineStatusService(&services));
	services.RegisterService(new PaperDollService(&services));
	services.RegisterService(new PetitionerService(&services));
	services.RegisterService(new PhotoUploadService(&services));
	services.RegisterService(new PlanetMgrService(&services));
	services.RegisterService(new PosMgrService(&services));
	services.RegisterService(new RamProxyService(&services));
	services.RegisterService(new RepairService(&services));
	services.RegisterService(new ReprocessingService(&services));
	services.RegisterService(new SearchMgrService(&services));
	services.RegisterService(new ShipService(&services));
	services.RegisterService(new SkillMgrService(&services));
	services.RegisterService(new SlashService(&services, &command_dispatcher));
	services.RegisterService(new SovereigntyMgrService(&services));
	services.RegisterService(new Standing2Service(&services));
	services.RegisterService(new StationService(&services));
	services.RegisterService(new StationSvcService(&services));
	services.RegisterService(new TutorialService(&services));
	services.RegisterService(new UserService(&services));
	services.RegisterService(new VoiceMgrService(&services));
	services.RegisterService(new WarRegistryService(&services));

	services.cache_service->PrimeCache();
}