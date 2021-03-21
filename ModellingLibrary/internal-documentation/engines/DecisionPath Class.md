# DecisionPath Class
#livelyRD/code/Engine

## Data
* Position of discretized-path points
* Optimum 

## Actions


## Algorithm
[based on Dijkstra’s algorithm] 

1. While ( totalEffort > AllOpenNodeEfforts ) 
	1. While( ! optimal.isInReach() )
		1. Generate new nodes in reach
		2. Connect them to active point
		3. Evaluate total “effort” of new points
		4. Set as active point the node with the least “effort” from all the “open nodes” (nodes without a branch coming out of them)  [What happens if two open nodes have the same total effort]
	2. Connect active node to optimal
	3. Calculate total effort 
	4. Check effort is lower than effort from open nodes 
- - - -
- [ ] How do we evaluate effort?
	- [ ] What does effort mean exactly?
- [ ] What does it mean to be “in reach” within a time-step?
	- [ ] How will a time-step be defined?
- [ ] How doe we define a node?

Effort: resource expenditure