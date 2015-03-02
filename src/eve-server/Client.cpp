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

long Client::nextID = 0;

Client::Client()
{
	//get unique id for this class
	id = ++nextID;
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
	sClientList.Remove(const_cast<Client&>(*this));
}
/*
void Client::_LoadSystems(EntityList &entityList, ItemFactory &itemFactory)
{
	// instantiate the service manager
	PyServiceMgr services( 888444, entityList, itemFactory );

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
*/