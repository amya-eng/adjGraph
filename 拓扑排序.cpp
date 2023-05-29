#include<stdio.h>                       /*输出所有可能的拓扑排序序列*/
#include<stdlib.h>
#define MAXV 10
#define INF 32767
typedef struct{
	int no;
	int info;
}vexType;
typedef struct{
	int edges[MAXV][MAXV];
	int n, e;
	vexType vexs[MAXV];
}MatGraph;

typedef struct arcNode{
	int adjvex;
	struct arcNode* nextarc;
	int weight;
}arcNode;
typedef struct VNode{
	int info;
	struct arcNode* firstarc;
	int count;
}VNode;
typedef struct{
	struct VNode adjlist[MAXV];
	int n, e;
}AdjGraph;

void MatToGraph(AdjGraph* &G, MatGraph g)
{
	G = (AdjGraph*)malloc(sizeof(AdjGraph));
	G -> n = g.n;
	G -> e = g.e;
	for(int i = 0; i < G -> n; i ++)
	{
		G -> adjlist[i].firstarc = NULL;
		G -> adjlist[i].info = i;
		G -> adjlist[i].count = 0;
	}
	for(int i = 0; i < g.n; i ++)
	{
		for(int j = g.n - 1; j >= 0; j --)
		{
			if(g.edges[i][j] != 0 && g.edges[i][j] != INF)
			{
				arcNode*p = (arcNode*)malloc(sizeof(arcNode));
				p -> adjvex = j;
				p -> nextarc = G -> adjlist[i].firstarc;
				G -> adjlist[i].firstarc = p;
				p -> weight = g.edges[i][j];
			}
		}
	}
}

void print(AdjGraph* G)
{
	for(int i = 0; i < G -> n;i ++)
	{
		arcNode* p = G -> adjlist[i].firstarc;
		printf("%d: ", G -> adjlist[i].info);
		while(p)
		{
			printf("->[%d]%d ", p -> weight, p -> adjvex);
			p = p -> nextarc;
		}
		printf("\n");
	}
	printf("\n");
}

void printTP(AdjGraph* &G)
{
	arcNode*p;
	for(int i = 0; i < G -> n; i ++)
	{
		arcNode*p = G -> adjlist[i].firstarc;
		while(p)
		{
			G -> adjlist[p -> adjvex].count ++;
			p = p -> nextarc;
		}
	}
	int j;
	int stack[MAXV];
	int top = -1;
	for(j = 0; j < G -> n; j ++)
	{
		if(G -> adjlist[j].count == 0)
		{
			top ++;
			stack[top] = j;
		}
	}
	while(top != -1)
	{
		j = stack[top];
		printf("%d ",j);
		top --;                             //栈是先取出来再自减
		p = G -> adjlist[j].firstarc;
		while(p)
		{
			G -> adjlist[p -> adjvex].count --;
			if(G -> adjlist[p -> adjvex].count == 0)              //count需要通过头结点数组获得
			{
				top ++;
				stack[top] = p -> adjvex;
			}
			p = p -> nextarc;
		}
	}
	
}

/*
void printAllTP(AdjGraph* &G, int*path, int d)               //自己写的，作废，运行不出来……
{
	arcNode*p;
	for(int i = 0; i < G -> n; i ++)
	{
		arcNode*p = G -> adjlist[i].firstarc;
		while(p)
		{
			G -> adjlist[p -> adjvex].count ++;
			p = p -> nextarc;
		}
	}
	int j;
	int stack[MAXV];
	int top = -1;
	for(j = 0; j < G -> n; j ++)
	{
		if(G -> adjlist[j].count == 0)
		{
			top ++;
			stack[top] = j;
		}
	}
	while(true)
	{
		if(d == G -> n - 1)
		{
			for(int i = 0; i <= d; i ++)
			{
				printf("%d ", path[i]);
			}
			printf("\n");
			G -> adjlist[path[d]].count ++;
			d --;
			top ++;
			
		}
		while(top != -1)
		{
		j = stack[top];
		top --;    
		d ++;
		path[d] = j;
		                         //栈是先取出来再自减
		p = G -> adjlist[j].firstarc;
		while(p)
		{
			G -> adjlist[p -> adjvex].count --;
			if(G -> adjlist[p -> adjvex].count == 0)              //count需要通过头结点数组获得
			{
				top ++;
				stack[top] = p -> adjvex;
			}
			p = p -> nextarc;
		}
		}
	}
	
}
*/
void backtrack(int* result, int cnt, AdjGraph* graph) {                      //chatgpt写的，可以运行……
    if (cnt == graph->n) { // 完成图的所有顶点排列，输出结果
        for (int i = 0; i < cnt; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        return;
    }

    // 找到一个入度为 0 的未访问节点
    for (int i = 0; i < graph->n; i++) {
        if (graph->adjlist[i].count == 0 && graph->adjlist[i].info != -1) {
            result[cnt] = graph->adjlist[i].info;
            graph->adjlist[i].info = -1; // 标记为已访问
            for (arcNode* p = graph->adjlist[i].firstarc; p != NULL; p = p->nextarc) {
                graph->adjlist[p->adjvex].count--; // 更新相邻节点的入度计数
            }
            backtrack(result, cnt+1, graph);
            for (arcNode* p = graph->adjlist[i].firstarc; p != NULL; p = p->nextarc) {
                graph->adjlist[p->adjvex].count++; // 恢复相邻节点的入度计数
            }
            graph->adjlist[i].info = result[cnt]; // 恢复当前节点的标记
        }
    }
}

void topological_sort(AdjGraph* graph) {
    int result[MAXV] = {0}; // 存储拓扑序列
    backtrack(result, 0, graph);
}

void destroy(AdjGraph* &G)
{
	for(int i = 0; i < G -> n; i ++)
	{
		arcNode*pre = G -> adjlist[i].firstarc, *p;
		if(pre)
		{
			p = pre -> nextarc;
			while(p)
			{
				free(pre);
				pre = p;
				p = p -> nextarc;
			}
		}
	}
	free(G);
}

int main()
{
	MatGraph g;
	AdjGraph* G = NULL;
	g.n = 6;
	g.e = 6;
	for(int i = 0; i < g.n; i ++)
	{
		for(int j = 0; j < g.n; j ++)
		{
			if(i != j)
			g.edges[i][j] = INF;
			else
			g.edges[i][j] = 0;
		}
	}
	g.edges[0][1] = g.edges[1][2] = g.edges[2][3] = g.edges[4][1] = g.edges[5][3] = g.edges[4][5] = 1;
	MatToGraph(G, g);
	print(G);
	int d = -1;
	int path[MAXV];
	//printAllTP(G, path, d);
	topological_sort(G);
	destroy(G);
}
