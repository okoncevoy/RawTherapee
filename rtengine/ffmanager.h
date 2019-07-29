/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <cmath>
#include <map>
#include <string>

#include <glibmm/ustring.h>

#include "rawimage.h"

namespace rtengine
{

class ffInfo
{
public:

    Glib::ustring pathname; // filename of flat field
    std::list<Glib::ustring> pathNames; // other similar dark frames, used for average
    std::string maker;  ///< manufacturer
    std::string model;  ///< model
    std::string lens;   ///< lens
    double aperture;    ///< aperture in stops
    double focallength; ///< focal length in mm
    time_t timestamp;   ///< seconds since 1 Jan 1970


    ffInfo(const Glib::ustring &name, const std::string &mak, const std::string &mod, const std::string & len, double focal, double apert, time_t t)
        : pathname(name), maker(mak), model(mod), lens(len), aperture(apert), focallength(focal), timestamp(t), ri(nullptr) {}

    ffInfo( const ffInfo &o)
        : pathname(o.pathname), maker(o.maker), model(o.model), lens(o.lens), aperture(o.aperture), focallength(o.focallength), timestamp(o.timestamp), ri(nullptr) {}
    ~ffInfo()
    {
        if( ri ) {
            delete ri;
        }
    }


    ffInfo &operator =(const ffInfo &o);
    bool operator <(const ffInfo &e2) const;

    // Calculate virtual distance between two shots; different model return infinite
    double distance(const std::string &mak, const std::string &mod, const std::string &lens, double focallength, double aperture) const;

    static std::string key(const std::string &mak, const std::string &mod, const std::string &len, double focal, double apert );
    std::string key()
    {
        return key( maker, model, lens, focallength, aperture);
    }

    RawImage *getRawImage();

protected:
    RawImage *ri; ///< Flat Field raw data

    void updateRawImage();
};

class FFManager
{
public:
    using ffStat_t = std::pair<int, int>;

    static FFManager &getInstance();
    void init( const Glib::ustring &pathname );
    Glib::ustring getPathname() const;
    ffStat_t getStat() const;
    RawImage *searchFlatField( const std::string &mak, const std::string &mod, const std::string &len, double focallength, double apert, time_t t );
    RawImage *searchFlatField( const Glib::ustring filename );

private:
    FFManager() = default;
    ffInfo *addFileInfo(const Glib::ustring &filename, bool pool = true );
    ffInfo *find( const std::string &mak, const std::string &mod, const std::string &len, double focal, double apert, time_t t );

    typedef std::multimap<std::string, ffInfo> ffList_t;
    ffList_t ffList;
    bool initialized;
    Glib::ustring currentPath;
};

#define ffm FFManager::getInstance()

}
