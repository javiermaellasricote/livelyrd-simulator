# Core business plan
#livelyRD/business

## TODO:
- [x] Determine real-language to use for each term (following DDD)
- [x] Create BDD tests
	- [x] Make catch2 work
	- [x] Gather information about markets for TestCases
	- [x] Divide information into MarketEnvironment, KeyPlayers and OtherPlayers  
- [ ] Design Data Infrastructure
	- [x]  Decide how to represent possible actions for each player
		- [x] How to represent non-monetary conditions in negotiated agreements
		- [x] Create reactive-programming system for multilateral actions messages
			- [x] messagesToSend and receivedMessages
			- [ ] 
			- [x] ??? define private public 
- [x] Design Engine
	- [x] Design DecisionMaker class 
		- [x] Best way to discretize the path (Shortest path in graphs)
	- [ ] How to get the results??

- [ ] Implement data infrastructure 
	- [ ] Create BDD tests
		- [x] For Resources
			- [x] Change Visibility enum to Boolean 
			- [x] Create DataInfrastructure folder in code 
		- [x] For Objectives
		- [x] For Actions
			- [x] Unilateral Actions
			- [x] Multilateral actions 
		- [ ] For Players
			- [ ] Perform and decide actions
			- [ ] Solve error with getAction
		- [ ] For environment 
			- [ ] Simulate environment
	- [ ] Implement classes
		- [x] Resources
		- [x] Objectives
		- [ ] Actions
		- [x] Players
		- [ ] Environment
- - - -
- - - -
- [ ] Use Agile board for all of this
- [ ] Create a Demo
	- [x] Use Plotly
	- [ ] Add callbacks
	- [ ] Write demo backend algorithm 
	- [ ] Host website on server
	- [ ] Make app adjust to screen size

- - - -
- - - -

## Notes:
Stability analysis should be one of our main products

Cooperation, non-cooperation, competition,  and non-competition: models, options, and strategies 


Scenario analysis, strategic planning, scenario planning


- - - -
- - - -
## New notes:
The software should decide who to cooperate with, who to compete, and who to avoid competition with. 

- - - -
- - - -
## Ideas for the future 
* Fast response software (leaving all the models built so, by changing a few parameters you can model outcomes fast. Making you able to respond to real life events in time) 