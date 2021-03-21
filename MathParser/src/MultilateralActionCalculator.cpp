#include <regex>
#include <utility>
#include <cmath>
#include <map>

#include "MultilateralActionCalculator.h"
#include "DefinitionsMultilateralActionCalculator.h"
#include "MultilateralActionFormula.h"


livelyRD::MultilateralActionCalculator::MultilateralActionCalculator(
        std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>> roots,
        const std::unordered_map<std::string, double> &inputs, std::unordered_map<std::string, double> &outputs,
        const livelyRD::MultilateralActionFormula::MultilateralActionType &type,
        std::vector<std::string> potentialRecipients,
        std::unordered_map<std::string, std::vector<std::shared_ptr<MessageMultilateralAction>>> &messages,
        std::string playerName, std::string actionName, std::string recipientActionName):
        AbstractActionCalculator(std::move(roots), inputs, outputs),
        type_(type), potentialRecipients_(std::move(potentialRecipients)),
        messages_(messages), playerName_(std::move(playerName)),actionName_(std::move(actionName)),
        recipientActionName_(std::move(recipientActionName)), idealValue_(inputs_.find(actionName_ + "0")->second)
{

}

bool livelyRD::MultilateralActionCalculator::performOldTransaction(
        const std::shared_ptr<MessageMultilateralAction> &message)
{
    if(message->messageContent == MULTILATERAL_APPROVAL_MESSAGE)
    {
        auto inputsCopy = inputs_;
        inputsCopy[actionName_ + "0"] = message->doublesContent[message->sender];
        for(const auto& root : roots_)
        {
            root.second->value = outputs_[root.first];
            outputs_[root.first] = calculateAction(root.second, inputsCopy);
        }
        return true;
    }
    return false;
}

bool livelyRD::MultilateralActionCalculator::performCompulsoryTransaction(
        const std::shared_ptr<MessageMultilateralAction> &message)
{
    if(message->messageContent == MULTILATERAL_REQUEST_MESSAGE)
    {
        auto inputsCopy = inputs_;
        inputsCopy[actionName_ + "0"] = message->doublesContent[message->sender];
        for(const auto& root : roots_)
        {
            root.second->value = outputs_[root.first];
            outputs_[root.first] = calculateAction(root.second, inputsCopy);
        }
        sendApproval(message->sender, message->doublesContent[message->sender]);
        return true;
    }
    return false;
}

bool livelyRD::MultilateralActionCalculator::performTransaction(
        const std::shared_ptr<MessageMultilateralAction> &message)
{
    auto inputsCopy = inputs_;
    inputsCopy[actionName_ + "0"] = message->doublesContent[message->sender];
    for(const auto& root : roots_)
    {
        root.second->value = outputs_[root.first];
        outputs_[root.first] = calculateAction(root.second, inputsCopy);
    }
    sendApproval(message->sender, message->doublesContent[message->sender]);

    deleteNonApprovalMessages();

    for(const auto& rejectedMessage : messages_.find(playerName_)->second)
    {
        if(rejectedMessage != message)
        {
            sendRejection(rejectedMessage->sender);
        }
    }

    return true;
}

bool livelyRD::MultilateralActionCalculator::sendRequest()
{
    auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
    newMessage->sender = playerName_;
    newMessage->senderActionName = actionName_;
    newMessage->recipients = potentialRecipients_;
    newMessage->recipientActionName = recipientActionName_;
    newMessage->doublesContent[playerName_] = inputs_.find(actionName_ + std::to_string(0))->second;
    newMessage->messageContent = MULTILATERAL_REQUEST_MESSAGE;
    for(const auto& recipient : potentialRecipients_)
    {
        messages_[recipient].push_back(newMessage);
    }
    return true;
}

bool livelyRD::MultilateralActionCalculator::sendRejection(const std::string& recipientToReject)
{
    auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
    newMessage->sender = playerName_;
    newMessage->senderActionName = actionName_;
    newMessage->recipients.push_back(recipientToReject);
    newMessage->recipientActionName = recipientActionName_;
    newMessage->doublesContent[playerName_] = 0.0;
    newMessage->messageContent = MULTILATERAL_REJECTION_MESSAGE;
    messages_[recipientToReject].push_back(newMessage);
    return true;
}

bool livelyRD::MultilateralActionCalculator::sendAmendment(
        const std::string& recipientForAmendment, const double& amendmentValue,
        const std::shared_ptr<MessageMultilateralAction>& messageToReply)
{
    auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
    newMessage->sender = playerName_;
    newMessage->senderActionName = actionName_;
    newMessage->recipients.push_back(recipientForAmendment);
    newMessage->recipientActionName = recipientActionName_;
    newMessage->doublesContent[playerName_] = amendmentValue;
    newMessage->doublesContent[recipientForAmendment + MULTILATERAL_AMEND_PREVIOUS_VALUE] =
            messageToReply->doublesContent[recipientForAmendment];
    newMessage->messageContent = MULTILATERAL_AMEND_MESSAGE;
    messages_[recipientForAmendment].push_back(newMessage);
    return true;
}

bool livelyRD::MultilateralActionCalculator::sendApproval(const std::string &recipientToApprove,
                                                          const double &approvalValue)
{
    auto newMessage = std::make_shared<livelyRD::MessageMultilateralAction>();
    newMessage->sender = playerName_;
    newMessage->senderActionName = actionName_;
    newMessage->recipients.push_back(recipientToApprove);
    newMessage->recipientActionName = recipientActionName_;
    newMessage->doublesContent[playerName_] = approvalValue;
    newMessage->messageContent = MULTILATERAL_APPROVAL_MESSAGE;
    messages_[recipientToApprove].push_back(newMessage);
    return true;
}

