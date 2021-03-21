#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include "AbstractDataElement.h"

namespace livelyRD
{
    class Resource : public AbstractDataElement
    {
        template <class T>
        struct ResourceVariable
        {
            T value;
            bool isPublic;
        };

    public:
        Resource
        (
            std::string name,
            std::string owner,
            double initialQuantity = 0.0,
            double costToObtain = 0.0,
            double reservationValueToSell = 0.0,
            double reservationValueToBuy = 0.0
        );
        ~Resource() override = default;

        double getQuantity(const std::string& playerRequestingInfo);
        void setQuantity(const double& newValue);
        bool setQuantityPublic(const std::string& playerRequestingAction);
        bool setQuantityPrivate(const std::string& playerRequestingAction);
        bool add(const double& amountToAdd, const std::string& playerRequestingAction);
        bool subtract(const double& amountToSubtract, const std::string& playerRequestingAction);

        double getCostToObtain(const std::string& playerRequestingInfo);
        bool setCostToObtainPublic(const std::string& playerRequestingAction);
        bool setCostToObtainPrivate(const std::string& playerRequestingAction);
        bool changeCostToObtain(const double& newCost, const std::string& playerRequestingAction);

        double getReservationValueToSell(const std::string& playerRequestingInfo);
        bool changeReservationValueToSell(const double& newValue, const std::string& playerRequestingInfo);

        double getReservationValueToBuy(const std::string& playerRequestingInfo);
        bool changeReservationValueToBuy(const double& newValue, const std::string& playerRequestingInfo);

    private:
        DataVariable<double> quantity_;
        DataVariable<double> costToObtain_;
        DataVariable<double> reservationValueToSell_;
        DataVariable<double> reservationValueToBuy_;
    };
}

#endif