//
// Copyright (C) 2014 Florian Meier
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/physicallayer/ieee802154/packetlevel/Ieee802154NarrowbandDimensionalReceiver.h"
#include "inet/physicallayer/ieee802154/packetlevel/Ieee802154DimensionalTransmission.h"

namespace inet {

namespace physicallayer {

Define_Module(Ieee802154NarrowbandDimensionalReceiver);

Ieee802154NarrowbandDimensionalReceiver::Ieee802154NarrowbandDimensionalReceiver() :
    FlatReceiverBase()
{
}

void Ieee802154NarrowbandDimensionalReceiver::initialize(int stage)
{
    FlatReceiverBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        minInterferencePower = mW(math::dBm2mW(par("minInterferencePower")));
    }
}

std::ostream& Ieee802154NarrowbandDimensionalReceiver::printToStream(std::ostream& stream, int level) const
{
    stream << "Ieee802154NarrowbandDimensionalReceiver";
    return FlatReceiverBase::printToStream(stream, level);
}

bool Ieee802154NarrowbandDimensionalReceiver::computeIsReceptionPossible(const IListening *listening, const ITransmission *transmission) const
{
    auto ieee802154Transmission = dynamic_cast<const Ieee802154DimensionalTransmission *>(transmission);
    return ieee802154Transmission && NarrowbandReceiverBase::computeIsReceptionPossible(listening, transmission);
}

bool Ieee802154NarrowbandDimensionalReceiver::computeIsReceptionPossible(const IListening *listening, const IReception *reception, IRadioSignal::SignalPart part) const
{
    auto ieee802154Transmission = dynamic_cast<const Ieee802154DimensionalTransmission *>(reception->getTransmission());
    return ieee802154Transmission && NarrowbandReceiverBase::computeIsReceptionPossible(listening, reception, part);
}

} // namespace physicallayer

} // namespace inet
