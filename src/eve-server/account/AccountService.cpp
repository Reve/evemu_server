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
    Author:        Zhur
*/

#include "eve-server.h"

#include "EntityList.h"
#include "PyServiceCD.h"
#include "account/AccountService.h"
#include "cache/ObjCacheService.h"

PyCallable_Make_InnerDispatcher(AccountService)

AccountService::AccountService(PyServiceMgr *mgr, DBcore *db): 
	PyService(mgr, "account"), 
	m_db(db),
	m_dispatch(new Dispatcher(this))
{
    _SetCallDispatcher(m_dispatch);

    PyCallable_REG_CALL(AccountService, GetCashBalance)
    PyCallable_REG_CALL(AccountService, GetEntryTypes)
    PyCallable_REG_CALL(AccountService, GetKeyMap)
    PyCallable_REG_CALL(AccountService, GiveCash)
    PyCallable_REG_CALL(AccountService, GiveCashFromCorpAccount)
    PyCallable_REG_CALL(AccountService, GetJournal)
    PyCallable_REG_CALL(AccountService, GetWalletDivisionsInfo)
}

AccountService::~AccountService() {
    delete m_dispatch;
}

PyResult AccountService::Handle_GetCashBalance(PyCallArgs &call) {
    const int32 ACCOUNT_KEY_AURUM = 1200;

    bool hasAccountKey = false;
    int32 accountKey = 0;
    if (call.byname.find("accountKey") != call.byname.end())
    {
        hasAccountKey = true;
        accountKey = call.byname.find("accountKey")->second->AsInt()->value();
    }

    if (call.tuple->size() >= 1)
    {
        Call_SingleArg args;
        if(!args.Decode(&call.tuple)) {
            args.arg = new PyInt(0);
        }

        //we can get an integer or a boolean right now...
        bool corporate_wallet = false;

        if( args.arg->IsInt() )
            corporate_wallet = ( args.arg->AsInt()->value() != 0 );
        else if( args.arg->IsBool() )
            corporate_wallet = args.arg->AsBool()->value();
        else
        {
            codelog(CLIENT__ERROR, "Invalid arguments");
            return NULL;
        }

        if(corporate_wallet)
            //corporate wallet
            return new PyFloat( account_db.GetCorpBalance(m_db, call.player->GetCorporationID() ) );
        else
            //personal wallet
            return new PyFloat( call.player->GetBalance() );
    }
    else if (hasAccountKey && accountKey == ACCOUNT_KEY_AURUM)
    {
        return new PyFloat(call.player->GetAurBalance());
    }

	// FAILSAFE:
	return new PyNone();
}

//givecash takes (ownerID, retval['qty'], retval['reason'][:40])
//GiveCashFromCorpAccount(ownerID, retval['qty'], retval['reason'][:40])
// notify OnAccountChange:
//         accountKey: 'cash', ownerID: charID or corpID, new balance

PyResult AccountService::Handle_GetEntryTypes(PyCallArgs &call) {
    PyRep *result = NULL;

    ObjectCachedMethodID method_id(GetName(), "GetEntryTypes");

    //check to see if this method is in the cache already.
    if(!m_manager->cache_service->IsCacheLoaded(method_id)) {
        //this method is not in cache yet, load up the contents and cache it.
        result = account_db.GetEntryTypes(m_db);
        if(result == NULL) {
            codelog(SERVICE__ERROR, "Failed to load cache, generating empty contents.");
            result = new PyNone();
        }
        m_manager->cache_service->GiveCache(method_id, &result);
    }

    //now we know its in the cache one way or the other, so build a
    //cached object cached method call result.
    result = m_manager->cache_service->MakeObjectCachedMethodCallResult(method_id);

    return result;
}

