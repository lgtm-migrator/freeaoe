/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <memory>
#include <unordered_map>
#include <fstream>
#include "Graphic.h"
#include "Terrain.h"
#include <global/Logger.h>
#include <global/Types.h>

#include <genie/resource/PalFile.h>
#include <genie/resource/SlpFile.h>
#include <genie/resource/BlendomaticFile.h>
#include <genie/script/ScnFile.h>

class ColorPalette;
class BinaFile;

namespace genie {
class DrsFile;
}

//------------------------------------------------------------------------------
/// The resource manager is the primary interface for getting recourses. At
//
class ResourceManager
{
public:
    enum UiResolution {
        Ui800x600 = 51100,
        Ui1024x768 = 51120,
        Ui1280x1024 = 51140
    };

    enum UiCiv {
        Briton = 1,
        Frank = 2,
        Goth = 3,
        Teuton = 4,
        Japanese = 5,
        Chinese = 6,
        Byzantine = 7,
        Persian = 8,
        Saracen = 9,
        Turkish = 10,
        Viking = 11,
        Mongol = 12,
        Celtic = 13,
        Spanish = 14,
        Aztec = 15,
        Mayan = 16,
        Hunnic = 17,
        Korean = 18,

    };
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;

    //----------------------------------------------------------------------------
    /// Initializes the resource manager once and returns its reference. On
    /// calling inst the first time the manager will load header information from
    /// drs and other files.
    //
    static ResourceManager *Inst();

    //----------------------------------------------------------------------------
    /// Returns the slp file with given id or 0 if not found. The slp file
    /// should not be deleted!
    ///
    /// @param id id of the slp file
    /// @return slp file
    //
    genie::SlpFilePtr getSlp(unsigned int id);

    genie::SlpFilePtr getUiOverlay(const UiResolution res, const UiCiv civ);

    genie::ScnFilePtr getScn(unsigned int id);

    //----------------------------------------------------------------------------
    /// Get a Graphic resource object.
    ///
    /// @param id id of the resource
    /// @return GraphicPtr pointing to the object
    //
    res::GraphicPtr getGraphic(unsigned int id);

    //----------------------------------------------------------------------------
    /// Get a Terrain resource object.
    ///
    /// @param id id of the resource
    /// @return resource pointer to the object
    //
    res::TerrainPtr getTerrain(unsigned int id);

    genie::PalFilePtr getPalette(unsigned int id = 50500);

    genie::BlendModePtr getBlendmode(unsigned int id = 0);

    //----------------------------------------------------------------------------
    /// Adds an slp file that will be managed by the ResourceManager.
    /// TODO: Maybe auto_ptr
    ///
    /// @param slp reference to the slp file
    //
    void addSlpFile(SlpFile *slp);

    void addBinaFile(BinaFile *bina);

    bool initialize(const std::string dataPath);

private:
    ResourceManager();
    virtual ~ResourceManager();

    typedef std::vector<std::shared_ptr<genie::DrsFile>> DrsFileVector;
    DrsFileVector drs_files_;

    genie::BlendomaticFilePtr blendomatic_file_;
    /*
  std::map<unsigned int, SlpFile *> slp_files_;
  std::map<unsigned int, BinaFile*> bina_files_;
  */
    //TODO: All resources into one map?
    typedef std::unordered_map<unsigned int, res::Graphic *> GraphicMap;
    GraphicMap graphics_;

    typedef std::unordered_map<unsigned int, res::TerrainPtr> TerrainMap;
    TerrainMap terrains_;

    /*
  std::fstream terrain_file_;
  std::fstream graphics_file_;
  */

    static Logger &log;

    //----------------------------------------------------------------------------
    /// Loads all resource headers.
    //

    void loadDrs(std::string file_name);
};

#endif // RESOURCEMANAGER_H
