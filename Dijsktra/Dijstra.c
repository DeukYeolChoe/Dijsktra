#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
/*
 * matrixSize - Number of vertices in the graph
 * s - Start
 * e - End
 */
#define INFINITE 9999
#define ARRAY_SIZE 50

typedef struct graph
{
	char leftVertex[ARRAY_SIZE];
	char rightVertex[ARRAY_SIZE];
	char weight[ARRAY_SIZE];
} graph;

typedef struct map
{
	char vertexName[ARRAY_SIZE];
	int index;
} map;

int gCount;
struct graph *G;
struct map *M;
int mapIndex;
int matrixSize;

int giveMeIndex(char* vertex)
{
	int i;
	int size = matrixSize;
	for (i = 0; i < size; i++)
	{
		if (strcmp(vertex, M[i].vertexName) == 0)
			return M[i].index;
	}
	return -1;
}

char* giveMeVertex(int index)
{
	int i;
	int size = matrixSize;
	for (i = 0; i < size; i++)
	{
		if (index == M[i].index)
			return M[i].vertexName;
	}
	return "n/a";
}

int giveMeWeight(int i, int j)
{
	char left[ARRAY_SIZE];
	char right[ARRAY_SIZE];
	
	strcpy(left, giveMeVertex(i));
	strcpy(right, giveMeVertex(j));
	
	int in;
	int size = gCount;
	for (in = 0; in < size; in++)
	{
		if (strcmp(G[in].leftVertex, left) == 0 && strcmp(G[in].rightVertex, right) == 0)
			return atoi(G[in].weight);
	}
	return 0;
}

void createMatrix(int matrix[][matrixSize])
{
	int i, j;
	int size = matrixSize;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			int weight = giveMeWeight(i,j);
			matrix[i][j] = weight;
		}
	}
	
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (matrix[i][j] > 0)
				matrix[j][i] = matrix[i][j];
		}
	}	
}

void createMap(char* vertex, int size)
{
	int i;
	bool found = false;
	for (i = 0; i < size; i++)
	{
		if (strcmp(vertex, M[i].vertexName) == 0)
			found = true;
		if (strcmp(M[i].vertexName, "") == 0)
			break;
	}

	if (!found)
	{
		strcpy(M[mapIndex].vertexName, vertex);
		M[mapIndex].index = mapIndex;
		mapIndex++;	
	}	 	
}

void createGraph(char* line, int size)
{
	int i;
	int first;
	char *ptr;
	ptr = strtok(line, " ");
	strcpy(G[gCount].leftVertex, ptr);
	createMap(ptr, size);
		
	ptr = strtok(NULL, " ");
	strcpy(G[gCount].rightVertex, ptr);
	createMap(ptr, size);	

	ptr = strtok(NULL, " \t\r\n");
	strcpy(G[gCount].weight, ptr);
}


/*
 * Find the vertex with minimum distance value from unvisited vertices
 */
int minDistance(int distance[], bool visited[])
{
	int minmatrixSizealue = INFINITE;
	int minIndex = INFINITE;
	int i;
	for (i = 0; i < matrixSize; i++)
	{
		if (visited[i] == false && distance[i] <= minmatrixSizealue)
		{
			minmatrixSizealue = distance[i];
			minIndex = i;
		}
	}
	return minIndex;
}

void printOut(int* distance)
{
	printf("\nVertex \t\t Distance\n");
	int i;
	for (i = 0; i < matrixSize; i++)
	{
		printf("%s \t\t %d\n", giveMeVertex(i), distance[i]);
	}
}

