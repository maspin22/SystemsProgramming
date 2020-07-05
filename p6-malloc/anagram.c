#include <stdio.h>

int main()
{
	char str1[100];
	char str2[100];
	int flag=0;

	printf("Input first string: ");
	scanf("%s", str1);
	printf("Input second string: ");
	scanf("%s", str2);
	
	char alpha[26] = {0};
	int i = 0;

	while (str1[i] != '\0')
	{
		alpha[str1[i] - 'a'] += 1;
		i++;
	}
	
	i = 0;
	while (str2[i] != '\0')
	{
		alpha[str2[i] - 'a'] -= 1;
		i++;
	}
	
	i = 0;
	while (i<26)
	{
		if(alpha[i] != 0)
			{flag = 1; 
			break;}
		i++;
	} 


	if(flag==1)
		printf("%s is not an anagram of %s\n", str2, str1);
	else
		printf("%s is an anagram of %s\n", str2, str1);
	return 0;
}
