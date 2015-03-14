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

#include "cache/ObjCacheDB.h"

ObjCacheDB::ObjCacheDB()
{
    //register all the generators
    m_generators["charNewExtraCreationInfo.raceskills"] = &ObjCacheDB::Generate_CharNewExtraRaceSkills;
    m_generators["charNewExtraCreationInfo.careerskills"] = &ObjCacheDB::Generate_CharNewExtraCareerSkills;
    m_generators["charNewExtraCreationInfo.specialityskills"] = &ObjCacheDB::Generate_CharNewExtraSpecialitySkills;
    m_generators["charNewExtraCreationInfo.careers"] = &ObjCacheDB::Generate_CharNewExtraCareers;
    m_generators["charNewExtraCreationInfo.specialities"] = &ObjCacheDB::Generate_CharNewExtraSpecialities;

    m_generators["config.BulkData.paperdollResources"] = &ObjCacheDB::Generate_PaperdollResources;
    m_generators["config.BulkData.paperdollColors"] = &ObjCacheDB::Generate_PaperdollColors;
    m_generators["config.BulkData.paperdollModifierLocations"] = &ObjCacheDB::Generate_PaperdollModifierLocations;
    m_generators["config.BulkData.paperdollSculptingLocations"] = &ObjCacheDB::Generate_PaperdollSculptingLocations;
    m_generators["config.BulkData.paperdollColorNames"] = &ObjCacheDB::Generate_PaperdollColorNames;
    m_generators["config.BulkData.paperdollColorRestrictions"] = &ObjCacheDB::Generate_PaperdollColorRestrictions;

    m_generators["config.BulkData.bloodlineNames"] = &ObjCacheDB::Generate_BloodlineNames;
    m_generators["config.BulkData.locationscenes"] = &ObjCacheDB::Generate_Locationscenes;
    m_generators["config.BulkData.overviewDefaults"] = &ObjCacheDB::Generate_OverviewDefaults;
    m_generators["config.BulkData.schematicspinmap"] = &ObjCacheDB::Generate_Schematicspinmap;
    m_generators["config.BulkData.overviewDefaultGroups"] = &ObjCacheDB::Generate_OverviewDefaultGroups;
    m_generators["config.BulkData.schematics"] = &ObjCacheDB::Generate_Schematics;
    m_generators["config.BulkData.schematicstypemap"] = &ObjCacheDB::Generate_Schematicstypemap;
    m_generators["config.BulkData.sounds"] = &ObjCacheDB::Generate_Sounds;
    m_generators["config.BulkData.invtypematerials"] = &ObjCacheDB::Generate_Invtypematerials;
    m_generators["config.BulkData.ownericons"] = &ObjCacheDB::Generate_Ownericons;
    m_generators["config.BulkData.icons"] = &ObjCacheDB::Generate_Icons;

    m_generators["config.BulkData.billtypes"] = &ObjCacheDB::Generate_BillTypes;
    m_generators["config.BulkData.allianceshortnames"] = &ObjCacheDB::Generate_AllianceShortnames;
    m_generators["config.BulkData.categories"] = &ObjCacheDB::Generate_invCategories;
    m_generators["config.BulkData.invtypereactions"] = &ObjCacheDB::Generate_invTypeReactions;

    m_generators["config.BulkData.dgmtypeattribs"] = &ObjCacheDB::Generate_dgmTypeAttribs;
    m_generators["config.BulkData.dgmtypeeffects"] = &ObjCacheDB::Generate_dgmTypeEffects;
    m_generators["config.BulkData.dgmeffects"] = &ObjCacheDB::Generate_dgmEffects;
    m_generators["config.BulkData.dgmattribs"] = &ObjCacheDB::Generate_dgmAttribs;
    m_generators["config.BulkData.metagroups"] = &ObjCacheDB::Generate_invMetaGroups;
    m_generators["config.BulkData.ramactivities"] = &ObjCacheDB::Generate_ramActivities;
    m_generators["config.BulkData.ramaltypesdetailpergroup"] = &ObjCacheDB::Generate_ramALTypeGroup;
    m_generators["config.BulkData.ramaltypesdetailpercategory"] = &ObjCacheDB::Generate_ramALTypeCategory;
    m_generators["config.BulkData.ramaltypes"] = &ObjCacheDB::Generate_ramALTypes;
    m_generators["config.BulkData.ramcompletedstatuses"] = &ObjCacheDB::Generate_ramCompletedStatuses;
    m_generators["config.BulkData.ramtyperequirements"] = &ObjCacheDB::Generate_ramTypeRequirements;

    m_generators["config.BulkData.mapcelestialdescriptions"] = &ObjCacheDB::Generate_mapCelestialDescriptions;
    m_generators["config.BulkData.tickernames"] = &ObjCacheDB::Generate_tickerNames;
    m_generators["config.BulkData.groups"] = &ObjCacheDB::Generate_invGroups;
    m_generators["config.BulkData.certificates"] = &ObjCacheDB::Generate_certificates;
    m_generators["config.BulkData.certificaterelationships"] = &ObjCacheDB::Generate_certificateRelationships;
    m_generators["config.BulkData.shiptypes"] = &ObjCacheDB::Generate_invShipTypes;
    m_generators["config.BulkData.locations"] = &ObjCacheDB::Generate_cacheLocations;
    m_generators["config.BulkData.locationwormholeclasses"] = &ObjCacheDB::Generate_locationWormholeClasses;
    m_generators["config.BulkData.bptypes"] = &ObjCacheDB::Generate_invBlueprintTypes;
    m_generators["config.BulkData.graphics"] = &ObjCacheDB::Generate_eveGraphics;
    m_generators["config.BulkData.types"] = &ObjCacheDB::Generate_invTypes;
    m_generators["config.BulkData.invmetatypes"] = &ObjCacheDB::Generate_invMetaTypes;
    m_generators["config.Bloodlines"] = &ObjCacheDB::Generate_chrBloodlines;
    m_generators["config.Units"] = &ObjCacheDB::Generate_eveUnits;
    m_generators["config.BulkData.units"] = &ObjCacheDB::Generate_eveBulkDataUnits;
    m_generators["config.BulkData.owners"] = &ObjCacheDB::Generate_cacheOwners;
    m_generators["config.StaticOwners"] = &ObjCacheDB::Generate_eveStaticOwners;
    m_generators["config.Races"] = &ObjCacheDB::Generate_chrRaces;
    m_generators["config.Attributes"] = &ObjCacheDB::Generate_chrAttributes;
    m_generators["config.Flags"] = &ObjCacheDB::Generate_invFlags;
    m_generators["config.StaticLocations"] = &ObjCacheDB::Generate_eveStaticLocations;
    m_generators["config.InvContrabandTypes"] = &ObjCacheDB::Generate_invContrabandTypes;

    m_generators["charCreationInfo.bloodlines"] = &ObjCacheDB::Generate_c_chrBloodlines;
    m_generators["charCreationInfo.races"] = &ObjCacheDB::Generate_c_chrRaces;
    m_generators["charCreationInfo.ancestries"] = &ObjCacheDB::Generate_c_chrAncestries;
    m_generators["charCreationInfo.schools"] = &ObjCacheDB::Generate_c_chrSchools;
    m_generators["charCreationInfo.attributes"] = &ObjCacheDB::Generate_c_chrAttributes;
    m_generators["charCreationInfo.bl_accessories"] = &ObjCacheDB::Generate_bl_accessories;
    m_generators["charCreationInfo.bl_lights"] = &ObjCacheDB::Generate_bl_lights;
    m_generators["charCreationInfo.bl_skins"] = &ObjCacheDB::Generate_bl_skins;
    m_generators["charCreationInfo.bl_beards"] = &ObjCacheDB::Generate_bl_beards;
    m_generators["charCreationInfo.bl_eyes"] = &ObjCacheDB::Generate_bl_eyes;
    m_generators["charCreationInfo.bl_lipsticks"] = &ObjCacheDB::Generate_bl_lipsticks;
    m_generators["charCreationInfo.bl_makeups"] = &ObjCacheDB::Generate_bl_makeups;
    m_generators["charCreationInfo.bl_hairs"] = &ObjCacheDB::Generate_bl_hairs;
    m_generators["charCreationInfo.bl_backgrounds"] = &ObjCacheDB::Generate_bl_backgrounds;
    m_generators["charCreationInfo.bl_decos"] = &ObjCacheDB::Generate_bl_decos;
    m_generators["charCreationInfo.bl_eyebrows"] = &ObjCacheDB::Generate_bl_eyebrows;
    m_generators["charCreationInfo.bl_costumes"] = &ObjCacheDB::Generate_bl_costumes;

    m_generators["charCreationInfo.eyebrows"] = &ObjCacheDB::Generate_a_eyebrows;
    m_generators["charCreationInfo.eyes"] = &ObjCacheDB::Generate_a_eyes;
    m_generators["charCreationInfo.decos"] = &ObjCacheDB::Generate_a_decos;
    m_generators["charCreationInfo.hairs"] = &ObjCacheDB::Generate_a_hairs;
    m_generators["charCreationInfo.backgrounds"] = &ObjCacheDB::Generate_a_backgrounds;
    m_generators["charCreationInfo.accessories"] = &ObjCacheDB::Generate_a_accessories;
    m_generators["charCreationInfo.lights"] = &ObjCacheDB::Generate_a_lights;
    m_generators["charCreationInfo.costumes"] = &ObjCacheDB::Generate_a_costumes;
    m_generators["charCreationInfo.makeups"] = &ObjCacheDB::Generate_a_makeups;
    m_generators["charCreationInfo.beards"] = &ObjCacheDB::Generate_a_beards;
    m_generators["charCreationInfo.skins"] = &ObjCacheDB::Generate_a_skins;
    m_generators["charCreationInfo.lipsticks"] = &ObjCacheDB::Generate_a_lipsticks;
}

