#' counts number of loops of length 3, 4, ..., max.loop.length. 
#'
#' Counts number of loops of length 3, 4, ..., max.loop.length. The results are inclusive. That is if an edge belongs to a loop of size k it is not consider for loop of size k+1.
#'
#'
#'@param edge.list an edge list of an undirected graph. If the graph is directed, i.e. v->u exits but u->v doesn't then it may return the wrong result without warning
#'@param max.loop.length an upper bound for the loop lengths
#'
#'
#'@return the statistics of the loops including number of loops and number of edges in the counted loops. If edge e is a part of for instance two loops it is counted once.
#'
#'@export
graph_loops_stats <- function(edge.list, max.loop.length=10){
    loops.stats <- decompose_graph_into_loops(edge.list)
    return(loops.stats)
}

#' generates a random ER graph.
#'
#' generates a random ER graph. In ER model node v and u are connected with a fixed probability p.
#'
#'@param average.degree expected node degree. p = average.degree/num.nodes.
#'@param num.nodes number of nodes of the graph.
#'@param rseed a seed value for the random number generator.
#'
#'@return the edge list of the graph ensemble.
#'@export
ER_graph <- function(average.degree, num.nodes, rseed=1){
    edgeList  <-  generate_ER_graph(10,100)
    return(edgeList)
}
