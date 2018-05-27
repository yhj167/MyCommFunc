/*hash table max size*/
#define HASH_TABLE_MAX_SIZE 40

/*hash table大小*/
int hash_table_size=0;

/*.BH-----------------------------------------------------------------
**                 结构体定义
**.EH-----------------------------------------------------------------
*/
/*hashTable结构*/
typedef SysIndexs HashKeyType;
typedef struct{     
    OMS_TYPE__CurrFaultReport curr_fault_report;
    unsigned int begin_time[SYS_FAULT_REPORT_MAX_NUM];
    unsigned int end_time[SYS_FAULT_REPORT_MAX_NUM];
    unsigned int report_valid[SYS_FAULT_REPORT_MAX_NUM];
}HashValueType;
typedef struct HashNode_Struct HashNode;

struct HashNode_Struct
{
    HashKeyType sKey;
    HashValueType nValue;
    HashNode* pNext;
};
HashNode* hashTable[HASH_TABLE_MAX_SIZE]; //hash table data strcutrue

/*=================hash table function======================*/
/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：string hash function
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
unsigned int hash_table_hash_str(const char* skey)
{
    const signed char *p = (const signed char*)skey;
    unsigned int h = *p;
    if(h)
    {
        for(p += 1; *p != '\0'; ++p){
            h = (h << 5) - h + *p;
        	}
    }
    return h;
}


/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：insert key-value into hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
int hash_table_insert(const HashKeyType skey, HashValueType nvalue)
{
	unsigned int pos = 0;
	HashNode* pHead = NULL;
	HashNode* pNewNode = NULL;

	if (hash_table_size >= HASH_TABLE_MAX_SIZE)
	{
		printf("out of hash table memory!\n");
		return 0;
	}

	pos = skey % HASH_TABLE_MAX_SIZE;
	pHead = hashTable[pos];
	while (pHead)
	{
		if (pHead->sKey == skey)
		{
			printf("hash_table_insert: key %d already exists!\n", skey);
			return 0;
		}
		pHead = pHead->pNext;
	}

	pNewNode = (HashNode*)malloc(sizeof(HashNode));
	memset(pNewNode, 0, sizeof(HashNode));
	pNewNode->sKey = skey;
	memcpy(&pNewNode->nValue, &nvalue, sizeof(HashValueType));

	pNewNode->pNext = hashTable[pos];
	hashTable[pos] = pNewNode;


	hash_table_size++;
	return 1;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：lookup a key in the hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
HashNode* hash_table_find(const HashKeyType skey)
{
	unsigned int pos = 0;
	
	pos = skey % HASH_TABLE_MAX_SIZE;
	if (hashTable[pos])
	{
		HashNode* pHead = hashTable[pos];
		while (pHead)
		{
			if (skey == pHead->sKey)
				return pHead;
			pHead = pHead->pNext;
		}
	}
	return NULL;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：free the memory of the hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void hash_table_release()
{
	int i;
	for (i = 0; i < HASH_TABLE_MAX_SIZE; ++i)
	{
		if (hashTable[i])
		{
			HashNode* pHead = hashTable[i];
			while (pHead)
			{
				HashNode* pTemp = pHead;
				pHead = pHead->pNext;
				if (pTemp)
				{
					free(pTemp);
				}

			}
		}
	}
}

//remove key-value frome the hash table
/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：string hash function
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void hash_table_remove(const HashKeyType skey)
{
	unsigned int pos = skey % HASH_TABLE_MAX_SIZE;
	if (hashTable[pos])
	{
		HashNode* pHead = hashTable[pos];
		HashNode* pLast = NULL;
		HashNode* pRemove = NULL;
		while (pHead)
		{
			if (skey == pHead->sKey)
			{
				pRemove = pHead;
				break;
			}
			pLast = pHead;
			pHead = pHead->pNext;
		}
		if (pRemove)
		{
			if (pLast)
				pLast->pNext = pRemove->pNext;
			else
				hashTable[pos] = NULL;

			free(pRemove);
		}
	}
	hash_table_size--;
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：print the content in the hash table
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void hash_table_print()
{
	int i;
	printf("===========content of hash table===========\n");
	for (i = 0; i < HASH_TABLE_MAX_SIZE; ++i){
		if (hashTable[i])
		{
			HashNode* pHead = hashTable[i];
			printf("%d=>", i);
			while (pHead)
			{
				printf("%d:%d  ", pHead->sKey, pHead->nValue.begin_time);
				pHead = pHead->pNext;
			}
			printf("\n");
		}
	}
}

/*.BH-----------------------------------------------------------------
**
**函数名：
**
**功能：初始化系统名称的hashTable，插入所有系统名称
**
**参数： 无
**
**返回值：无
**
**设计注记：
**
**.EH-----------------------------------------------------------------
*/
void Common_InitHashTable()
{
	
    hash_table_size = 0;
    memset(hashTable, 0, sizeof(HashNode*) * HASH_TABLE_MAX_SIZE);
}
