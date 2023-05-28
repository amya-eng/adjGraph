/*邻接表实现有向图的基本操作，包括创建，插入，删除，打印，销毁*/
#include<stdio.h>
#include<stdlib.h>
#define MAXV 10
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

void insert_vex(adjGraph* &G, int v)           //插入一个顶点
{
	
	G -> adjlist[G -> n].info = v;
	G -> adjlist[G -> n].firstarc = NULL;
	G -> n ++;
	
}


void insert_edge(adjGraph* &G, int u, int v, int w)           //插入一条边<u, v>
{
	arcNode* p = (arcNode*)malloc(sizeof(arcNode));
	p -> adjvex = v;
	p -> nextarc = NULL;
	p -> weight = w;
	p -> nextarc = G -> adjlist[u].firstarc;
	G -> adjlist[u].firstarc = p;
	G -> e ++;
}

void dele_vex(adjGraph* & G, int v)              //删除一个结点,结点编号为v
{
	int count = 0;                             //count 记录关联到v结点的边有几条
	for(int i =0 ; i < G -> n; i ++)
	{
		arcNode*p, *pre;
		if(i == v)                             
		{
			p = G -> adjlist[i].firstarc;
			while(p)
			{
				count ++;
				p = p -> nextarc;
			}
		}
		else
		{
			pre = G -> adjlist[i].firstarc;
			while(pre && pre -> adjvex == v)                      //删除以v为终点的边
			{
				G -> adjlist[i].firstarc = pre -> nextarc;
				free(pre);
				pre = G -> adjlist[i].firstarc;
			}
			if(pre)
			{
				p = pre -> nextarc;
				while(p)
			{
				if(p -> adjvex == v)
				{
					count ++;
					pre -> nextarc = p -> nextarc;                //删除找到的边
					free(p);
					p = pre -> nextarc;
				}
				else
				{
					pre = p;
					p = p -> nextarc;
				}
			}
			}
		}
	}
	for(int i = v; i < G -> n - 1; i ++)
	{
		G -> adjlist[i] = G -> adjlist[i + 1];            //头结点数组从后往前覆盖，删除以v为起点的边
	}
	G -> n --;
	G -> e -= count;
}

void dele_edge(adjGraph* &G, int u, int v)            //删除一条边<u, v>
{
	arcNode* pre = G -> adjlist[u].firstarc, *p;
	if(pre && pre -> adjvex == v)              //起始如果有<u, v>，那么pre必然不为空
	G -> adjlist[u].firstarc = pre -> nextarc;
	p = pre -> nextarc;
	bool find = false;
	while(p && !find)
	{
		if(p -> adjvex == v)
		{
			pre -> nextarc = p -> nextarc;
			pre = p;
			p = p -> nextarc;
			find = true;
		}
	}
	G -> e --;
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
	g.edges[0][1] = 10;
	g.edges[0][3] = 7;
	g.edges[2][0] = 9;
	g.edges[1][2] = 8;
	g.edges[2][3] = 6;
	g.edges[2][4] = 5;
	matToLink(g, G);
	print(G);
	printf("insert:vex5\n:");
	insert_vex(G, 5);
	print(G);
	printf("insert:edge<1, 3>[99]\n");
	insert_edge(G, 1, 3, 99);
	print(G);
	printf("delete:vex3\n");
	dele_vex(G, 3);
	print(G);
	printf("delete:edgs<0, 1>\n");
	dele_edge(G, 0, 1);
	print(G);
	return 0;
	
}
