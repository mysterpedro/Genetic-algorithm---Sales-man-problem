import sys
import random
(nom_programme_python, a) = sys.argv



sommets = int (a)
graph = [[0 for _ in range(sommets)] for _ in range(sommets)]

for i in range(sommets):
    for j in range(i+1, sommets):
        val = random.randint(1, 300)
        graph[i][j] = val
        graph[j][i] = val


print(f"float graph[NUMBEROFCITIES][NUMBEROFCITIES] = {{")
for i in range(sommets):
    print("    {" + ", ".join(str(x) for x in graph[i]) + "},")
print("};")
