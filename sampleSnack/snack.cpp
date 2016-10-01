#include "stdafx.h"
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <iostream>
using namespace std;

#define COUT		cout<<"■";
#define COUTFOOD	cout<<"☆";
#define COUTSNAKE	cout<<"□";

int snakelife;
bool snakedie(struct snake_body *head);

//蛇的结构
struct snake_body
{
	int x;
	int y;
	struct snake_body *next;
};

//食物结构									
struct food
{
	int x;
	int y;
};

//屏幕的初始化
void screenstart(COORD pos,HANDLE hout)
{
	int x=0,y=0;
	SetConsoleTextAttribute(hout, 0x06);
	for(;x<80;x+=2)
	{
		pos.X=x;
		COUT
	}
	for(;y<24;y++)
	{
		pos.Y=y;
		SetConsoleCursorPosition(hout,pos);
		COUT
	}
	pos.X=0;
	for(y=0;y<24;y++)
	{
		pos.Y=y;
		SetConsoleCursorPosition(hout,pos);
		COUT
	}
	pos.Y=24;
	pos.X=0;
	SetConsoleCursorPosition(hout,pos);
	for(x=0;x<80;x+=2)
	{
		COUT
	}
	pos.X=0;
	pos.Y=0;
	SetConsoleCursorPosition(hout,pos);
}

//产生食物 返回食物的位置
food Food()
{
	srand(time(0));
	food f;

	f.x=2*(rand()%38+1);
	f.y=rand()%23+1;
	
	return f;
}

void foodprint(food f,COORD pos,HANDLE hout)
{
	pos.X=f.x;
	pos.Y=f.y;
	SetConsoleCursorPosition(hout,pos);
	SetConsoleTextAttribute(hout, 0x03);
	COUTFOOD
}

//初始化蛇 返回蛇头结点地址
struct snake_body* snakestart()
{
	struct snake_body *head,*p,*tail;
	tail=(struct snake_body*)malloc(sizeof(struct snake_body));
	head=p=tail;
	p->x = 40;
	p->y = 12;
	tail=(struct snake_body*)malloc(sizeof(struct snake_body));
	p=p->next=tail;
	p->x = 38;
	p->y = 12;
	tail=(struct snake_body*)malloc(sizeof(struct snake_body));
	p=p->next=tail;
	p->x = 36;
	p->y = 12;
	p->next=NULL;
	return head;
}

//刷新蛇
void snakeprint(struct snake_body *head,COORD pos,HANDLE hout)
{
	pos.X=head->x;
	pos.Y=head->y;
	SetConsoleCursorPosition(hout,pos);
	SetConsoleTextAttribute(hout, 0x01);
	cout << "■";
	head=head->next;
	while(head)
	{
		pos.X=head->x;
		pos.Y=head->y;
		SetConsoleCursorPosition(hout,pos);
		COUTSNAKE
		head = head->next;
	}
}

//方向检查 相同或相反返回false 否者返回true
bool atordir(char s,char a)
{
	if(a == s)
		return true;
	else if((a = 'w' && s == 's' )||(a == 'a' && s == 'd'))
		return true;
	else if((a = 's' && s == 'w' )||(a == 'd' && s == 'a'))
		return true;
	else
		return false;
}

food snakemove(food f,char snakedire,struct snake_body *head,COORD pos,HANDLE hout)
{
	struct snake_body *p;
	p=head;
	int x=head->x,y=head->y;
	switch(snakedire)
	{
	case 'w': 
		head->y -= 1;
		break;
	case 'a':
		head->x -= 2;
		break;
	case 's':
		head->y += 1;
		break;
	case 'd':
		head->x += 2;
		break;
	}
	while(head->next)
	{
		head = head->next;
		x = x^head->x;
		head->x = x^head->x;
		x = head->x^x;
		y = y^head->y;
		head->y = y^head->y;
		y = head->y^y;
	}

	if((p->x == f.x) && (p->y == f.y) )
	{
		head->next = (struct snake_body*)malloc(sizeof(struct snake_body));
		head=head->next;
		head->next = NULL;
		head->x = x;
		head->y = y;
		int xw=1;
		while(xw)
		{
			xw=0;
			f=Food();
			head=p;
			while(head)
			{
				if((head->x == f.x) && (head->y == f.y))
				{
					xw = 1;
					break;
				}
				head=head->next;
			}
		}
	}
	else
	{
		head = p;
		if(snakedie(head))
		{
			snakelife = 0;
		}
		else
		{
			pos.X=x;
			pos.Y=y;
			SetConsoleCursorPosition(hout,pos);
			cout << "  ";
		}
	}
	foodprint(f,pos,hout);
	return f;
}

bool snakedie(struct snake_body *head)
{
	struct snake_body *p=head->next;
	p=p->next;
	if(head->x == 0 || head->x == 78 || head->y == 0 || head->y == 24)
		return true;
	if(!p)
		return false;
	while(p)
	{
		if((head->x == p->x) && (head->y == p->y))
			return true;
		p=p->next;
	}
	return false;
}

void start(COORD pos,HANDLE hout)
{
	pos.X = 36;
	pos.Y = 12;
	SetConsoleCursorPosition(hout,pos);
	SetConsoleTextAttribute(hout, 0x02);
	cout << "Ready!!";
	Sleep(500);
	system("cls");
}

struct snake_body* snakefree(struct snake_body *head)
{
	struct snake_body *p;
	
	while(head)
	{
		p=head;
		head=head->next;
		free(p);
	}
	return head;
}

int main()
{
	char snakedire = 'd',atordire,ch='N';
	int speed = 0;
	snake_body *head=NULL;
	food sfood;
	HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos ={0,0};
	SetConsoleTextAttribute(hout, 0x03);
	pos.X=0;
	pos.Y=0;
	SetConsoleCursorPosition(hout,pos);
	cout<< "选择速度：\n\t1、最快\n\t2、快\n\t3、中速\n\t4、慢\n\t5、最慢\n请选择：";
	cin >> speed;
	speed = 100+speed*50;
	system("cls");
	start(pos,hout);
	head=snakestart();
	screenstart(pos,hout);
	foodprint(sfood=Food(),pos,hout);
	snakeprint(head,pos,hout);
	snakelife = 1;

	while(snakelife)
	{
		if(kbhit())
		{
			atordire = getch();
			if(atordire == snakedire)
				;
			else if((snakedire == 'd'|| snakedire == 'a') && (atordire == 'w' || atordire == 's'))
				snakedire = atordire;
			else if((snakedire == 'w'|| snakedire == 's') && (atordire == 'a' || atordire == 'd'))
				snakedire = atordire;
			else
				;
		}
		
		sfood = snakemove(sfood,snakedire,head,pos,hout);
		if(!snakelife)
			continue;
		snakeprint(head,pos,hout);
		Sleep(speed);
	}
	head=snakefree(head);
	system("cls");
	pos.X = 30;
	pos.Y = 12;
	SetConsoleCursorPosition(hout,pos);
	SetConsoleTextAttribute(hout, 0x03);
	cout << "GAME OVER !!";
	pos.X = 0;
	pos.Y = 24;
	SetConsoleCursorPosition(hout,pos);
	Sleep(1000);
	return 0;
}
