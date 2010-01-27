/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2008-2009 Pelican Ventures, Inc.
 * http://osgearth.org
 *
 * osgEarth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include <osgEarthFeatures/ScaleFilter>
#include <osgEarth/GeoData>

using namespace osgEarth;
using namespace osgEarth::Features;

ScaleFilter::ScaleFilter() :
_scale( 0.0 )
{
    //NOP
}

ScaleFilter::ScaleFilter( double scale ) :
_scale( scale )
{
    //NOP
}

FilterContext
ScaleFilter::push( FeatureList& input, const FilterContext& cx )
{
    for( FeatureList::iterator i = input.begin(); i != input.end(); ++i )
        push( i->get(), cx );

    return cx;
}

bool
ScaleFilter::push( Feature* input, const FilterContext& cx )
{
    if ( !input || !input->getGeometry() )
        return true;

    // first determine the feature's envelope.
    Bounds envelope = input->getGeometry()->getBounds();

    // now scale and shift everything
    GeometryIterator scale_iter( input->getGeometry() );
    while( scale_iter.hasMore() )
    {
        Geometry* geom = scale_iter.next();
        for( osg::Vec3dArray::iterator v = geom->begin(); v != geom->end(); v++ )
        {
            double xr = (v->x() - envelope.xMin()) / envelope.width();
            v->x() += (xr - 0.5) * _scale;

            double yr = (v->y() - envelope.yMin()) / envelope.height();
            v->y() += (yr - 0.5) * _scale;
        }
    }

    return true;
}