/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2022 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#include "pch.hpp"

#include "creatures/players/grouping/party.hpp"
#include "creatures/players/player.hpp"
#include "game/game.hpp"
#include "lua/functions/creatures/player/party_functions.hpp"

int32_t PartyFunctions::luaPartyCreate(lua_State* L) {
	// Party(userdata)
	Player* player = getUserdata<Player>(L, 2);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Party* party = player->getParty();
	if (!party) {
		party = new Party(player);
		g_game().updatePlayerShield(player);
		player->sendCreatureSkull(player);
		pushUserdata<Party>(L, party);
		setMetatable(L, -1, "Party");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyDisband(lua_State* L) {
	// party:disband()
	Party** partyPtr = getRawUserdata<Party>(L, 1);
	if (partyPtr && *partyPtr) {
		Party*&party = *partyPtr;
		party->disband();
		party = nullptr;
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyGetLeader(lua_State* L) {
	// party:getLeader()
	Party* party = getUserdata<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	Player* leader = party->getLeader();
	if (leader) {
		pushUserdata<Player>(L, leader);
		setMetatable(L, -1, "Player");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartySetLeader(lua_State* L) {
	// party:setLeader(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->passPartyLeadership(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyGetMembers(lua_State* L) {
	// party:getMembers()
	Party* party = getUserdata<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	int index = 0;
	lua_createtable(L, party->getMemberCount(), 0);
	for (Player* player : party->getMembers()) {
		pushUserdata<Player>(L, player);
		setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int PartyFunctions::luaPartyGetMemberCount(lua_State* L) {
	// party:getMemberCount()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		lua_pushnumber(L, party->getMemberCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyGetInvitees(lua_State* L) {
	// party:getInvitees()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		lua_createtable(L, party->getInvitationCount(), 0);

		int index = 0;
		for (Player* player : party->getInvitees()) {
			pushUserdata<Player>(L, player);
			setMetatable(L, -1, "Player");
			lua_rawseti(L, -2, ++index);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyGetInviteeCount(lua_State* L) {
	// party:getInviteeCount()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		lua_pushnumber(L, party->getInvitationCount());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyAddInvite(lua_State* L) {
	// party:addInvite(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->invitePlayer(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyRemoveInvite(lua_State* L) {
	// party:removeInvite(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->removeInvite(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyAddMember(lua_State* L) {
	// party:addMember(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->joinParty(*player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyRemoveMember(lua_State* L) {
	// party:removeMember(player)
	Player* player = getPlayer(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party && player) {
		pushBoolean(L, party->leaveParty(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyIsSharedExperienceActive(lua_State* L) {
	// party:isSharedExperienceActive()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		pushBoolean(L, party->isSharedExperienceActive());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyIsSharedExperienceEnabled(lua_State* L) {
	// party:isSharedExperienceEnabled()
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		pushBoolean(L, party->isSharedExperienceEnabled());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartyShareExperience(lua_State* L) {
	// party:shareExperience(experience)
	uint64_t experience = getNumber<uint64_t>(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		party->shareExperience(experience);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int PartyFunctions::luaPartySetSharedExperience(lua_State* L) {
	// party:setSharedExperience(active)
	bool active = getBoolean(L, 2);
	Party* party = getUserdata<Party>(L, 1);
	if (party) {
		pushBoolean(L, party->setSharedExperience(party->getLeader(), active));
	} else {
		lua_pushnil(L);
	}
	return 1;
}