PyResult AccountService::Handle_GetKeyMap(PyCallArgs &call) {
    PyRep *result = NULL;

    ObjectCachedMethodID method_id(GetName(), "GetKeyMap");

    //check to see if this method is in the cache already.
    if(!m_manager->cache_service->IsCacheLoaded(method_id)) {
        //this method is not in cache yet, load up the contents and cache it.
        result = account_db.GetKeyMap(m_db);
        if(result == NULL) {
            codelog(SERVICE__ERROR, "Failed to load cache, generating empty contents.");
            result = new PyNone();
        }
        m_manager->cache_service->GiveCache(method_id, &result);
    }

    //now we know its in the cache one way or the other, so build a
    //cached object cached method call result.
    result = m_manager->cache_service->MakeObjectCachedMethodCallResult(method_id);

    return result;
}

//give cash takes (ownerID, retval['qty'], retval['reason'][:40])
PyResult AccountService::Handle_GiveCash(PyCallArgs &call) {
    Call_GiveCash args;
    if(!args.Decode(&call.tuple)) {
        codelog(CLIENT__ERROR, "Invalid arguments");
        return NULL;
    }

    if(args.amount == 0)
        return NULL;

    if(args.amount < 0 || args.amount > call.player->GetBalance()) {
        _log(CLIENT__ERROR, "%s: Invalid amount in GiveCash(): %.2f", call.player->GetName(), args.amount);
        call.player->SendErrorMsg("Invalid amount '%.2f'", args.amount);
        return NULL;
    }

    SystemManager *system = call.player->System();
    if(system == NULL) {
        codelog(CLIENT__ERROR, "%s: bad system", call.player->GetName());
        return NULL;
    }

    //NOTE: this will need work once we reorganize the entity list...
    bool targetIsChar;
    Player *other = m_manager->entity_list.FindCharacter(args.destination);
    if(other == NULL) {
        // then the money has to be sent to a corporation...
        // check this too
        if (account_db.CheckIfCorporation(m_db, args.destination)) {
            targetIsChar = false;
        } else {
            _log(CLIENT__ERROR, "%s: Failed to find character %u", call.player->GetName(), args.destination);
            call.player->SendErrorMsg("Unable to find the target");
            return NULL;
        }
    } else {
        targetIsChar = true;
    }


    if (targetIsChar) {
        return GiveCashToChar(call.player, other, args.amount, args.reason.c_str(), RefType_playerDonation);
    } else {
        // here comes the corp's stuff
        return GiveCashToCorp(call.player, args.destination, args.amount, args.reason.c_str(), RefType_playerDonation);
    }
}

PyTuple * AccountService::GiveCashToCorp(Player * const player, uint32 corpID, double amount, const char *reason, JournalRefType refTypeID) {
    if(!player->AddBalance(-amount)) {
        _log(CLIENT__ERROR, "%s: Failed to remove %.2f ISK from %u for donation to %u",
            player->GetName(),
            amount,
            player->GetCharacterID(),
            corpID );
        player->SendErrorMsg("Failed to transfer money from your account.");
        return NULL;
    }
    if(!account_db.AddBalanceToCorp(m_db, corpID, amount)) {
        _log(CLIENT__ERROR, "%s: Failed to add %.2f ISK to %u for donation from %u",
            player->GetName(),
            amount,
            corpID,
            player->GetCharacterID());
        player->SendErrorMsg("Failed to transfer money to your destination.");

        //try to refund the money..
        player->AddBalance(amount);

        return NULL;
    }

    double cnb = account_db.GetCorpBalance(m_db, corpID);

    // Send notification about the cash change
    OnAccountChange oac;
    oac.accountKey = "cash";
    oac.balance = cnb;
    oac.ownerid = corpID;
    PyTuple * answer = oac.Encode();

    MulticastTarget mct;
    mct.corporations.insert(corpID);
    m_manager->entity_list.Multicast("OnAccountChange", "*corpid&corpAccountKey", &answer, mct);

    //record the transactions in the wallet.
    if(!account_db.GiveCash( m_db,
        player->GetCharacterID(),
        refTypeID,
        player->GetCharacterID(),
        corpID,
        "unknown",
        player->GetAccountID(),
        accountCash,
        -amount,
        player->GetBalance(),
        reason
        )
    ) {
        codelog(CLIENT__ERROR, "Failed to record transaction on sending side");
        //no good reason to return... the money has actually been moved.
    }

    if(!account_db.GiveCash(m_db,
        corpID,
        refTypeID,
        player->GetCharacterID(),
        corpID,
        "unknown",
        corpID,
        accountCash,
        amount,
        cnb,
        reason
        )
    ) {
        codelog(CLIENT__ERROR, "Failed to record transaction on recieveing side");
        //no good reason to return... the money has actually been moved.
    }

    //send back the new balance
    PyTuple *ans= new PyTuple(2);
    ans->items[0]=new PyFloat(cnb);//new balance
    ans->items[1]=new PyFloat(cnb);//new balance, not an error need to send it 2 times

    return ans;
}

