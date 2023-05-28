/*图的遍历，深度优先，广度优先及其应用*/
#include<stdio.h>
#include<stdlib.h>
#define MAXV 10
#define MAX 30
#define INF 32767
typedef struct {
	int no;
	int info;
}Vertex;

typedef struct{
	int edges[MAXV][MAXV];
	int n, e;
	int vexs[MAXV];
}matGraph;

typedef struct Anode{
	int adjvex;
	struct Anode* nextarc;
	int weight;
}arcNode;

typedef struct Vnode{
	int info;
	arcNode* firstarc;
}Vnode;

typedef struct{
	Vnode adjlist[MAXV];
	int e, n;
}adjGraph;

void matToLink(matGraph g, adjGraph* &G)             //由邻接矩阵创建邻接表
{
	G = (adjGraph*)malloc(sizeof(adjGraph));
	for(int i = 0; i < g.n; i ++)
	{
		G -> adjlist[i].info = i;
		G -> adjlist[i].firstarc = NULL;
	}
	arcNode* p;
	for(int i = 0; i < g.n; i ++)
	{
		for(int j = g.n - 1; j >= 0; j --)
		{
			if(g.edges[i][j] != 0 && g.edges[i][j] != INF)
			{
				p = (arcNode*)malloc(sizeof(arcNode));
				p -> weight = g.edges[i][j];
				p -> adjvex = j;
				p -> nextarc = G -> adjlist[i].firstarc;
				G -> adjlist[i].firstarc = p;
			}
		}
	}
	G -> n = g.n;
	G -> e = g.e; 
}
int visited[MAXV] = {0};
void DFS(adjGraph* G, int v)                   //深度优先遍历图
{
	arcNode*p = G -> adjlist[v].firstarc;
	printf("%d ",v);
	visited[v] = 1;
	while(p)
	{
		if(visited[p -> adjvex] == 0)
		DFS(G, p -> adjvex);
		p = p -> nextarc;
	}
}

void DFS1(adjGraph* G)                //对于非连通图使用深度优先
{
	for(int i = 0; i < G -> n; i ++)
	{
		if(visited[i] == 0)
		DFS(G, i);
	}
}

void BFS(adjGraph* G, int v)                //广度优先遍历图
{
	
	int queue[MAX];
	int rear = -1, front = -1;
	printf("%d ", v);
	rear ++;
	queue[rear] = v;
	visited[v] = 1;
	while(rear != front)
	{
		int u = queue[++front];
		arcNode* p = G -> adjlist[u].firstarc;
		while(p)
		{
			if(visited[p -> adjvex] == 0)
			{
				printf("%d ", p -> adjvex);
				visited[p -> adjvex] = 1;
				rear ++;
				queue[rear] = p -> adjvex;
			}
			p = p -> nextarc;
		}
	}
	
}
void BFS1(adjGraph* G)                 //对于非连通图使用广度优先遍历
{
	for(int i = 0; i < MAXV; i ++)
	visited[i] = 0;
	for(int i = 0; i < G -> n; i ++)
	{
		if(visited[i] == 0)
		{
			BFS(G, i);
		}
	}
}
void print(adjGraph* G)                    //打印邻接表
{
	arcNode* p;
	for(int i = 0; i < G -> n; i ++)
	{
		printf("%d: ", G -> adjlist[i].info);
		p = G -> adjlist[i].firstarc;
		while(p)
		{
			printf("->[%d]%d ", p -> weight, p -> adjvex);
			p = p -> nextarc;
		}
		printf("\n");
		
	}
	printf("\n");
}

void destroy(adjGraph* & G)                     //销毁邻接表
{
	arcNode* pre, *p;
	for(int i = 0; i < G -> n; i ++)
	{
		pre = G -> adjlist[i].firstarc;
		if(pre)
		{
			p = pre -> nextarc;
		while(p)
		{
			free(pre);
			pre = p;
			p = p -> nextarc;
		}
		free(pre);
		}
	}
	free(G);
}

int main()
{
	matGraph g;
	adjGraph* G;
	g.n = 5;
	g.e = 6;
	for(int i = 0; i < g.n; i ++)
	{
		for(int j = 0; j < g.n; j ++)
		g.edges[i][j] = INF;
	}
	g.edges[1][0] = 10;
	g.edges[0][3] = 7;
	g.edges[2][0] = 9;
	g.edges[1][2] = 8;
	g.edges[2][3] = 6;
	//g.edges[2][4] = 5;                 //使5成为孤立点，G成为非连通图
	matToLink(g, G);
	print(G);
	/*
	DFS(G, 1);                          //对于连通图的深度遍历和广度遍历
	printf("\n");
	BFS(G, 1);
	*/
	DFS1(G);                            
	printf("\n");
	BFS1(G);
	destroy(G);
	return 0;
	
}
