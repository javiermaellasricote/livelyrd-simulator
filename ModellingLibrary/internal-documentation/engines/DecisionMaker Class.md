# DecisionMaker Class
#livelyRD/code/Engine

- - - -
Would it make more sense to just create a path class??

* Called from StrategicPlayer class.
* Takes an StrategicPlayer.
* Modifies it to update its “Next move decision.”

- - - -

## Data
* Current position
* Optimal
* Path
* New decision


## Actions  
1. Call HEB Optimizer 
2. Optimize path 
3. Set new decision

## Algorithm 
* HEB optimization to find absolute optimal (DYCORS algorithm)
* Inside of path class:
	* Discretization of path
	* Shape optimization of path (look at previous project from MRes, FEMorph 2.0)