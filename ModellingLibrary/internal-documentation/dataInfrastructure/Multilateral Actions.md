# Multilateral Actions
#livelyRD/code/DataInfrastructure

Inherits from Unilateral class

## Data:
* List of subscribers with information about them
* List of actions 
* actionToBeTaken
* decisionToBeTaken
* isToBePerformed

- - - -
As with Unilateral Actions, the steps for the use of a multilateral action are the following:

1. Initialization of the instance:
	1. Creation of deicionToBeTaken
	2. Creation of actionsToBeTaken
	3. Addition of all actions to be performed
2. If the action is selected to be performed, the decisionsToBeTaken function will be evaluated. If it returns true, the action will be performed.
3. When the action is performed, the actionToBeTaken will be called. This will call the correspondent action from the “List of actions.”
4. The specific unilateral action will be performed. A message will be sent by actionsToBePerformed to the observer (environment) with the name of the recipient, and the necessary information.
	- [ ] How can we set up the message?
		- [ ] Create action handler?
	- [ ] How to handle list of subscribers inside of actions. 

- - - -
TODO:
* MultilateralAction should no longer inherit from UnilateralAction
* actionsToBeTaken in MultilateralActions should output a MessageMultilateralActions vector object as a reference
* actionsToBeTaken should get the list of subscribers as a reference. 



- - - -
- - - -
The easiest example of a multilateral action is the price negotiation for a sale. 

The normal steps are the following:
1. The seller puts an object on sale (spreads the message that the object is now on sale)
2. The buyer expresses interest in the object being sold and proposes a price 
3. The seller accepts the offer, makes a counter offer, or rejects the offer.
4. Then, the buyer must accept the new offer, if there is a new one; reject it; or propose a counter offer.
5. And so on until the offer is rejected or accepted (and the item is sold).
- - - -
All of these steps can be considered unilateral actions, therefore, a multilateral action is just a handler of smaller unilateral actions taken in a certain order and taking into account the unilateral actions of other parties. 

In our example we would have to create two multilateral actions, one for the buyer and another one for the seller:

**Buying-side, multilateral action**
~List of actions:~
* Search for items on sale
* Decide if item is worth buying (and SUBSCRIBE TO LIST OF BUYERS)
* Propose price 
* Accept offer 
* Decline offer

**Selling-side, multilateral action**
~List of actions:~
* Set an object as “to be sold” (and OPEN LIST OF SUBSCRIBERS)
* Propose price 
* Accept offer 
* Decline offer



## Types of Multilateral Actions:

* Negotiable multilateral actions. The recipients can:
  * Accept
  * Request
  * Reject
  * Amend
* Non-negotiable multilateral actions. The recipient can:
  * Accept
  * Request
  * Reject
* Bidiractional Compulsory multilateral actions. The recipient can:
  * Accept
  * Request
* Unidirectional Compulsory multilateral actions. The recipient can:
  * Accept

**NOTES:** We need to be able to simulate logrolling 