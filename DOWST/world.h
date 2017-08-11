/*
 * Copyright (c) 2017, Kurt Cancemi (kurt@x64architecture.com)
 *
 * This file is part of DoW Soulstorm Trainer (DOWST).
 *
 *  DOWST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3
 *  as published by the Free Software Foundation.
 *
 *  DOWST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DOWST.  If not, see <http://www.gnu.org/licenses/>.
 */

namespace dowss
{
class EntityFactory;
class Player;
class World;
class SigmaWorldImp;

namespace gamefuncs
{
    using GetPlayerAtFn = Player*(__thiscall*)(World*, unsigned int);
    extern GetPlayerAtFn GetPlayerAt;
    constexpr const char kGetPlayerAtFnName[] = "?GetPlayerAt@World@@QBEPBVPlayer@@I@Z";

    using SetGameOverFn = void(__thiscall*)(World*);
    extern SetGameOverFn SetGameOver;
    constexpr const char kSetGameOverFnName[] = "?SetGameOver@World@@QAEXXZ";
}

class World
{
public:
    unsigned int GetPlayerCount() { return (_playerArrayEnd - _playerArrayStart) / 4; }
    float GetNumOfSimsPerSecond() { return _simsPerSecond; }
    int GetGameTicks() { return _gameTicks; }
    bool IsGameOver() { return _gameOver; }
    unsigned int GetRandomSeed() { return _randomSeed; }
    void SetGameOver() { gamefuncs::SetGameOver(this); }
    Player* GetPlayerAt(unsigned int idx) { return gamefuncs::GetPlayerAt(this, idx); }

private:
    uint8_t __pad_0000[4];         // 0x0000
    SigmaWorldImp* _imp;           // 0x0004
    float _simsPerSecond;          // 0x0008
    int32_t _gameTicks;            // 0x000C
    bool _gameOver;                // 0x0010
    uint8_t __pad_0011[3];         // 0x0011
    uint32_t _randomSeed;          // 0x0014
    uint8_t __pad_0018[4];         // 0x0018
    EntityFactory* _entityFactory; // 0x001C
    uint8_t __pad_0020[8];         // 0x0020
    uintptr_t _playerArrayStart;   // 0x0028
    uintptr_t _playerArrayEnd;     // 0x002C
};
}