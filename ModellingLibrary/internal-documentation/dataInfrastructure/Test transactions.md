# Test transactions



## Test structure

* Two player exchanging forks for spoons 
* They want for both to get an equal number of spoons and forks
* One player starts with only spoons, the other with only forks
* They can only exchange five objects per transaction



## Action logic

Each player will take the following considerations:

 [TransactionDiagram.pdf](../images/TransactionDiagram.pdf) 



### How to store old transaction

Possibilities:

* The information to perform the old transaction will be taken from the message approving such transaction. [We will use this one for simplicity]
  * ~~Potential risks of making mistakes while writing the code to create the approval message since this message could alter the values of the transaction.~~
* ~~Create yet another input parameter for the actionToBeTaken with the information needed.~~
* ~~Create a message to itself storing the information needed, then such information could be checked against the information of the approval message.~~

* ~~Create the request message with itself as a recipient (apart from the proper recipient). When the approval message is obtained, take the values from the request message.~~

**Note:** The player will send in the message what the player wants to achieve. Thus, the values must be inverted when being considered by the other player. 