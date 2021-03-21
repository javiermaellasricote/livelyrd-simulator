Negotiating the price of a table 

## To-Do:

- [ ] How to dinamically change the "reduction ratio" (the "2/3")
- [ ] How to dinamically change the approval condition (instead of 2 good proposals, 1, 3, 4, ...)
- [ ] We will need to store the number of good proposals 
- [ ] How to change rejecting condition
- [ ] What if we are dealing with multiple resources being exchanged at the same time ??
- [ ] Model the negotiator's aggressiveness and patience??ß



### Possible messages:

* Request exchange at specific price.
* Approve exchange at requested price.
* Amend price of requested exchange.
* Reject exchange 



### Notes:

* If we try to optimize the exchange price, both players will start proposing the most extreme value allowed. 



### Rules: 

(from the view of the seller. They are the same for the buyer)

1. If transaction is wanted, the player must at least try to request an exchange or amend the price 
2. The first value proposed should be the ideal value for the player.
3. The amended value will be
   1. **2/3** far away from the RV if the proposed price is under the RV. [The 2/3 ratio could be changed to indicate the necessity to get a deal approved sooner]
   2. **2/3** from the proposed price if the price is over the RV. [The 2/3 ratio could be changed to indicate the necessecity to get a deal approved sooner]
4. A rule will be rejected if the 5 * (currentProposalByCounterPart - previousProposalByCounterPart) < RV
5. The approved exchange will be the one from the first candidate to offer two consecutive prices inside the ZOPA as long as that price is the highest of all the offers received from all the players.
6. The RV will change in each iteration to reflect the offers made by other players. 
7. Re-evaluate fraction depending on the number of players competing for your transaction.
8. The discount ratio will be calculated as the (numberOfOffers - 1) / numberOfOffers



### Diagrams:

 [NegotiationDiagram.pdf](files/NegotiationDiagram.pdf) 



## Types of Multilateral Actions in JSON:

* Negotiable multilateral actions.

  ```MongoDB
  'name': 'actionX',
  'owner': 'playerX',
  'type': 'negotiable',
  'environment': 'world',
  'lowerBound': -5.0,
  'upperBound': 5.0,
  'calculator': {
      'forks': 'forks + getMoreForks_value',
      'spoons': 'spoons - getMoreForks_value',
  },
  'recipients': {
  		'recipient1': {
  				'name': 'playerY',
  				'recipient action': 'actionY',
  		},
  		'recipient2': {
  				'name': 'playerZ',
  				'recipient action': 'actionZ',
  		},
  }
  ```

* Non-negotiable multilateral actions.

  ```MongoDB
  'name': 'actionX',
  'owner': 'playerX',
  'type': 'non-negotiable',
  'environment': 'world',
  'lowerBound': -5.0,
  'upperBound': 5.0,
  'calculator': {
      'forks': 'forks + getMoreForks_value',
      'spoons': 'spoons - getMoreForks_value',
  },
  'recipients': {
  		'recipient1': {
  				'name': 'playerY',
  				'recipient action': 'actionY',
  		},
  		'recipient2': {
  				'name': 'playerZ',
  				'recipient action': 'actionZ',
  		},
  }
  ```

* Bidiractional Compulsory multilateral actions. The recipient can:

  * Accept
  * Request

* Unidirectional Compulsory multilateral actions. The recipient can:

  * Accept

**NOTES:** We need to be able to simulate logrolling 



## Externally stored information 

* fraction
* reservationValue 



### On calculating the Reservation Value (RV)

The Reservation Value is the minimum amount that you are willing to take for you to give some of your resources. Any value below that will not be acceptable as a negotiated outcome since your resources could be better used another way.

**Your Reservation Value is esentially your best outcome if the negotiations fail.**

* In the case that we have multiple players interested in our transaction:
  * Our RV will be the one the best conditions for us 
* In case that we can use one resource for multiple actions 
  * The optimizer should be the one deciding how much to spend in each transaction
  * But the optimizer gives you the ideal value, not the RV

