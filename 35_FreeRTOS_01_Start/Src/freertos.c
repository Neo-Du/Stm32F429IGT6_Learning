/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//定义�?个测试用的列表和3个列表项
List_t TestList;		//测试用列�?
ListItem_t ListItem1;	//测试用列表项1
ListItem_t ListItem2;	//测试用列表项2
ListItem_t ListItem3;	//测试用列表项3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId Name_Start_TaskHandle;
osThreadId myTask02Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_Task(void const * argument);
void StartTask02(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory (StaticTask_t**ppxIdleTaskTCBBuffer,StackType_t**ppxIdleTaskStackBuffer,uint32_t*pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Name_Start_Task */
  osThreadDef(Name_Start_Task, Start_Task, osPriorityNormal, 0, 128);
  Name_Start_TaskHandle = osThreadCreate(osThread(Name_Start_Task), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_Task */
/**
 * @brief  Function implementing the Name_Start_Task thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_Task */
void Start_Task(void const * argument)
{
  /* USER CODE BEGIN Start_Task */
    for (;;)
    {
	HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_0);


	osDelay (100);
    }
  /* USER CODE END Start_Task */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the myTask02 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
    vListInitialise (&TestList);
    vListInitialiseItem (&ListItem1);
    vListInitialiseItem (&ListItem2);
    vListInitialiseItem (&ListItem3);

    ListItem1.xItemValue = 40;			//ListItem1列表项�?�为40
    ListItem2.xItemValue = 60;			//ListItem2列表项�?�为60
    ListItem3.xItemValue = 50;			//ListItem3列表项�?�为50

    printf ("/*******************列表和列表项地址*******************/\r\n");
    printf ("项目                              地址				    \r\n");
    printf ("TestList                          %#x					\r\n", (int) &TestList);
    printf ("TestList->pxIndex                 %#x					\r\n", (int) TestList.pxIndex);
    printf ("TestList->xListEnd                %#x					\r\n", (int) (&TestList.xListEnd));
    printf ("ListItem1                         %#x					\r\n", (int) &ListItem1);
    printf ("ListItem2                         %#x					\r\n", (int) &ListItem2);
    printf ("ListItem3                         %#x					\r\n", (int) &ListItem3);
    printf ("/************************结束**************************/\r\n");

    vListInsert(&TestList,&ListItem1);		//插入列表项ListItem1
    printf("/******************添加列表项ListItem1*****************/\r\n");
    printf("项目                              地址				    \r\n");
    printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
    printf("/*******************前后向连接分割线********************/\r\n");
    printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
    printf("/************************结束**************************/\r\n");


    vListInsert(&TestList,&ListItem2);	//插入列表项ListItem2
    printf("/******************添加列表项ListItem2*****************/\r\n");
    printf("项目                              地址				    \r\n");
    printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
    printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
    printf("/*******************前后向连接分割线********************/\r\n");
    printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
    printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
    printf("/************************结束**************************/\r\n");

    vListInsert(&TestList,&ListItem3);	//插入列表项ListItem3
    printf("/******************添加列表项ListItem3*****************/\r\n");
    printf("项目                              地址				    \r\n");
    printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
    printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
    printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
    printf("/*******************前后向连接分割线********************/\r\n");
    printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
    printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
    printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
    printf("/************************结束**************************/\r\n");

    uxListRemove(&ListItem2);						//删除ListItem2
    printf("/******************删除列表项ListItem2*****************/\r\n");
    printf("项目                              地址				    \r\n");
    printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
    printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
    printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
    printf("/*******************前后向连接分割线********************/\r\n");
    printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
    printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
    printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
    printf("/************************结束**************************/\r\n");

    TestList.pxIndex=TestList.pxIndex->pxNext;			//pxIndex向后移一项，这样pxIndex就会指向ListItem1�?
    vListInsertEnd(&TestList,&ListItem2);				//列表末尾添加列表项ListItem2
    printf("/***************在末尾添加列表项ListItem2***************/\r\n");
    printf("项目                              地址				    \r\n");
    printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
    printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
    printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
    printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
    printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
    printf("/*******************前后向连接分割线********************/\r\n");
    printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
    printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
    printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
    printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
    printf("/************************结束**************************/\r\n\r\n\r\n");
    while(1);
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
