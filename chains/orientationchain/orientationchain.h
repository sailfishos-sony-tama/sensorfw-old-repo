/**
   @file orientationchain.h
   @brief OrientationChain

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Üstün Ergenoglu <ext-ustun.ergenoglu@nokia.com>

   This file is part of Sensord.

   Sensord is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Sensord is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Sensord.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */

#ifndef ORIENTATIONCHAIN_H
#define ORIENTATIONCHAIN_H

#include "abstractsensor.h"
#include "abstractchain.h"
#include "deviceadaptor.h"
#include "sensord/bufferreader.h"
#include "sensord/filter.h"
#include "sensord/bin.h"
#include "orientationdata.h"
#include "posedata.h"
#include "datatypes/unsigned.h"

class Bin;
template <class TYPE> class BufferReader;
class FilterBase;

/**
 * @brief Orientationchain providies device orientation information
 * using the accelerometer information.
 *
 * <b>Output buffers:</b>
 * <ul><li>\em device orientation</li></ul>
 */
class OrientationChain : public AbstractChain
{
    Q_OBJECT;

    Q_PROPERTY(TimedUnsigned orientation READ orientation);

public:
    /**
     * Factory method for OrientationChain.
     * @return Pointer to new OrientationChain instance as AbstractChain*
     */
    static AbstractChain* factoryMethod(const QString& id)
    {
        OrientationChain* sc = new OrientationChain(id);
        return sc;
    }

    /**
     * Property method returning current orientation.
     * @return Current orientation.
     */
    TimedUnsigned orientation() const
    {
        PoseData pose = qvariant_cast< PoseData >(((QObject*)orientationInterpreterFilter_)->property("orientation"));
        return TimedUnsigned(pose.timestamp_, pose.orientation_);
    }


public Q_SLOTS:
    bool start();
    bool stop();

protected:
    OrientationChain(const QString& id);
    ~OrientationChain();

private:
    static double                    aconv_[3][3];
    Bin*                             filterBin_;

    AbstractChain*                   accelerometerChain_;
    BufferReader<AccelerationData>*  accelerometerReader_;
    FilterBase*                      topEdgeInterpreterFilter_;
    FilterBase*                      faceInterpreterFilter_;
    FilterBase*                      orientationInterpreterFilter_;
    RingBuffer<PoseData>*            topEdgeOutput_;
    RingBuffer<PoseData>*            faceOutput_;
    RingBuffer<PoseData>*            orientationOutput_;
};

#endif // ORIENTATIONCHAIN_H