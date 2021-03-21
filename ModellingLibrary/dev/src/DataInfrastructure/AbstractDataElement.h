#ifndef ABSTRACTDATAELEMENT_H
#define ABSTRACTDATAELEMENT_H

#include <string>

namespace livelyRD
{
    class AbstractDataElement
    {
    public:
        template <class T>
        struct DataVariable
        {
            T value;
            bool isPublic;
        };

    public:
        AbstractDataElement(const std::string& name, const std::string& owner);
        virtual ~AbstractDataElement();

        [[nodiscard]] std::string getName(const std::string& playerRequestingInfo) const;
        bool setNamePublic(const std::string& playerRequestingAction);
        bool setNamePrivate(const std::string& playerRequestingAction);

    protected:
        DataVariable<std::string> name_;
        DataVariable<std::string> owner_;
    };
}

#endif