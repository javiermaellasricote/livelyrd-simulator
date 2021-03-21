# Action Class
#livelyRD/code/DataInfrastructure

## Data
* Name
* Owner
* isToBePerformed
* function representing the action
* Function representing decision if is to be performed. 
* isActionBlocked, actionRestrictions

## Actions
* decideIfIsToBePerformed()
> All the requirements necessary for the action to be performed will be checked here.   

* performAction()
* resetIsToBePerformed()


- - - -
- - - -
## About Actions:

### multilateral actions (cooperation): 
Before an action is taken, there is a chain of decisions and exchange of information between the players involved. 

For example, for the case when an item is being sold:
1. The seller expresses interest to sell the item and waits until somebody responds
2. The buyer expresses interest for the item and expresses the amount willing to pay
3. The seller accepts or rejects the offer. In case of acceptance, the action of selling is performed. In case of rejection, the seller expresses the price that they are willing to pay or the seller walks away from the negotiation. 
4. In case of a new offer made by the seller, the buyer decides if accepts or rejects, continuing the cycle. 

Possible best approach: separate multilateral actions as a collection of unilateral actions taken by multiple players in a specific order.

**Non-monetary negotiating conditions:**

The software must be able to accommodate for exchange of non-monetary transactions, such us the establishment of certain rules or behaviours in other players by the negotiated agreement.

This means that certain agreement could block certain players from performing certain actions in certain situations.

This could be implemented as a “restrictions” vector of boolean conditions to be evaluated. The actions can only be performed if all the conditions on that “restrictions” vector give true.  New agreements can add or remove new restrictions to that vector. 

**Enforcement of negotiated agreements:**
Negotiated agreements can be loosely enforced or tightly enforced. A loosely enforced agreement depends only on the will of the players no to be broken. Tightly enforced agreements carry an actual penalty for players if they break the rules of the agreement. 

### Unilateral Actions:
Actions taken by a player independently of the state of any other player.  This actions must be carried out by the player alone. They could include things like setting the initial price for selling an item, changing the reservation values, refusing to cooperate with certain players, entering into direct competition with other players, or performing acts of aggression (the software will NOT accommodate or represent acts of aggression).

The steps are usually the following:
1. The player gathers information about its surroundings.
2. It analyses such information and comes out with the decision to take a unilateral action. 
3. The unilateral action is taken immediately.

Players with more decision-making power, or more independent from the state of others tend to have more ability to take significant unilateral actions. 

- - - -
- - - -
# Multilateral Actions
We need to create an observable and an event handler. The observer will be the Action class through the player. The environment should act as the event handler

## Data
* Name
* Owner
* isToBePerfomed
* isBeingDecided
* isBeingPerformed
* list of unilateral actions to be performed 

## Action 
* decideIfIsToBePerformed()
> All the requirements necessary for the action to be performed will be checked here.   

* receiveExternalResponse()

* performAction()
* resetIsToBePerformed()