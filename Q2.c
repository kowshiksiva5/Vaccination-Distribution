#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int checking = 40,TIME_sleep =2;
int arr[10000];
typedef struct company
{
	int id;
	int prepare;
	int capacity;
	int probability;
} company;
company *company_input;
int n,m,k,var,randpeople;
pthread_mutex_t mutex[10000];
typedef struct student
{
	int id;
	int valid;
	int count;
} student;
student *stud_id;
typedef struct vaccine
{
	int id;
	int  cap;
	int allow;
} vaccine ;
vaccine *vaccine_input;

void *prep_vaccine(void *inp)
{
	int w,temp,temp1,temp2,temp3;
	company * input =(company *) inp;
	w =2+rand()%4;
	temp = input->prepare;
	if(temp==0)
	{
		printf("company with id %d is preparing vaccine \n",input->id);
		temp1 = rand()%10;
		temp2 = rand()%26;
		temp3 = rand()%100;
		input->prepare=temp1+1;
		input->capacity=temp2+25;
		input->probability=temp3;
		sleep(w);
		printf("company with id %d done with vaccine has %d vaccines with capacity %d each\n",input->id,input->prepare,input->capacity);
	}
	return NULL;
}
void* student_came(void *inp)
{
	int t1,t2,t3;
	student * inuts =(student *) inp;
	t1 = inuts->id;
	inuts->valid=0;
	t2 = rand()%4;
	printf("Student with  id %d has just came \n",t1);
	sleep(t2+1);
	printf("Student with  id %d is waiting for slot\n",t1);
	return NULL;
}
void* vaccine_fill_by_company(void *zone)
{
	int j,t1,t2,t3,t4,t5;
	company * input =(company*) zone;
	j=0;
	while(j<n)
	{
		t1 = pthread_mutex_lock(&mutex[j]);
		if(t1 == 0 )
		{
			t2 = input->prepare;
			t3 = vaccine_input[j].cap;
			if(t2 >0 && t3 ==0 )
			{
				input->prepare--;
				t4 = input->capacity;
				vaccine_input[j].cap=t4;	
				printf("zone with id %d is refilled by company with id %d\n",vaccine_input[j].id,input->id);	
			}
			//sleep(0.05);
			pthread_mutex_unlock(&mutex[j]);
		}	
		else
		{
			continue ;
		}
		j++;

	}
	return NULL;
}

