#include "AbstractDataElement.h"

livelyRD::AbstractDataElement::AbstractDataElement(const std::string& name, const std::string& owner)
{
    name_.value = name;
    name_.isPublic = false;

    owner_.value = owner;
    owner_.isPublic = false;
}

livelyRD::AbstractDataElement::~AbstractDataElement() = default;

std::string livelyRD::AbstractDataElement::getName(const std::string &playerRequestingInfo) const
{
    if (name_.isPublic || playerRequestingInfo == owner_.value)
        return name_.value;
    else
        return "";
}

bool livelyRD::AbstractDataElement::setNamePublic(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        name_.isPublic = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool livelyRD::AbstractDataElement::setNamePrivate(const std::string &playerRequestingAction)
{
    if (playerRequestingAction == owner_.value)
    {
        name_.isPublic = false;
        return true;
    }
    else
    {
        return false;
    }
}