PyTuple * AccountService::GiveCashToChar(Player * const player, Player * const other, double amount, const char *reason, JournalRefType refTypeID) {
    if(!player->AddBalance(-amount)) {
        _log(CLIENT__ERROR, "%s: Failed to remove %.2f ISK from %u for donation to %u",
            player->GetName(),
            amount,
            player->GetCharacterID(),
            other->GetCharacterID() );
        player->SendErrorMsg("Failed to transfer money from your account.");
        return NULL;
    }
    if(!other->AddBalance(amount)) {
        _log(CLIENT__ERROR, "%s: Failed to add %.2f ISK to %u for donation from %u",
            player->GetName(),
            amount,
            other->GetCharacterID(),
            player->GetCharacterID());
        player->SendErrorMsg("Failed to transfer money to your destination.");

        //try to refund the money..
        player->AddBalance(amount);

        return NULL;
    }

    //record the transactions in the wallet.
    //first on the send side.
    if(!account_db.GiveCash(m_db,
        player->GetCharacterID(),
        refTypeID,
        player->GetCharacterID(),
        other->GetCharacterID(),
        "unknown",
        player->GetAccountID(),
        accountCash,
        -amount,
        player->GetBalance(),
        reason
        )
    ) {
        codelog(CLIENT__ERROR, "Failed to record transaction on recieveing side");
        //no good reason to return... the money has actually been moved.
    }

    //then on the receive side.
    if(!account_db.GiveCash(m_db,
        other->GetCharacterID(),
        refTypeID,
        other->GetCharacterID(),
        player->GetCharacterID(),
        "unknown",
        other->GetAccountID(),
        accountCash,
        amount,
        other->GetBalance(),
        reason
        )
    ) {
        codelog(CLIENT__ERROR, "Failed to record transaction on sending side");
        //no good reason to return... the money has actually been moved.
    }


    //send back the new balance
    PyTuple *ans= new PyTuple(2);
    ans->items[0]=new PyFloat(player->GetBalance());//new balance
    ans->items[1]=new PyFloat(player->GetBalance());//new balance, not an error need to send it 2 times

    return ans;
}

PyResult AccountService::Handle_GetJournal(PyCallArgs &call) {
    Call_GetJournal args;
    if(!args.Decode(&call.tuple)) {
        codelog(CLIENT__ERROR, "Invalid arguments");
        return NULL;
    }

    bool ca = false;
    if( args.corpAccount->IsBool() )
        ca = args.corpAccount->AsBool()->value();
    else if( args.corpAccount->IsInt() )
        ca = ( args.corpAccount->AsInt()->value() != 0 );
    else
    {
        // problem
        _log( SERVICE__WARNING, "%s: Unsupported value for corpAccount", GetName() );

        return NULL;
    }

    return account_db.GetJournal(m_db,
        ( ca ? call.player->GetCorporationID() : call.player->GetCharacterID() ),
        args.refTypeID,
        args.accountKey,
        args.fromDate
    );
}

