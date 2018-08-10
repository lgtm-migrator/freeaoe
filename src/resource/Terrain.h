/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Resource.h"
#include <SFML/Graphics/Texture.hpp>
#include <genie/dat/Terrain.h>
#include <global/Logger.h>

#include <genie/resource/SlpFile.h>

#include <unordered_map>

class SlpFile;

class Terrain;
typedef std::shared_ptr<Terrain> TerrainPtr;

struct Slope {
    enum Direction {
        Flat = 0,
        SouthUp = 1,
        NorthUp     = 2,
        WestUp      = 3,
        EastUp      = 4,
        SouthWestUp = 5,
        NorthWestUp = 6,
        SouthEastUp = 7,
        NorthEastUp = 8,

        NorthDown   = 13,
        SouthDown   = 14,
        WestDown    = 15,
        EastDown    = 16,

        SouthWestEastUp = 17,
        NorthWestEastUp = 18,

        NorthSouthEastUp = 19,
        NorthSouthWestUp = 20,
    };

    Slope (const Direction s) : direction(s) {}
    Slope &operator =(const Direction s) { direction = s; return *this; }
    Slope &operator =(const uint8_t s) { direction = Direction(s); return *this; }

    inline bool operator <(const Direction other) const { return direction < other; }
    inline bool operator==(const Direction other) const { return other == direction; }
    inline bool operator <(const Slope other) const { return direction < other.direction; }
    inline bool operator==(const Slope other) const { return other.direction == direction; }

    inline operator uint8_t() const { return uint8_t(direction); }

    inline genie::Slope toGenie() const {
        const uint8_t s(direction);
        if (s < SouthWestEastUp) {
            return genie::Slope(s);
        }

        switch (s) {
        case Slope::SouthWestEastUp:
            return genie::SlopeSouthWestEastUp;
        case Slope::NorthWestEastUp:
            return genie::SlopeNorthWestEastUp;
        case Slope::NorthSouthEastUp:
            return genie::SlopeWestDown;
        case Slope::NorthSouthWestUp:
            return genie::SlopeEastDown;
        default:
            return genie::SlopeInvalid;
        }
    }

    Direction direction = Flat;
};

struct TileSlopes
{
    Slope self = Slope::Flat;

    Slope north = Slope::Flat;
    Slope south = Slope::Flat;

    Slope west = Slope::Flat;
    Slope east = Slope::Flat;

    Slope southWest = Slope::Flat;
    Slope southEast = Slope::Flat;

    Slope northWest = Slope::Flat;
    Slope northEast = Slope::Flat;

    bool operator==(const TileSlopes &other) const {
        return self == other.self &&
               north == other.north &&
               south == other.south &&
               west == other.west &&
               east == other.east &&
               southWest == other.southWest &&
               southEast == other.southEast &&
               northWest == other.northWest &&
               northEast == other.northEast;
    }
};

struct Blend  {
    enum BlendTile {
        LowerLeft1 = 0,
        UpperLeft1 = 4,
        LowerRight1 = 8,
        UpperRight1 = 12,

        Right = 16,
        Down = 17,
        Up = 18,
        Left = 19,

        UpperRightAndLowerLeft = 20,
        UpperLeftAndLowerRight = 21,

        OnlyRight = 22,
        OnlyDown = 23,
        OnlyUp = 24,
        OnlyLeft = 25,

        KeepUpperLeft = 26,
        KeepUpperRight = 27,
        KeepLowerRight = 28,
        KeepLowerLeft = 29,

        All = 30,

        BlendTileCount
    };

    inline void addBlend(const BlendTile blend)
    {
        bits |= 1 << blend;
    }

    inline bool operator ==(const Blend other) const { return bits == other.bits && x == other.x && y == other.y; }

    uint32_t bits = 0;
    uint8_t blendMode = 0;
    int x = 0;
    int y = 0;

    uint32_t terrainId;
};

namespace std {

template<> struct hash<TileSlopes>
{
    size_t operator()(const TileSlopes s) const {
        return hash<int8_t>()(s.self.direction) ^
               hash<int8_t>()(s.north.direction) ^
               hash<int8_t>()(s.south.direction) ^
               hash<int8_t>()(s.west.direction) ^
               hash<int8_t>()(s.east.direction) ^
               hash<int8_t>()(s.southWest.direction) ^
               hash<int8_t>()(s.southEast.direction) ^
               hash<int8_t>()(s.northWest.direction) ^
               hash<int8_t>()(s.northEast.direction);
    }
};

template<> struct hash<Blend>
{
    size_t operator()(const Blend b) const {
        return hash<uint32_t>()(b.bits) ^ hash<uint8_t>()(b.blendMode);
    }
};
}

class Terrain : public Resource
{
public:
    //----------------------------------------------------------------------------
    /// @param Id resource id
    //
    Terrain(unsigned int Id);
    virtual ~Terrain();

    const sf::Texture &texture(int x, int y);
    const sf::Image image(int x, int y);

    virtual bool load();

    const genie::Terrain &data();

    static uint8_t blendMode(const uint8_t ownMode, const uint8_t neighborMode);
    const sf::Texture &blendImage(const Blend blends, int tileX, int tileY);

    const sf::Texture &slopedImage(const TileSlopes &slopes, int tileX, int tileY);

private:
    void addOutline(sf::Image &img);

    genie::Terrain m_data;
    genie::SlpFilePtr m_slp;

    std::unordered_map<int, sf::Texture> m_images; // TODO Frames?
    std::unordered_map<Blend, sf::Texture> m_blendImages;
    std::unordered_map<TileSlopes, sf::Texture> m_slopeImages;
};

