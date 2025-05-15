# City Road Network Analyzer

A C++ application developed as a university assignment that analyzes city administrative structures and road networks using graph algorithms and tree traversals.

## Project Description

This application was developed as part of a Data Structures and Algorithms course to demonstrate:
- Graph traversal techniques
- Shortest-path calculations
- Tree data structure implementations
- C++ programming practices

The program models city infrastructure with:
1. A hierarchical district system (tree structure)
2. A road network between cities (weighted graph)

## Features

### District Hierarchy Analysis
- **DFS Traversal**: Depth-first search through city districts
- **BFS Traversal**: Breadth-first search through city districts
- **Administrative Identification**: Finds all uppercase-named districts

### Road Network Analysis
- **Connectivity Check**: Finds all reachable cities from a starting point
- **Shortest Path Calculation**: Uses Dijkstra's algorithm to find optimal routes
- **Two-way Connections**: Models bidirectional road connections with distances

## How It Works

1. **District Analysis**:
    - Builds a tree representing city administrative structure
    - Traverses using both DFS and BFS to identify important districts

2. **Road Network Analysis**:
    - Constructs a weighted graph of cities and connecting roads
    - Uses BFS for connectivity analysis
    - Implements Dijkstra's algorithm with priority queue for shortest paths
