#include<stdio.h>
#include<stdlib.h>
int main()
{
	FILE *fptr;
	fptr=fopen("fileRead.txt","r");
	char c[100];
	if(fptr==NULL)
	{
		printf("Cannot open file\n");
		exit(0);
	}
	while(fgets(c,60,fptr)!=NULL)
	{
		if(c[0]=='#')
			continue;
		printf("%s",c);
	}
	fclose(fptr);
	return 0;
}
