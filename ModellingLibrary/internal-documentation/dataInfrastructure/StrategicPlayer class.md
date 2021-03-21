# StrategicPlayer class 
#livelyRD/code/DataInfrastructure

## Data 
* List of available resources (hash map of pointers to Resource objects)
* List of objectives (goals to achieve) (hash map of pointers to Objective objects)
* List of available “actions” or “moves” by the player  
* Next move decision (list of actions that the player wants to take in their next move)

## Actions 
* Decide next move (calls decision-whatever-solver class)
* Execute next move

* getPublicData()
* getPrivateData()

### Private actions 
* Implementation of all moves

- - - -
- - - -
## About the implementation of “list of possible actions”
* Each 