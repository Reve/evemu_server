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

#ifndef __OBJCACHEDB_H_INCL__
#define __OBJCACHEDB_H_INCL__

#include "ServiceDB.h"

class PyRep;
class DBQueryResult;

class ObjCacheDB
: public ServiceDB
{
public:
    ObjCacheDB();

    PyRep *GetCachableObject(DBcore *db, const std::string &type);

protected:
    typedef PyRep *(ObjCacheDB::* genFunc)();
    std::map<std::string, genFunc> m_generators;

    //hack:
    PyRep *DBResultToRowsetTuple(DBQueryResult &result);

    //declare all the generators
    PyRep *Generate_PaperdollResources(DBcore *db);
    PyRep *Generate_PaperdollColors(DBcore *db);
    PyRep *Generate_PaperdollModifierLocations(DBcore *db);
    PyRep *Generate_PaperdollSculptingLocations(DBcore *db);
    PyRep *Generate_PaperdollColorNames(DBcore *db);
    PyRep *Generate_PaperdollColorRestrictions(DBcore *db);
    PyRep *Generate_BloodlineNames(DBcore *db);
    PyRep *Generate_Locationscenes(DBcore *db);
    PyRep *Generate_OverviewDefaults(DBcore *db);
    PyRep *Generate_Schematicspinmap(DBcore *db);
    PyRep *Generate_OverviewDefaultGroups(DBcore *db);
    PyRep *Generate_Schematics(DBcore *db);
    PyRep *Generate_Schematicstypemap(DBcore *db);
    PyRep *Generate_Sounds(DBcore *db);
    PyRep *Generate_Invtypematerials(DBcore *db);
    PyRep *Generate_Ownericons(DBcore *db);
    PyRep *Generate_Icons(DBcore *db);
    PyRep *Generate_CharNewExtraRaceSkills(DBcore *db);
    PyRep *Generate_CharNewExtraCareerSkills(DBcore *db);
    PyRep *Generate_CharNewExtraSpecialitySkills(DBcore *db);
    PyRep *Generate_CharNewExtraCareers(DBcore *db);
    PyRep *Generate_CharNewExtraSpecialities(DBcore *db);

    PyRep *Generate_BillTypes(DBcore *db);
    PyRep *Generate_AllianceShortnames(DBcore *db);
    PyRep *Generate_invCategories(DBcore *db);
    PyRep *Generate_invTypeReactions(DBcore *db);

    PyRep *Generate_dgmTypeAttribs(DBcore *db);
    PyRep *Generate_dgmTypeEffects(DBcore *db);
    PyRep *Generate_dgmEffects(DBcore *db);
    PyRep *Generate_dgmAttribs(DBcore *db);

    PyRep *Generate_invMetaGroups(DBcore *db);

    PyRep *Generate_ramActivities(DBcore *db);
    PyRep *Generate_ramALTypeGroup(DBcore *db);
    PyRep *Generate_ramALTypeCategory(DBcore *db);
    PyRep *Generate_ramALTypes(DBcore *db);
    PyRep *Generate_ramCompletedStatuses(DBcore *db);
    PyRep *Generate_ramTypeRequirements(DBcore *db);

    PyRep *Generate_mapCelestialDescriptions(DBcore *db);
    PyRep *Generate_tickerNames(DBcore *db);
    PyRep *Generate_invGroups(DBcore *db);
    PyRep *Generate_certificates(DBcore *db);
    PyRep *Generate_certificateRelationships(DBcore *db);
    PyRep *Generate_invShipTypes(DBcore *db);
    PyRep *Generate_cacheLocations(DBcore *db);
    PyRep *Generate_locationWormholeClasses(DBcore *db);
    PyRep *Generate_invBlueprintTypes(DBcore *db);
    PyRep *Generate_eveGraphics(DBcore *db);
    PyRep *Generate_invTypes(DBcore *db);
    PyRep *Generate_invMetaTypes(DBcore *db);
    PyRep *Generate_chrBloodlines(DBcore *db);
    PyRep *Generate_eveUnits(DBcore *db);
    PyRep *Generate_eveBulkDataUnits(DBcore *db);
    PyRep *Generate_cacheOwners(DBcore *db);
    PyRep *Generate_eveStaticOwners(DBcore *db);
    PyRep *Generate_chrRaces(DBcore *db);
    PyRep *Generate_chrAttributes(DBcore *db);
    PyRep *Generate_invFlags(DBcore *db);
    PyRep *Generate_eveStaticLocations(DBcore *db);
    PyRep *Generate_invContrabandTypes(DBcore *db);

    PyRep *Generate_c_chrBloodlines(DBcore *db);
    PyRep *Generate_c_chrRaces(DBcore *db);
    PyRep *Generate_c_chrAncestries(DBcore *db);
    PyRep *Generate_c_chrSchools(DBcore *db);
    PyRep *Generate_c_chrAttributes(DBcore *db);
    PyRep *Generate_bl_accessories(DBcore *db);
    PyRep *Generate_bl_lights(DBcore *db);
    PyRep *Generate_bl_skins(DBcore *db);
    PyRep *Generate_bl_beards(DBcore *db);
    PyRep *Generate_bl_eyes(DBcore *db);
    PyRep *Generate_bl_lipsticks(DBcore *db);
    PyRep *Generate_bl_makeups(DBcore *db);
    PyRep *Generate_bl_hairs(DBcore *db);
    PyRep *Generate_bl_backgrounds(DBcore *db);
    PyRep *Generate_bl_decos(DBcore *db);
    PyRep *Generate_bl_eyebrows(DBcore *db);
    PyRep *Generate_bl_costumes(DBcore *db);

    PyRep *Generate_a_eyebrows(DBcore *db);
    PyRep *Generate_a_eyes(DBcore *db);
    PyRep *Generate_a_decos(DBcore *db);
    PyRep *Generate_a_hairs(DBcore *db);
    PyRep *Generate_a_backgrounds(DBcore *db);
    PyRep *Generate_a_accessories(DBcore *db);
    PyRep *Generate_a_lights(DBcore *db);
    PyRep *Generate_a_costumes(DBcore *db);
    PyRep *Generate_a_makeups(DBcore *db);
    PyRep *Generate_a_beards(DBcore *db);
    PyRep *Generate_a_skins(DBcore *db);
    PyRep *Generate_a_lipsticks(DBcore *db);
};

#endif
