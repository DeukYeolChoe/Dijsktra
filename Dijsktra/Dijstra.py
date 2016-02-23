from sys import argv

map = {}
map = dict()
rmap = {}
rmap = dict()
graphListSize = 0

def giveMeWeight(i, j):
    #print "rmap:", rmap
    #print "\n graphListSize:" , graphListSize
    for a in range(0, graphListSize):
        if graphList[a][0] == rmap[i] and graphList[a][1] == rmap[j]:
            return graphList[a][2]

def printPaths(stIndex, edIndex, distance, via, size):
    path = [-1] * size
    count = 0
    k = edIndex
    
    while True:
        path[count] = k
        count += 1
        if k == stIndex:
            break
        k = via[k]
    #print path
    #print rmap
    print "Path:"
    for i in range(count-1, -1, -1):
        if i == 0:
            print rmap[path[0]]
        else:
            print rmap[path[i]], " -> ", giveMeWeight(path[i], path[i-1]), " -> ",
    #print "\npath: ", path 
    print "\nThe total distance: ", distance[path[0]]

def giveMeIndex(vertex):
    return map[vertex]

def minDistance(distance, visited, size):
    minValue = 9999
    minIndex = 9999
    for i in range(0, size):
        if visited[i] == False and distance[i] <= minValue:
            minValue = distance[i]
            minIndex = i

    return minIndex
        
    
def Dijkstra(G, src, dest, size):
    visited = [False] * size
    distance = [9999] * size
    via = [-1] * size
    via.append(0)
    distance[src] = 0
    for i in range(0, size):
        s = minDistance(distance, visited, size)
        visited[s] = True;
        
        for e in range(0, size):
            sum = int(distance[s]) + int(G[s][e])
            if visited[e] == False and G[s][e] != 0 and distance[s] != 9999 and sum < distance[e]:
                distance[e] = distance[s] + int(G[s][e])
                via[e] = s         

    #print distance
    #print visited
    printPaths(src, dest, distance, via, size)




script, filename = argv
lines = open(filename, 'r')

graphList = []
graph = []
mapIndex = -1

for line in lines.readlines():
    if line == '\n':
        continue
    line = line.replace("\r", "")
    line = line.replace("\n", "")
    left, right, weight = line.split(" ")
    graph.append(left); graph.append(right); graph.append(weight)        
    graphList.append(graph)
    
    if not left in map:
        map[left] = mapIndex + 1
        mapIndex += 1         
    elif not right in map:
        map[right] = mapIndex + 1
        mapIndex += 1
    graph = []

rmap = dict((y,x) for x,y in map.iteritems())
#print rmap
# Create a matrix
#Matrix = (mapIndex+1)*[(mapIndex+1)*[0]]  
Matrix = [[0 for i in range(mapIndex+1)] for j in range(mapIndex+1)]
for i in range(0, len(graphList)):
    leftI = giveMeIndex(graphList[i][0])
    rightI = giveMeIndex(graphList[i][1])
    Matrix[leftI][rightI] = graphList[i][2]

print graphList
graphListSize = len(graphList)
#print "List Size: ", graphListSize
#print map

for i in range(0, len(Matrix)):
    for j in range(0, len(Matrix[0])):
        print Matrix[i][j],
    print "\n"

st = 0
ed = 0

while True:
    st_cmd = raw_input("Input a start point:")
    ed_cmd = raw_input("Input an end point:")
    st = map[st_cmd]
    ed = map[ed_cmd]
    if st == -1 or ed == -1:
        print "Trye again!!"
    else:
        break;

Dijkstra(Matrix, st, ed, mapIndex+1)
lines.close();
