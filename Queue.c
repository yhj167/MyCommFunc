/*FIFO队列中参数的类型*/
typedef int QElemtype;

 /*对节点的结构定义*/
typedef struct QNode
{
    QElemtype data;
    struct QNode *next;
}QNode,*QueuePtr;
 
/*FIFO队列的结构定义*/
typedef struct{     
    QueuePtr head;
    QueuePtr rear;
}LinkQueue;
LinkQueue updateListMsgQ;

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：链表实现消息队列，支持任意类型数据
**
**参数： 
**
**返回值：
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
 //初始化队列
int Queue_Init(LinkQueue* que)
{
    que->head=que->rear=(QueuePtr)malloc(sizeof(QNode));
    if(!que->head)  //这段代码对队列里面的用户自定义数据类型进行了初始化
        return ERROR;
    return OK;
}
 
//回收队列
int Queue_Destory(LinkQueue* que) 
{
    while(que->head)
    {
        que->rear = que->head->next;
        free(que->head);
        que->head=que->rear;
    }
    return OK;
}

 /*将元素插到尾部*/
int Queue_Push(LinkQueue* que,QElemtype e)
{
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if(!p) //若未能申请到空间，便退出
        return ERROR;
    p->data=e;
    p->next=NULL;

    que->rear->next = p;
    que->rear=p;
    return OK;
}

 /*指针que指向头节点的后一节点，既要出队的节点*/
int Queue_Pop(LinkQueue* que,QElemtype *t)
{
    if(que->rear==que->head)
        return ERROR; //队列为空

    QueuePtr p = que->head->next;
    *t=p->data;

    que->head->next=p->next;
    if(que->rear==p) //这个判断是 确保在清空队列的时候，让rear指针归位。
        que->rear=que->head;
    free(p);
    return OK;
}

/*遍历队列*/
int Queue_View(LinkQueue* que)
{
    if(que->rear == que->head)
        return ERROR;
    
    QueuePtr p =que->head->next;
    while(p)
    {
        printf("val:%d",p->data);
        p=p->next;
    }
    return OK;
}
