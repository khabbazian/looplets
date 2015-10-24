
### Tools for understanding structures of complex networks. 
It decomposes the graph into loops of size three, four and so on. It is useful to investigate the structure of large graphs. The algorithm uses breath first search approach to count loops of size k where all the edges of the loop do not belong to a loop of size less than k. The algorithm iterates over k=3,4,... until there is no loop or it reaches the loop length upper bound.

### Install using the devtools package.
```
install.packages("devtools")
require(devtools)
install_github("khabbazian/looplets")
require(looplets)
```
Windows users will first need to install [Rtools](https://cran.r-project.org/bin/windows/Rtools/).

### TODO
I will implement and add more functions to reveal further information about the structure of complex networks (graphs) and also generate null models, i.e. a graph that has certain property(properties) but it is random in other aspects. 