PyRep *ObjCacheDB::GetCachableObject(DBcore *db, const std::string &type)
{
    std::map<std::string, genFunc>::const_iterator res;
    res = m_generators.find(type);

    if(res == m_generators.end())
    {
        _log(SERVICE__ERROR, "Unable to find cachable object generator for type '%s'", type.c_str());
        return NULL;
    }

    genFunc f = res->second;
    return (this->*f)();
}

//implement all the generators:
PyRep *ObjCacheDB::Generate_CharNewExtraSpecialities(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT careerID, specialityID, specialityName, description, shortDescription, graphicID, iconID, dataID FROM specialities";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charNewExtraCreationInfo.specialities': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_CharNewExtraCareers(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT raceID, careerID, careerName, description, shortDescription, graphicID, schoolID, iconID, dataID FROM careers";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charNewExtraCreationInfo.careers': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_CharNewExtraSpecialitySkills(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT specialityID, skillTypeID, levels FROM specialitySkills";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charNewExtraCreationInfo.specialityskills': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_CharNewExtraCareerSkills(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT careerID, skillTypeID, levels FROM careerSkills";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charNewExtraCreationInfo.careerskills': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_CharNewExtraRaceSkills(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT raceID, skillTypeID, levels FROM raceSkills";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charNewExtraCreationInfo.raceSkills': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Icons(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT iconID, iconFile, description, obsolete, iconType FROM icons";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.icons': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Ownericons(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT ownerID, iconID FROM ownerIcons";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.ownerIcons': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Invtypematerials(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT typeID, materialTypeID, quantity FROM invTypeMaterials";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.invTypeMaterials': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Sounds(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT soundID, soundFile, description, obsolete FROM sounds";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.sounds': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Schematicstypemap(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT schematicID, typeID, quantity, isInput FROM schematicsTypeMap";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.schematicsTypeMap': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Schematics(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT schematicID, schematicName, cycleTime, dataID FROM schematics";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.schematics': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_OverviewDefaultGroups(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT overviewID, groupID FROM chrDefaultOverviewGroups";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.overviewDefaultGroups': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Schematicspinmap(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT schematicID, pinTypeID FROM schematicsPinMap";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.schematicsPinMap': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_OverviewDefaults(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT dataID, overviewID, overviewName, overviewShortName FROM chrDefaultOverviews";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.overviewDefaults': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_Locationscenes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT locationID, sceneID FROM locationScenes";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.locationScenes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_BloodlineNames(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT nameID, bloodlineID, lastName FROM chrBloodlineNames";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.bloodlineNames': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_PaperdollColors(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT colorID, colorKey, hasSecondary, hasWeight, hasGloss FROM paperdollColors";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.paperdollColors': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_PaperdollColorRestrictions(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT colorNameID, gender, restrictions FROM paperdollColorRestrictions";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.paperdollColorRestrictions': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_PaperdollColorNames(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT colorNameID, colorName FROM paperdollColorNames";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.paperdollColorNames': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_PaperdollSculptingLocations(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT sculptLocationID, weightKeyCategory, weightKeyPrefix FROM paperdollSculptingLocations";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.paperdollSculptingLocations': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_PaperdollModifierLocations(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT modifierLocationID, modifierKey, variationKey FROM paperdollModifierLocations";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.paperdollModifierLocations': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_PaperdollResources(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT paperdollResourceID, resGender, resPath, restrictions FROM paperdollResources";
    if (db->RunQuery(res, q) == false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.paperdollResources': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_BillTypes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT billTypeID,billTypeName,description FROM billTypes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.billtypes': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_AllianceShortnames(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT allianceID,shortName FROM alliance_ShortNames";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.alliance_ShortNames': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invCategories(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT categoryID, categoryName, description, 0 as graphicID, iconID, published, 0 as dataID FROM invCategories";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.categories': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invTypeReactions(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT reactionTypeID,input,typeID,quantity FROM invTypeReactions";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.invtypereactions': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_dgmTypeAttribs(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT    dgmTypeAttributes.typeID,    dgmTypeAttributes.attributeID,    IF(valueInt IS NULL, valueFloat, valueInt) AS value FROM dgmTypeAttributes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.dgmtypeattribs': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_dgmTypeEffects(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT typeID,effectID,isDefault FROM dgmTypeEffects";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.dgmtypeeffects': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_dgmEffects(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT effectID, effectName, effectCategory, preExpression, postExpression, description, guid, iconID, isOffensive, isAssistance, durationAttributeID, trackingSpeedAttributeID, dischargeAttributeID, rangeAttributeID, falloffAttributeID, published, displayName, isWarpSafe, rangeChance, electronicChance, propulsionChance, distribution, sfxName, npcUsageChanceAttributeID, npcActivationChanceAttributeID, 0 as graphicID, fittingUsageChanceAttributeID, 0 AS dataID FROM dgmEffects";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.dgmeffects': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_dgmAttribs(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT attributeID, attributeName, attributeCategory, description, maxAttributeID, attributeIdx, graphicID, chargeRechargeTimeID, defaultValue, published, unitID, displayName, stackable, highIsGood, iconID, dataID FROM dgmattribs";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.dgmattribs': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invMetaGroups(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT metaGroupID, metaGroupName, description, iconID, 0 as graphicID, 0 AS dataID FROM invMetaGroups";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.metagroups': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_ramActivities(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT activityID, activityName, iconNo, description, published FROM ramActivities";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.ramactivities': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_ramALTypeGroup(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT a.assemblyLineTypeID, b.activityID, a.groupID, a.timeMultiplier, a.materialMultiplier FROM ramAssemblyLineTypeDetailPerGroup AS a LEFT JOIN ramAssemblyLineTypes AS b ON a.assemblyLineTypeID = b.assemblyLineTypeID";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.ramaltypesdetailpergroup': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_ramALTypeCategory(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT a.assemblyLineTypeID, b.activityID, a.categoryID, a.timeMultiplier, a.materialMultiplier FROM ramAssemblyLineTypeDetailPerCategory AS a LEFT JOIN ramAssemblyLineTypes AS b ON a.assemblyLineTypeID = b.assemblyLineTypeID";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.ramaltypesdetailpercategory': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_ramALTypes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT assemblyLineTypeID, assemblyLineTypeName, assemblyLineTypeName AS typeName, description, activityID, baseTimeMultiplier, baseMaterialMultiplier, volume FROM ramAssemblyLineTypes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.ramaltypes': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_ramCompletedStatuses(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT completedStatusID, completedStatusName FROM ramCompletedStatuses";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.ramcompletedstatuses': %s",res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_ramTypeRequirements(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT typeID, activityID, requiredTypeID, quantity, damagePerJob, recycle FROM ramTypeRequirements";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.ramtyperequirements': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_mapCelestialDescriptions(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT celestialID, description FROM mapCelestialDescriptions";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.mapcelestialdescriptions': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_tickerNames(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT corporationID,tickerName,shape1,shape2,shape3,color1,color2,color3 FROM corporation WHERE hasPlayerPersonnelManager=0";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.tickernames': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invGroups(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT groupID, categoryID, groupName, description, iconID, 0 as graphicID, useBasePrice, allowManufacture, allowRecycler, anchored, anchorable, fittableNonSingleton, 1 AS published, 0 AS dataID FROM invGroups";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.groups': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_certificates(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT certificateID, categoryID, classID, grade, iconID, corpID, description, 0 AS dataID FROM crtCertificates";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.certificates': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_certificateRelationships(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT relationshipID, parentID, parentTypeID, parentLevel, childID, childTypeID FROM certificateRelationShips";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.certificaterelationships': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invShipTypes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT shipTypeID, weaponTypeID, miningTypeID, skillTypeID FROM shipTypes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.shiptypes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_cacheLocations(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT locationID, locationName, x, y, z FROM cacheLocations";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.locations': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_locationWormholeClasses(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT locationID, wormholeClassID FROM mapLocationWormholeClasses";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.locationwormholeclasses': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invBlueprintTypes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT blueprintTypeID, parentBlueprintTypeID, productTypeID, productionTime, techLevel, researchProductivityTime, researchMaterialTime, researchCopyTime, researchTechTime, productivityModifier, materialModifier, wasteFactor, chanceOfReverseEngineering, maxProductionLimit FROM bpTypes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.bptypes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_eveGraphics(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT graphicID, graphicFile, graphicName, description, obsolete, graphicType, collisionFile, paperdollFile, animationTemplate, collidable, explosionID, directoryID, graphicMinX, graphicMinY, graphicMinZ, graphicMaxX, graphicMaxY, graphicMaxZ from graphics";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.graphics': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invTypes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT typeID, groupID, typeName, description, graphicID, radius, mass, volume, capacity, portionSize, raceID, basePrice, published, marketGroupID, chanceOfDuplicating, soundID, iconID, dataID, typeNameID, descriptionID FROM invTypes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.types': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invMetaTypes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT typeID, parentTypeID, metaGroupID FROM invMetaTypes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.invmetatypes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_chrBloodlines(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, bloodlineName, raceID, description, maleDescription, femaleDescription, shipTypeID, corporationID, perception, willpower, charisma, memory, intelligence, shortDescription, shortMaleDescription, shortFemaleDescription, iconID, bloodlineNameID, descriptionID, dataID FROM chrBloodlines";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.Bloodlines': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToPackedRowList(res);
}

PyRep *ObjCacheDB::Generate_eveUnits(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT unitID, unitName, displayName FROM eveUnits";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.Units': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToPackedRowList(res);
}

PyRep *ObjCacheDB::Generate_eveBulkDataUnits(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT unitID, unitName, displayName FROM eveUnits";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.units': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_cacheOwners(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT ownerID, ownerName, typeID FROM cacheOwners";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.BulkData.owners': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_eveStaticOwners(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT ownerID, ownerName, typeID FROM eveStaticOwners";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.StaticOwners': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_chrRaces(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT raceID, raceName, description, iconID as graphicID, shortDescription, iconID, 0 AS dataID FROM chrRaces";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.Races': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToPackedRowList(res);
}

PyRep *ObjCacheDB::Generate_chrAttributes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT attributeID, attributeName, description, iconID, shortDescription, notes, iconID as graphicID FROM chrAttributes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.Attributes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToPackedRowList(res);
}

PyRep *ObjCacheDB::Generate_invFlags(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT flagID, flagName, flagName as flagLabel, flagName as flagGroup, flagName as description, orderID FROM invFlags";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.Flags': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToPackedRowList(res);
}

PyRep *ObjCacheDB::Generate_eveStaticLocations(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT locationID, locationName, x, y, z FROM eveStaticLocations";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.StaticLocations': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_invContrabandTypes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT factionID, typeID, standingLoss, confiscateMinSec, fineByValue, attackMinSec FROM invContrabandTypes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'config.InvContrabandTypes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_c_chrBloodlines(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, bloodlineName, raceID, description, maleDescription, femaleDescription, shipTypeID, corporationID, perception, willpower, charisma, memory, intelligence, shortDescription, shortMaleDescription, shortFemaleDescription, iconID, bloodlineNameID, descriptionID, dataID FROM chrBloodlines";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bloodlines': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_c_chrRaces(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT raceID, raceName, description, iconID, iconID AS graphicID, shortDescription, 0 AS dataID FROM chrRaces";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.races': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_c_chrAncestries(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT ancestryID, ancestryName, bloodlineID, description, perception, willpower, charisma, memory, intelligence, iconID, iconID AS graphicID, shortDescription, ancestryNameID, descriptionID, dataID FROM chrAncestries";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.ancestries': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_c_chrSchools(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT raceID, schoolID, schoolName, description, graphicID, chrSchools.corporationID, chrSchools.agentID, newAgentID, iconID, schoolNameID, descriptionID FROM chrSchools LEFT JOIN agtAgents USING (corporationID) GROUP BY schoolID";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.schools': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_c_chrAttributes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT attributeID, attributeName, description, iconID, iconID as graphicID, shortDescription, notes FROM chrAttributes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.attributes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_accessories(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, accessoryID FROM chrBLAccessories";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_accessories': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_lights(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT lightID, lightName FROM chrBLLights";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_lights': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_skins(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, skinID FROM chrBLSkins";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_skins': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_beards(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, beardID FROM chrBLBeards";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_beards': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_eyes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, eyesID FROM chrBLEyes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_eyes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_lipsticks(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, lipstickID FROM chrBLLipsticks";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_lipsticks': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_makeups(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, makeupID FROM chrBLMakeups";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_makeups': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_hairs(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, hairID FROM chrBLHairs";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_hairs': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_backgrounds(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT backgroundID, backgroundName FROM chrBLBackgrounds";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_backgrounds': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_decos(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, decoID FROM chrBLDecos";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_decos': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_eyebrows(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, eyebrowsID FROM chrBLEyebrows";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_eyebrows': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_bl_costumes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT bloodlineID, gender, costumeID FROM chrBLCostumes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.bl_costumes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_eyebrows(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT eyebrowsID, eyebrowsName FROM chrEyebrows";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.eyebrows': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_eyes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT eyesID, eyesName FROM chrEyes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.eyes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_decos(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT decoID, decoName FROM chrDecos";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.decos': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_hairs(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT hairID, hairName FROM chrHairs";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.hairs': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_backgrounds(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT backgroundID, backgroundName FROM chrBackgrounds";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.backgrounds': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_accessories(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT accessoryID, accessoryName FROM chrAccessories";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.accessories': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_lights(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT lightID, lightName FROM chrLights";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.lights': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_costumes(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT costumeID, costumeName FROM chrCostumes";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.costumes': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_makeups(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT makeupID, makeupName FROM chrMakeups";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.makeups': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_beards(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT beardID, beardName FROM chrBeards";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.beards': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_skins(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT skinID, skinName FROM chrSkins";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.skins': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}

PyRep *ObjCacheDB::Generate_a_lipsticks(DBcore *db)
{
    DBQueryResult res;
    const char *q = "SELECT lipstickID, lipstickName FROM chrLipsticks";
    if(db->RunQuery(res, q)==false)
    {
        _log(SERVICE__ERROR, "Error in query for cached object 'charCreationInfo.lipsticks': %s", res.error.c_str());
        return NULL;
    }
    return DBResultToCRowset(res);
}
