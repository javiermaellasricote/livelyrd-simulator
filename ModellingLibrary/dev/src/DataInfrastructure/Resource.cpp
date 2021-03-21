#include "Resource.h"

livelyRD::Resource::Resource
(
        std::string name,
        std::string owner,
        double initialQuantity,
        double costToObtain,
        double reservationValueToSell,
        double reservationValueToBuy
) : AbstractDataElement(name, owner)
{
    quantity_.value = initialQuantity;
    quantity_.isPublic = false;

    costToObtain_.value = costToObtain;
    costToObtain_.isPublic = false;

    reservationValueToSell_.value = reservationValueToSell;
    reservationValueToSell_.isPublic = false;

    reservationValueToBuy_.value = reservationValueToBuy;
    reservationValueToBuy_.isPublic = false;
}

double livelyRD::Resource::getQuantity(const std::string &playerRequestingInfo)
{
    if (quantity_.isPublic)
        return quantity_.value;
    else if (playerRequestingInfo == owner_.value)
        return quantity_.value;
    else
        return -1.0;
}

bool livelyRD::Resource::setQuantityPublic(const std::string& playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        quantity_.isPublic = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Resource::setQuantityPrivate(const std::string& playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        quantity_.isPublic = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Resource::add(const double &amountToAdd, const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        quantity_.value += amountToAdd;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Resource::subtract(const double &amountToSubtract, const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        quantity_.value -= amountToSubtract;
        return true;
    }
    else
    {
        return false;
    }
}

double livelyRD::Resource::getCostToObtain(const std::string &playerRequestingInfo)
{
    if (costToObtain_.isPublic)
        return costToObtain_.value;
    else if (playerRequestingInfo == owner_.value)
        return costToObtain_.value;
    else
        return -1.0;
}

bool livelyRD::Resource::setCostToObtainPublic(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        costToObtain_.isPublic = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Resource::setCostToObtainPrivate(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        costToObtain_.isPublic = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::Resource::changeCostToObtain(const double &newCost, const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        costToObtain_.value = newCost;
        return true;
    }
    else
    {
        return false;
    }
}

double livelyRD::Resource::getReservationValueToSell(const std::string &playerRequestingInfo)
{
    if (playerRequestingInfo == owner_.value)
    {
        return reservationValueToSell_.value;
    }
    else
    {
        return -1.0;
    }
}

bool livelyRD::Resource::changeReservationValueToSell(const double &newValue, const std::string &playerRequestingInfo)
{
    if (playerRequestingInfo == owner_.value)
    {
        reservationValueToSell_.value = newValue;
        return true;
    }
    else
    {
        return false;
    }
}

double livelyRD::Resource::getReservationValueToBuy(const std::string &playerRequestingInfo)
{
    if (playerRequestingInfo == owner_.value)
    {
        return reservationValueToBuy_.value;
    }
    else
    {
        return -1.0;
    }
}

bool livelyRD::Resource::changeReservationValueToBuy(const double &newValue, const std::string &playerRequestingInfo)
{
    if (playerRequestingInfo == owner_.value)
    {
        reservationValueToBuy_.value = newValue;
        return true;
    }
    else
    {
        return false;
    }
}

void livelyRD::Resource::setQuantity(const double &newValue)
{
    quantity_.value = newValue;
}
