# Resource Class
#livelyRD/code/DataInfrastructure

- - - -
Create enumerator Visibility: PRIVATE_VAR, PUBLIC_VAR 
Each element in Resource class is a struct, with its value and its Visibility condition.
- - - -

## Data
* Name [DEFAULT: Visible to all players]
* Owner [DEFAULT: Visible to all players]
* Quantity [DEFAULT: Only visible to Owner]
* Cost of obtaining it [DEFAULT: Only visible to owner]
* Reservation value of owner to sell it [DEFAULT: Only visible to Owner]
* Reservation value to buy more [DEFAULT: Only visible to Owner]
* [Not necessary] Is it a product (first created, then sold, then consumed) or a service (created, sold and consumed at the same time) [DEFAULT: Visible to all players]

## Actions
* Sell it (change owner in exchange for other resources)
* [Action by player, move to player] Consume it (make it disappear in exchange for benefits)
* Buy/build more of it (change owner in exchange for resources)
* Update cost of obtaining it 
* Update reservation value