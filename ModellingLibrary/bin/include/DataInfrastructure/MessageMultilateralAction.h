#ifndef MESSAGESMULTILATERALACTION_H
#define MESSAGESMULTILATERALACTION_H

#include <string>
#include <vector>
#include <unordered_map>

namespace livelyRD
{
    struct MessageMultilateralAction
    {
        std::string sender;
        std::vector<std::string> recipients;
        std::string senderActionName;
        std::string recipientActionName;
        std::unordered_map<std::string, double> doublesContent;
        std::string messageContent;
    };
}
#endif
