/*这个程序用邻接表解决迷宫问题*/
/*版权声明：本文为CSDN博主「奚满庭」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/m0_54570435/article/details/130276106
只有一个函数是我自己写的，即函数cutPath()*/

#include <stdio.h>
#include <malloc.h>
#define MAX_SIZE 100
#define M 4                                 // 行数
#define N 4                                 // 列数
 
/*----------------------------以下定义邻接表类型---------------------------*/
typedef struct ANode                        // 边的结点结构类型
{
    int i, j;                               // 该边的终点位置(i,j)
    struct ANode *nextarc;                  // 指向下一条边的指针
}ArcNode;
 
typedef struct Vnode                        // 邻接表头结点的类型
{
    ArcNode *firstarc;                      // 指向第一个相邻点
}VNode;
 
typedef struct
{
    VNode adjlist[M + 2][N + 2];            // 邻接表头结点数组
}ALGraph;                                   // 图的邻接表类型
 
typedef struct
{
    int i;                                  // 当前方块的行号
    int j;                                  // 当前方块的列号
}Box;
 
 typedef struct{
 	int i;
 	int j;
 	int parent;
 }Box_q;
 
typedef struct
{
    Box data[MAX_SIZE];
    int length;                             // 路径长度
}PathType;                                  // 定义路径类型
 
int cnt = 0;
int visited[M + 2][N + 2] = {0};
 
/*-------------------由迷宫数组mg建立对应的邻接表G--------------------*/
static void CreateAdj(ALGraph *&G, int mg[][N + 2])
{
    int i, j;
    int di;
    int i1, j1;
    ArcNode *p;
 
    G = (ALGraph *)malloc(sizeof(ALGraph));
    for(i = 0; i < M + 2; i++)              // 给邻接表中所有头结点的指针域设置初值
    {
        for(j = 0; j < N + 2; j++)
        {
            G->adjlist[i][j].firstarc = NULL;
        }
    }
 
    for(i = 1; i <= M; i++)                 // 检查mg中每个元素
    {
        for(j = 1; j <= N; j++)
        {
            if(mg[i][j] == 0)
            {
                di = 0;
                while(di < 4)
                {
                    switch(di)
                    {
                    case 0:                                     // 向上走
                        i1 = i - 1;
                        j1 = j;
                        break;
                    case 1:                                     // 向右走
                        i1 = i;
                        j1 = j + 1;
                        break;
                    case 2:                                     // 向下走
                        i1 = i + 1;
                        j1 = j;
                        break;
                    case 3:                                     // 向左走
                        i1 = i;
                        j1 = j - 1;
                        break;
                    }
                    if(mg[i1][j1] == 0)                         // (i1,j1)为可走方块
                    {
                        p = (ArcNode *)malloc(sizeof(ArcNode)); // 创建一个结点p
                        p->i = i1;
                        p->j = j1;
                        p->nextarc = G->adjlist[i][j].firstarc; // 将p结点链到链表后
                        G->adjlist[i][j].firstarc = p;
                    }
                    di++;
                }
            }
        }
    }
}
 
/*-----------------------输出邻接表G--------------------------*/
static void DispAdj(ALGraph *G)
{
    int i, j;
    ArcNode *p;
 
    for(i = 0; i < M + 2; i++)
    {
        for(j = 0; j < N + 2; j++)
        {
            printf(" [%d,%d]: ", i, j);
            p = G->adjlist[i][j].firstarc;
            while(p != NULL)
            {
                printf("(%d,%d)", p->i, p->j);
                p = p->nextarc;
            }
            printf("\n");
        }
    }
}
 
/*---------------------销毁邻接表--------------------------*/
static void DestroyAdj(ALGraph *&G)
{
    int i, j;
    ArcNode *pre, *p;
 
    for(i = 0; i < M + 2; i++)
    {
        for(j = 0; j < N + 2; j++)
        {
            pre = G->adjlist[i][j].firstarc;
            if(pre != NULL)
            {
                p = pre->nextarc;
                while(p != NULL)
                {
                    free(pre);
                    pre = p;
                    p = p->nextarc;
                }
                free(pre);
            }
        }
    }
    free(G);
}
 
/*--------------在图G中采用DFS算法求(xi,yi)到(xe,ye)的所有路径-------------*/
// path数组记录访问过的顶点序列，当找到出口时输出path中的访问序列
static void FindPath(ALGraph *G, int xi, int yi, int xe, int ye, PathType path)
{
    ArcNode *p;
 
    visited[xi][yi] = 1;                                        // 置已访问标记
    path.data[path.length].i = xi;                              // 设置起始方块的行号
    path.data[path.length].j = yi;                              // 设置起始方块的列号
    path.length++;
 
    if((xi == xe) && (yi == ye))                                // 走到迷宫的出口(M,N)时，输出迷宫路径
    {
        printf(" 迷宫路径%d: ", ++cnt);
        for(int k = 0; k < path.length; k++)
        {
            printf("(%d,%d)", path.data[k].i, path.data[k].j);  // 输出迷宫的行号和列号
        }
        printf("\n");
    }
    p = G->adjlist[xi][yi].firstarc;                            //p指向顶点v的第一条边顶点
    while(p != NULL)
    {
        if(visited[p->i][p->j] == 0)                            // 若(p->i,p->j)方块未访问，则递归访问它
            FindPath(G, p->i, p->j, xe, ye, path);
        p = p->nextarc;                                         //p指向顶点v的下一条边顶点
    }
    visited[xi][yi] = 0;                                        // 取消访问标记
}
 
void cutPath(ALGraph* G, int xi, int yi, int xe, int ye)          //输出一条最短路径
{
	for(int i = 0; i < M + 2; i ++)
	{
		for(int j = 0;  j < N + 2; j ++)
		visited[i][j] = {0};
	}
	ArcNode* p;
	Box_q queue[MAX_SIZE], e;
	int front = -1, rear = -1;
	rear ++;
	e.i = xi;
	e.j = yi;
	e.parent = -1;
	queue[rear] = e;                    //（xi, yi）入队
	visited[xi][yi] = -1;
	int x1, y1;
	while(rear != front)
	{
		Box_q e1 = queue[++front];
		if(e1.i == xe && e1.j == ye)
		{
			while(e1.parent != -1)
			{
				printf("(%d, %d)", e1.i, e1.j);
				e1 = queue[e1.parent];
			}
			printf("(%d, %d)", e1.i, e1.j);
			return;
		}
		p = G -> adjlist[e1.i][e1.j].firstarc;
		while(p)
		{
			if(visited[p -> i][p -> j] == 0)
			{
				Box_q e2;
				e2.i = p -> i;
				e2.j = p -> j;
				e2.parent = front;
				rear ++;
				queue[rear] = e2;
			}
			p = p -> nextarc;
		}	
	}
	printf("\n");
	return;		
}
int main(void)
{
    ALGraph *G;
    int mg[M + 2][N + 2] = {                                    // 一个迷宫:其四周加上均为1的外框
        {1, 1, 1, 1, 1, 1}, 
		{1, 0, 0, 0, 1, 1},
        {1, 0, 1, 0, 0, 1}, 
		{1, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 1}, 
		{1, 1, 1, 1, 1, 1}
    };
    CreateAdj(G, mg);
    printf("迷宫对应的邻接表:\n");
    DispAdj(G);                                                 // 输出邻接表
    PathType path;
    path.length = 0;
    printf("所有的迷宫路径:\n");
    FindPath(G, 1, 1, M, N, path);
    printf("\n");
    printf("最短的迷宫路径:\n");
    cutPath(G, 1, 1, M, N);
    DestroyAdj(G);
    return 0;
}