bool livelyRD::MultilateralActionCalculator::calculateTimeStepParameters()
{
    if(messages_.find(playerName_)->second.empty())
    {
        return false;
    }
    std::map<double, double> differenceToIdealValue;
    for(const auto& message : messages_[playerName_])
    {
        if(message->recipientActionName == actionName_ &&
           (message->messageContent == MULTILATERAL_REQUEST_MESSAGE ||
           message->messageContent == MULTILATERAL_AMEND_MESSAGE))
        {
            differenceToIdealValue[std::abs(idealValue_ - message->doublesContent[message->sender])] =
                    message->doublesContent[message->sender];
        }
    }
    if(differenceToIdealValue.empty())
    {
        return false;
    }
    reservationValue_ = differenceToIdealValue.begin()->second;
    discountFraction_ = 1.0 / (differenceToIdealValue.size() + 2.0);
    return true;
}

bool livelyRD::MultilateralActionCalculator::proposeAmendment(
        const std::shared_ptr<MessageMultilateralAction> &message)
{
    double previousProposal = message->doublesContent[playerName_ + MULTILATERAL_AMEND_PREVIOUS_VALUE];
    double amendmentValue;
    if(message->messageContent == MULTILATERAL_REQUEST_MESSAGE)
    {
        amendmentValue = idealValue_;
    }
    else
    {
        double modificationValue = std::abs(previousProposal - reservationValue_) * discountFraction_;

        if(idealValue_ > message->doublesContent[message->sender])
        {
            amendmentValue = previousProposal - modificationValue;
        }
        else
        {
            amendmentValue = previousProposal + modificationValue;
        }
    }
    sendAmendment(message->sender, amendmentValue, message);
    return true;
}

bool livelyRD::MultilateralActionCalculator::isProposalAcceptable(
        const std::shared_ptr<MessageMultilateralAction> &message)
{
    if(message->messageContent == MULTILATERAL_REQUEST_MESSAGE)
    {
        return false;
    }
    else
    {
        double offerDifference = message->doublesContent[message->sender] -
                message->doublesContent[playerName_ + MULTILATERAL_AMEND_PREVIOUS_VALUE];
        return (std::abs(offerDifference) / std::abs(idealValue_ - reservationValue_)) < 0.2;
    }
}

bool livelyRD::MultilateralActionCalculator::isProposalBetterThanRV(
        const std::shared_ptr<MessageMultilateralAction>& message)
{
    std::map<double, double> valuesMap;
    valuesMap[std::abs(reservationValue_ - idealValue_)] = reservationValue_;
    valuesMap[std::abs(message->doublesContent[message->sender] - idealValue_)] =
            message->doublesContent[message->sender];
    return valuesMap.begin()->second == message->doublesContent[message->sender];
}

bool livelyRD::MultilateralActionCalculator::isNewTransactionWanted()
{
    return idealValue_ != 0.0;
}

void livelyRD::MultilateralActionCalculator::performAction()
{
    calculateTimeStepParameters();
    bool thereIsRequest = false;
    bool transactionApproved = false;
    for(const auto& message : messages_.find(playerName_)->second)
    {
        if(message->recipientActionName == actionName_)
        {
            bool oldTransactionPerformed = false;

            if(type_ != MultilateralActionFormula::RECEIVER_UNIDIRECTIONAL_COMPULSORY)
            {
                oldTransactionPerformed = performOldTransaction(message);
            }

            if(!oldTransactionPerformed && (message->messageContent == MULTILATERAL_REQUEST_MESSAGE ||
                    message->messageContent == MULTILATERAL_AMEND_MESSAGE))
            {
                thereIsRequest = true;
                if(type_ == MultilateralActionFormula::NEGOTIABLE || type_ == MultilateralActionFormula::NON_NEGOTIABLE)
                {
                    if(isProposalBetterThanRV(message) && !transactionApproved)
                    {
                        if(isProposalAcceptable(message) || type_ == MultilateralActionFormula::NON_NEGOTIABLE)
                        {
                            performTransaction(message);
                            transactionApproved = true;
                        }
                        else
                        {
                            proposeAmendment(message);
                        }
                    }
                    else if(type_ == MultilateralActionFormula::NEGOTIABLE && !transactionApproved)
                    {
                        proposeAmendment(message);
                    }
                    else
                    {
                        sendRejection(message->sender);
                    }
                }
                else if(type_ != MultilateralActionFormula::SENDER_UNIDIRECTIONAL_COMPULSORY)
                {
                    performCompulsoryTransaction(message);
                }
            }
        }
    }
    if(!thereIsRequest && (type_ != MultilateralActionFormula::RECEIVER_UNIDIRECTIONAL_COMPULSORY))
    {
        if(isNewTransactionWanted())
        {
            sendRequest();
        }
    }
    for(const auto& root : roots_)
    {
        if(outputs_.find(root.first) == outputs_.end())
        {
            outputs_[root.first] = inputs_.find(root.first)->second;
        }
    }
}

const std::unordered_map<std::string, std::vector<std::shared_ptr<livelyRD::MessageMultilateralAction>>> &
livelyRD::MultilateralActionCalculator::getMessages()
{
    return messages_;
}

bool livelyRD::MultilateralActionCalculator::deleteNonApprovalMessages()
{
    for(auto& messageColumn : messages_)
    {
        for(auto nonApprovalMessage = messageColumn.second.begin(); nonApprovalMessage != messageColumn.second.end();)
        {
            if(nonApprovalMessage->get()->senderActionName == actionName_ &&
               nonApprovalMessage->get()->messageContent != MULTILATERAL_APPROVAL_MESSAGE)
            {
                messageColumn.second.erase(nonApprovalMessage);
            }
            else
            {
                ++nonApprovalMessage;
            }
        }
    }
    return true;
}