void printPaths(int stIndex, int edIndex, int* distance, int* via)
{
	int path[matrixSize], count = 0;
	int k = edIndex;
	int i,j;

	while(1)
	{
		path[count++] = k;
		if (k == stIndex)
			break;
		k = via[k]; 
	}
	
	
	//for (i = 0; i < count; i++)
	//	printf("%d ", path[i]);

	printf("\nPath: \n");
	for (i = count - 1; i >= 0; i--)
	{
		if (i == 0)
		{
			printf("%s ", giveMeVertex(path[0]));
		}
		else
		{
			printf("%s -> (%d) -> ", giveMeVertex(path[i]),giveMeWeight(path[i],path[i-1]));
		}

	}
	printf("\n");
	printf("The total distance: %d\n", distance[path[0]]);	
}

void Dijkstra(int G[matrixSize][matrixSize], int src, int dest)
{
	// If a vertex is visited, then it will be true
	bool visited[matrixSize];
	// Hold the shortest distance from src to i
	int distance[matrixSize];

	// Initialize all distance as a big number and visited array as false
	int i;
	int via[matrixSize];
	via[0] = 0;
	for (i = 0; i < matrixSize; i++)
	{
		distance[i] = INFINITE;
		visited[i] = false;
	}

	distance[src] = 0;

	for (i = 0; i < matrixSize - 1; i++)
	{
		/*
		 * (s,e) - The path from s to e
		 */
		int s = minDistance(distance, visited);
		visited[s] = true;
		int e;
		for (e = 0; e < matrixSize; e++)
		{
			/*
			 * Conditions
			 * 1. unvisited
			 * 2. G[u][v] isn't 0
			 * 3. weight[s] isn't INFINITE
			 * 4. if the sum of weight and a vertex is less than the weight of the vertex[e]
			 */
			if (!visited[e] && G[s][e] != 0 && distance[s] != INFINITE && distance[s] + G[s][e] < distance[e])
			{
				// update the distance
				distance[e] = distance[s] + G[s][e];
				via[e] = s;
			}
		}
	}
	printOut(distance);
	//int a;
	//for (a = 0; a < matrixSize-1; a++)
	//	printf("%d ", via[a]);
	//printf("\n\n");
	printPaths(src, dest, distance, via);
}

int main(int argc, char *argv[])
{
	FILE *fileReader;
	
	if (argc != 2)
	{
		printf("Invalid cmd. Please, follow this: ./file input.txt\n");
		exit(1);
	}

	fileReader = fopen(argv[1], "r");
	char line[50];	
	if (fileReader == NULL)
	{
		printf("Could not open the file!!");
		return 1;		
	}
	
	int count = 0;
	int ch;
	do
	{
		ch = fgetc(fileReader);
		if (ch == '\n') count++;
	}while(ch != EOF); 
	close(fileReader);	

	G = (graph*) malloc(sizeof(graph) * count);
	M = (map*) malloc(sizeof(map) * count * 2);
 	gCount = 0;
	mapIndex = 0;
	fileReader = fopen(argv[1], "r");
	while (fgets(line, sizeof(line), fileReader) != NULL)
	{
		if (line[0] == '\n')
			continue;
		createGraph(line, count*2);
		gCount++;
	}

	int mapSize = mapIndex;
	int matrix[mapSize][mapSize];	
	matrixSize = mapSize;
	createMatrix(matrix);


	int b,c;
	for (b = 0; b < matrixSize; b++)
	{
		for (c = 0; c < matrixSize; c++)
		{
			printf("%d ", matrix[b][c]);
		}
		printf("\n");
	}
	printf("--------------------------------\n");


	int st = 0;
	int ed = 0;
	while(1)
	{
		printf("Input a start point: ");
		char cmd_st[ARRAY_SIZE];
		scanf("%s", cmd_st);
		printf("Input a end point: ");
		char cmd_ed[ARRAY_SIZE];
		scanf("%s", cmd_ed);
		st = giveMeIndex(cmd_st);
		ed = giveMeIndex(cmd_ed);
		if (st == -1)
			printf("Try again!");
		else
			break;
	}			
	Dijkstra(matrix, st, ed);

	fclose(fileReader);
	free(G);
	return 0;
}