PyResult AccountService::Handle_GiveCashFromCorpAccount(PyCallArgs &call) {
    Call_GiveCash args;
    if(!args.Decode(&call.tuple)) {
        codelog(CLIENT__ERROR, "Invalid arguments");
        return NULL;
    }

    if(args.amount == 0)
        return NULL;

    if(args.amount < 0 || args.amount > account_db.GetCorpBalance(m_db, call.player->GetCorporationID())) {
        _log(CLIENT__ERROR, "%s: Invalid amount in GiveCashFromCorpAccount(): %.2f", call.player->GetName(), args.amount);
        call.player->SendErrorMsg("Invalid amount '%.2f'", args.amount);
        return NULL;
    }

    SystemManager *system = call.player->System();
    if(system == NULL) {
        codelog(CLIENT__ERROR, "%s: bad system", call.player->GetName());
        return NULL;
    }

    //NOTE: this will need work once we reorganize the entity list...
    Player *other = m_manager->entity_list.FindCharacter(args.destination);
    if(other == NULL) {
        _log(CLIENT__ERROR, "%s: Failed to find character %u", call.player->GetName(), args.destination);
        call.player->SendErrorMsg("Unable to find the target");
        return NULL;
    }


    return WithdrawCashToChar(call.player, other, args.amount, args.reason.c_str(), RefType_corpAccountWithdrawal);
}

PyTuple * AccountService::WithdrawCashToChar(Player * const player, Player * const other, double amount, const char *reason, JournalRefType refTypeID) {
    // remove money from the corp
    uint32 corpID = player->GetCorporationID();
    if (!account_db.AddBalanceToCorp(m_db, corpID, double(-amount))) {
        _log(CLIENT__ERROR, "%s: Failed to remove %.2f ISK from %u for withdrawal to %u",
            player->GetName(),
            amount,
            corpID,
            other->GetCharacterID() );
        player->SendErrorMsg("Failed to transfer money from your account.");
        return NULL;
    }

    double ncb = account_db.GetCorpBalance(m_db, corpID);

    // Send notification about the cash change
    OnAccountChange oac;
    oac.accountKey = "cash";
    oac.balance = ncb;
    oac.ownerid = corpID;
    PyTuple * answer = oac.Encode();

    MulticastTarget mct;
    mct.corporations.insert(corpID);
    m_manager->entity_list.Multicast("OnAccountChange", "*corpid&corpAccountKey", &answer, mct);

    if(!other->AddBalance(amount)) {
        _log(CLIENT__ERROR, "%s: Failed to add %.2f ISK to %u for donation from %u",
            player->GetName(),
            amount,
            corpID,
            player->GetCharacterID());
        player->SendErrorMsg("Failed to transfer money to your destination.");

        //try to refund the money..
        account_db.AddBalanceToCorp(m_db, corpID, double(amount));
        // if we're here, we have a more serious problem than
        // corp's balance not being displayed properly, so i won't bother with it

        return NULL;
    }

    //record the transactions in the wallet.
    //first on the send side.
    char argID[15];
    snprintf(argID, 14, "%u", player->GetCharacterID());
    if(!account_db.GiveCash(m_db,
        corpID,
        refTypeID,
        corpID,
        other->GetCharacterID(),
        argID,
        corpID,
        accountCash,
        -amount,
        ncb,
        reason
        )
    ) {
        codelog(CLIENT__ERROR, "Failed to record transaction on recieveing side");
        //no good reason to return... the money has actually been moved.
    }

    //then on the receive side.
    if(!account_db.GiveCash(m_db,
        other->GetCharacterID(),
        refTypeID,
        corpID,
        other->GetCharacterID(),
        argID,
        other->GetAccountID(),
        accountCash,
        amount,
        other->GetBalance(),
        reason
        )
    ) {
        codelog(CLIENT__ERROR, "Failed to record transaction on sending side");
        //no good reason to return... the money has actually been moved.
    }


    //send back the new balance
    PyTuple *ans= new PyTuple(2);

    // maybe this needs it this way, just like the other ones...
    // i'm not sure, but it works for sure
    ans->items[0]=new PyFloat(ncb);
    ans->items[1]=new PyFloat(ncb);

    return ans;
}

PyResult AccountService::Handle_GetWalletDivisionsInfo(PyCallArgs &call)
{
  /*  Empty Call  */
    PyRep *result = NULL;

    return result;
}
