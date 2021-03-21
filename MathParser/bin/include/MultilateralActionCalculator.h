#ifndef MATHPARSER_MULTILATERALACTIONCALCULATOR_H
#define MATHPARSER_MULTILATERALACTIONCALCULATOR_H

#include <unordered_map>
#include <string>
#include <ModellingLibrary.h>
#include "AbstractMathFormula.h"
#include "AbstractActionCalculator.h"
#include "MultilateralActionFormula.h"

namespace livelyRD
{
    class MultilateralActionCalculator : public AbstractActionCalculator
    {
    public:
        MultilateralActionCalculator(
                std::unordered_map<std::string, std::shared_ptr<livelyRD::AbstractMathFormula::EquationElement>>
                roots,
                const std::unordered_map<std::string, double>& inputs,
                std::unordered_map<std::string, double>& outputs,
                const MultilateralActionFormula::MultilateralActionType& type,
                std::vector<std::string>  potentialRecipients,
                std::unordered_map<std::string,
                        std::vector<std::shared_ptr<MessageMultilateralAction>>>& messages,
                std::string  playerName, std::string  actionName, std::string  recipientActionName);
        ~MultilateralActionCalculator() override = default;

        void performAction() override;

        [[nodiscard]] const std::unordered_map<std::string,
                std::vector<std::shared_ptr<MessageMultilateralAction>>>& getMessages();

    private:

        bool performOldTransaction(const std::shared_ptr<MessageMultilateralAction> &message);
        bool performCompulsoryTransaction(const std::shared_ptr<MessageMultilateralAction> &message);
        bool performTransaction(const std::shared_ptr<MessageMultilateralAction> &message);
        bool proposeAmendment(const std::shared_ptr<MessageMultilateralAction>& message);
        bool isProposalAcceptable(const std::shared_ptr<MessageMultilateralAction>& message);
        bool isProposalBetterThanRV(const std::shared_ptr<MessageMultilateralAction>& message);
        bool isNewTransactionWanted();
        bool sendRequest();
        bool sendRejection(const std::string& recipientToReject);
        bool sendApproval(const std::string& recipientToApprove, const double& approvalValue);
        bool sendAmendment(const std::string& recipientForAmendment, const double& amendmentValue,
                const std::shared_ptr<MessageMultilateralAction>& messageToReply);
        bool calculateTimeStepParameters();
        bool deleteNonApprovalMessages();


        const MultilateralActionFormula::MultilateralActionType type_;
        const std::vector<std::string> potentialRecipients_;
        std::unordered_map<std::string,std::vector<std::shared_ptr<MessageMultilateralAction>>> messages_;
        const std::string playerName_;
        const std::string actionName_;
        const std::string recipientActionName_;
        double discountFraction_{};
        double reservationValue_{};
        double idealValue_;
    };
}

#endif
