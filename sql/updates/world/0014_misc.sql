-- fix text for Brood Queen Typanna
DELETE FROM `creature_text` WHERE `entry` = 95048;
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(95048, 0, 0, 'Deal with these insects, Beliash.', 12, 0, 100, 0, 0, 55071, 96146, 100420, 100420, 0, 'Tyranna to Beliash');
-- kneel state for Beliash
DELETE FROM `creature_addon` WHERE `guid` = 367814;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(367814, 0, 0, 8, 0, 0, NULL);
-- update beliash script name
UPDATE `creature_template` SET `ScriptName` = 'npc_doom_commander_beliash' WHERE `ScriptName` = 'npc_q93221_beliash' AND `entry` = 93221;
UPDATE `creature_template` SET `ScriptName` = 'npc_sevis_brightflame_sacrifice' WHERE `ScriptName` = 'npc_q38765' AND `entry` = 99915;
-- Gauntlet Event
UPDATE `creature_template` SET `AIName`="", `ScriptName`='npc_mardum_devastator', `minlevel` = 98, `maxlevel` = 100, `faction`=190, `unit_flags`=65536 WHERE `entry`=100161;
-- Test Above
UPDATE `quest_template_addon` SET `NextQuestID`=0 WHERE `ID`=38727;
UPDATE `quest_template_addon` SET `PrevQuestID`=39495 WHERE `ID`=38725;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_general_volroth' WHERE `entry`=97370;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_count_nefarious' WHERE `entry`=97058;
-- Disables
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES (1, 3096, 0, "", "", "Deprecated Quest - Encrypted Scroll");
-- General Volroth and Count Nefarius
UPDATE `gameobject_template` SET `ScriptName`='go_well_of_souls' WHERE `entry`=244638;
-- King Voras
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_king_voras' WHERE `entry`=97059;
UPDATE `creature_template` SET `ScriptName`='npc_mardum_gaardoun', `AIName`='SmartAI' WHERE `entry`=97459;
DELETE FROM `creature_text` WHERE `Entry` = 97059;
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(97059, 0, 0, 'Want your little, broken draenei back?', 12, 0, 100, 0, 0, 55281, 0, 0, 0, 0, 'King Voras to Player'),
(97059, 1, 0, 'My queen\'s brood will hatch soon.', 14, 0, 100, 0, 0, 55283, 0, 0,  0, 0, 'King Voras to Player'),
(97059, 2, 0, 'They\'ve slain me, my queen...', 14, 0, 100, 0, 0, 55282, 0, 0,  0, 0, 'King Voras to Player');
DELETE FROM `creature_text` WHERE `Entry`=97459;
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(97459, 0, 0, '$p, this is embarassing...', 12, 0, 100, 0, 0, 55049, 0, 0,  0, 0, 'Gaardoun to Player'),
(97459, 1, 0, 'My forces are decimated. I have failed you.', 12, 0, 100, 0, 0, 55050, 0, 0,  0, 0, 'Gaardoun to Player'),
(97459, 2, 0, 'I swear to give my life if that is what it takes', 12, 0, 100, 0, 0, 55051, 0, 0,  0, 0, 'Gaardoun to Player');
-- Remove Spider Eggs without model
DELETE FROM `creature` WHERE `id`=100717;
-- Spider Egg (45286)
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=100719;
DELETE FROM `smart_scripts` WHERE `entryorguid`=100719;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param_string`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(100719, 0, 0, 0, 11, 0, 100, 1, 0, 0, 0, 0, '', 11, 198239, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spider Egg - On Spawn - Cast Hatching');
UPDATE `creature_template` SET `faction` = '7' WHERE `entry` = '100719';
UPDATE `creature_template` SET `faction` = '1786' WHERE `entry` = '100723';
-- Hatchling Creeper SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=100723;
DELETE FROM `smart_scripts` WHERE `entryorguid`=100723 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(100723,0,0,0,0,0,100,0,5000,8000,12000,15000,11,213758,0,0,0,0,0,2,0,0,0,0,0,0,0,"Hatchling Creeper - In Combat - Cast 'Spiderling Venom'");
DELETE FROM `creature` WHERE `id`=101781;
-- BreadCrumb Quest "Cry Havoc And Let Slip the Illidari"
UPDATE `quest_template_addon` SET `PrevQuestID`=40051 WHERE `ID`=39516;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=19 AND `SourceEntry` IN (39516,39518);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19,0,39516,0,0,14,0,39518,0,0,0,0,"","Quest 'Cry Havoc And Let Slip the Illidari' can only be taken if quest 'Vengeance Spec Tracking Event' is not taken"),
(19,0,39518,0,0,28,0,39516,0,0,1,0,"","Quest 'Vengeance Spec Tracking Event' can only be taken if quest 'Cry Havoc And Let Slip the Illidari' is not completed");
-- BreadCrumb Quest "Vengeance Will Be Mine"
UPDATE `quest_template_addon` SET `PrevQuestID`=40051 WHERE `ID`=39515;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=19 AND `SourceEntry` IN (39515,39517);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19,0,39515,0,0,14,0,39517,0,0,0,0,"","Quest 'Vengeance Will Be Mine' can only be taken if quest 'Havoc Spec Tracking Event' is not taken"),
(19,0,39517,0,0,28,0,39515,0,0,1,0,"","Quest 'Havoc Spec Tracking Event' can only be taken if quest 'Vengeance Will Be Mine' is not completed");
-- Make "On Felbat Wings" being available after breadcrumb quests
UPDATE `quest_template_addon` SET `PrevQuestID`=40051, `NextQuestID`=39663, `ExclusiveGroup`=39515 WHERE `ID` IN (39515, 39516);
UPDATE `quest_template_addon` SET `PrevQuestID`=39515 WHERE `ID`=39663;
-- Visual updates for teaching DH
UPDATE `creature_template` SET `ScriptName` = "npc_mardum_dh_taught" WHERE `entry` in (96655, 99045, 96420, 96652);
-- Kayn Sunfury Invasion SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=98229;
DELETE FROM `smart_scripts` WHERE `entryorguid`=98229 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(98229,0,0,0,19,0,100,0,40378,0,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury - On Quest 'Enter the Illidari: Ashtongue' Taken - Say Line 0"),
(98229,0,1,0,52,0,100,0,0,98229,0,0,1,1,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Kayn Sunfury - On Text 0 Over - Say Line 1"),
(98229,0,2,0,52,0,100,0,1,98229,0,0,1,0,3000,0,0,0,0,11,98354,30,0,0,0,0,0,"Kayn Sunfury - On Text 1 Over - Say Line 0");
DELETE FROM `creature_text` WHERE `Entry` = 98229;
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(98229,0,0,'Activate all three gateways and summon in the rest of our forces.',12,0,100,0,0,55244,0,0,0,0,'Kayn Sunfury to Player'),
(98229,1,0,'I\'ll cut a hole through the demons and meet you in the volcano.',12,0,100,0,0,55245,0,0,0,0,'Kayn Sunfury to Player');
-- Kor'vas Bloodthorn SAI
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=98354;
DELETE FROM `smart_scripts` WHERE `entryorguid`=98354 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(98354,0,0,0,52,0,100,0,1,98229,0,0,1,0,2000,0,0,0,0,11,98229,50,0,0,0,0,0,"Kor'vas Bloodthorn - On Text 1 Over - Say Line 0");
-- Korvas text
DELETE FROM `creature_text` WHERE `Entry` = 98354;
INSERT INTO `creature_text` (`Entry`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `MinTimer`, `MaxTimer`, `SpellID`, `comment`) VALUES
(98354, 0, 0, 'I will join you. Let\'s kill some more demons.', 12, 0, 100, 0, 0, 55285, 0, 0, 0, 0, 'Korvas Bloodthorn to Player'),
(98354, 1, 0, 'I\'m with you. Kill more demons.', 12, 0, 100, 0, 0, 55285, 96404, 12338, 12338, 0, 'Korvas Bloodthorn to Player');
-- Inquisitor Baleful
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_inquisitor_baleful' WHERE `entry`=93105;
DELETE FROM `creature` WHERE `guid` IN (367685) AND `id`=96159; -- 367686
-- Brood Queen Tyranna near Beliash
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `PhaseId`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `AiID`, `MovementID`, `MeleeID`, `isActive`, `skipClone`, `personal_size`, `isTeemingSpawn`, `unit_flags3`) VALUES
(367685,95048,1481,0,0,1,1,'',0,0,1945.06,1562.81,82.3541,1.18682,120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
-- Make Tyranna visible and not attackable
-- UPDATE `creature` SET `PhaseId` = '0' WHERE `guid` = '367685';
UPDATE `creature_addon` SET `auras` = '' WHERE `guid` = '367685';
UPDATE `creature_template_addon` SET `auras` = '' WHERE `entry` = '95048';
UPDATE `creature_template` SET `unit_flags` = '196864' WHERE `entry` = '95048';
-- Update Spells
UPDATE `spell_script_names` SET `ScriptName`='spell_destroying_fel_spreader' WHERE `ScriptName`='spell_legion_q39279';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(201112, 'spell_destroying_stabilizer'),
(192252, 'spell_destroying_soulharvester');
-- clear npc
DELETE FROM `creature` WHERE `guid` IN (368347,
368348,
368349,
368350,
368351,
368353,
368354,
368355,
368359,
368361,
368362,
368363,
368364,
368365
);
DELETE FROM `creature_addon` WHERE `guid` IN (368347,
368348,
368349,
368350,
368351,
368353,
368354,
368355,
368359,
368361,
368362,
368363,
368364,
368365
);
-------------------