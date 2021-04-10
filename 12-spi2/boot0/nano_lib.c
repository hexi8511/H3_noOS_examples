#include "nano_lib.h"

void printf( char* buf)
{
	 unsigned char *pbuf=( unsigned char *)buf;

	while(*pbuf!='\0')
	{
		putc(*pbuf);
		pbuf++;
	}
}


void memset(void*buf,int val,int size)
{
	char *pbuf=(char*)buf;
	while(size--)
	{
		*pbuf=val;
		pbuf++;

	}
}

void memcpy( void *dest,void *src,int size)
{
	unsigned char* pdest=(unsigned char*)dest,*psrc=(unsigned char*)src;
	while(size--)
	{
		*pdest=*psrc;
		pdest++;
		psrc++;
	}	
}

char * strcpy(char * dest,const char *src)
{
	unsigned char* pdest=(unsigned char*)dest,*psrc=(unsigned char*)src;
	while(*psrc!=0)
	{
		*pdest=*psrc;
		pdest++;
		psrc++;
	}
	return dest;	
}

int strlen(char *str)
{
	int i=0;
	char *pstr=str;
	while((*pstr)!=0)
		{
		pstr++;
		i++;
		}
	return i;
}