void* vaccine_ready_to_fill(void* inp)
{
	int j=rand()%10+1;
	vaccine * input =(vaccine *) inp;
	int k2,k1,t1,t2,t3,t4;
	t1 = input->cap;
	k2 = t1;
	if(j > k2)
	{
		j=k2;
	}
	t2 = j;
	pthread_mutex_lock(&mutex[input->id -1]);
	input->allow = t2;
	t3 = input->allow;
	t4 = input->id;
	pthread_mutex_unlock(&mutex[input->id-1]);
	sleep(TIME_sleep);
	printf("Slots number at zone with id %d is %d\n",t4,t3);	
	return NULL;
}
void* student_at_zone(void* inp)
{

	student * input=(student *)inp;
	int f=0,j;
	j=0;
	while(j<n)
	{
		int t1,t2,t3,t4;
		if(pthread_mutex_trylock(&mutex[j])==0)
		{
			if(input->valid == 0 && vaccine_input[j].allow > 0 && input->count<=3)
			{
				t1 = input->count;
				input->count++;
				printf("Student with id %d is vaccinated for the %d time at zone with id %d\n",input->id,input->count,vaccine_input[j].id);
				input->valid=-1;
				t2 = vaccine_input[j].allow;
				vaccine_input[j].allow--;
				t3 = vaccine_input[j].cap;
				vaccine_input[j].cap--;

				if(company_input->probability >= checking)
				{
					f = 1;
				}
			}
			pthread_mutex_unlock(&mutex[j]);
		}
		else
		{
			continue;
		}
		j++;
	}
	sleep(3);
	if(f==1)
	{
		int t1;
		t1 = input->id;
		printf("Student with id %d exited from vaccination zone\n",t1);
	}
	return NULL;
}
int main()
{
	int i,s,f,j;
	int l,l2,point;
	s = 0;
	l2 = 0;
	printf("\nEnter number of companies:");
	scanf("%d",&m);
	printf("\nNumber of vaccination zones");
	scanf("%d",&n);
	printf("\nNumber of Students:");
	scanf("%d",&k);
	printf("\n\n");
	pthread_t comp[m],zone[n];
	vaccine_input = (vaccine*)malloc(sizeof(vaccine)*n);
	i=0;
	while(i<n)
	{
		vaccine_input[i].id=i+1;
		vaccine_input[i].cap=0;
		i++;
	}
	company_input =(company*)malloc(sizeof(company)*m);
	i=0;
	while(i<m)
	{
		company_input[i].prepare=0;
		company_input[i].capacity=0;
		company_input[i].id=i+1;
		i++;
	}
	pthread_t students[k];
	stud_id =(student *)malloc(sizeof(student)*k);
	i=0;
	while(i<k)
	{
		stud_id[i].id=i+1;
		stud_id[i].valid=-2;
		i++;
	}
	l2=0;
	i=0;
	while(i<n)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
	while(1)
	{
		point=f=s=0;
		memset(arr,0,10000*4);
	
		l=rand()%20;
		i=0;
		while(i<k&&l>0)
		{
			int temp;
			temp = stud_id[i].valid;
			if(temp == -2)
			{
				l--;
				s++;
				arr[point]=i;
			}
			if(temp==-2)
			{
				point++;
				pthread_create(&students[i], NULL, student_came, (void*)(&stud_id[i]));
			}
			i++;
		}
		i=0;
		while(i<point)
		{
			pthread_join(students[arr[i]], NULL);
			i++;
		}
		if(s==0)
		{
			sleep(5);
			printf("Simulation over\n");
			return 0;
		}
		if(l2==1)
		{
			i=0;
			while(i<m)
			{
				pthread_create(&comp[i], NULL, vaccine_fill_by_company, (void*)(&company_input[i]));
				i++;
			}
			i=0;
			while(i<m)
			{
				pthread_join(comp[i], NULL);
				i++;
			}
		}
		i=0;
		while(i<n)
		{
			if(vaccine_input[i].cap)
			{
				f=1;
			}
			i++;
		}
		i=0;
		while(i<m)
		{
			if(company_input[i].prepare)
			{
				f=1;
			}
			i++;
		}
		if(f==0)
		{
			printf("All vaccine zones are emptied started producing\n");
			i=0;
			while(i<m)
			{
				pthread_create(&comp[i], NULL, prep_vaccine, (void*)(&company_input[i]));
				i++;
			}
			i=0;
			while(i<m)
			{
				pthread_join(comp[i], NULL);
				i++;
			}
		} 
		if(l2==1)
		{
			i=0;
			while(i<m)
			{
				printf("company with id %d has vaccines %d with capacity %d each\n",company_input[i].id,company_input[i].prepare,company_input[i].capacity);
				i++;
			}
			i=0;
			while(i<n)
			{
				printf("vaccine with id %d has capacity %d \n",vaccine_input[i].id,vaccine_input[i].cap);	
				i++;
			}
			i=0;
			while(i<n)
			{
				pthread_create(&zone[i], NULL, vaccine_ready_to_fill, (void*)(&vaccine_input[i]));
				i++;
			}
			i=0;
			while(i<n)
			{
				pthread_join(zone[i], NULL);			
				i++;
			}
			i=0;
			while(i<k)
			{
				pthread_create(&students[i], NULL, student_at_zone, (void*)(&stud_id[i]));
				i++;
			}
			i=0;
			while(i<n)
			{
				pthread_join(students[i], NULL);			
				i++;
			}
		}
		l2=1;
	}
	return 0;
}