#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 1000
int main(int argc, char*argv[]){
	int i = 0;
	int j = 0;
	int carry = 0;
	int barrow = 0;
	int bn0[MAX]={0};
	int bn1[MAX]={0};
	int sum[MAX]={0};
	//warning line
	if(argc!=4){
		printf("Please using command  %s [integer1] [op] [integer2]\n",argv[0]);
		exit(1);
	}
	//Get Num1 into array~
	if(argv[1][0]=='-'){
		
		for(i=0,j=strlen(argv[1])-1 ; j>0 ; i++,j--){
			bn0[i]=9-(argv[1][j]-'0');}
		//sign extension
		for(;i<=MAX;i++){bn0[i]=9;}
		
		bn0[0] = bn0[0]+1;
		
		for(i=0;i<=MAX-1;i++){
			if(bn0[i]>=10){
				bn0[i+1] += bn0[i]/10;
				bn0[i] = bn0[i]%10;
			}
		}
	}
	else{
		for(i=0,j=strlen(argv[1])-1 ; j>=0 ; i++,j--){
			bn0[i]=argv[1][j]-'0';
		}
	}
	//Get Num2 into array~
	if(argv[3][0]=='-'){
		
		for(i=0,j=strlen(argv[3])-1 ; j>0 ; i++,j--){
			bn1[i]=9-(argv[3][j]-'0');}
		//sign extension
		for(;i<=MAX;i++){bn1[i]=9;}
		//above we get the 9's complement
		bn1[0] = bn1[0]+1;//obtain 10's complement
		
		for(i=0;i<=MAX-1;i++){
			if(bn1[i]>=10){
				bn1[i+1] += bn1[i]/10;
				bn1[i] = bn1[i]%10;
			}
		}
	}
	else{	
		for(i=0,j=strlen(argv[3])-1 ; j>=0 ; i++,j--){
			bn1[i]=argv[3][j]-'0';
		}
	}
	//show the number
	/*
	printf("number 1 : "); 
	for(i=0;i<20;i++){
		printf("%d",bn0[i]);}
	printf("\n");
	//show the number
	printf("number 2 : "); 
	for(i=0;i<20;i++){
		printf("%d",bn1[i]);}
	printf("\n");
	*/
	//big num adder
	if(argv[2][0]=='+'){
		printf("----Add----\n");
		
		for(i=0;i<=MAX;i++){
			sum[i]=bn0[i]+bn1[i]+carry;
			carry = 0;
                        if(sum[i]>=10){
                                carry += sum[i]/10;
                                sum[i] = sum[i]%10;
                        }
                }
		if(sum[MAX-1]==0){
			for(i=MAX-1;i>=0;i--){
				if(sum[i]!=0)break;
			}
			printf("Add Ans : ");
			for(;i>=0;i--){
				printf("%d",sum[i]);
			}
			printf("\n");
		}
		else{
			for(i=0;i<MAX;i++){
				sum[i]=9-sum[i];
			}
			sum[0]+=1;
			for(i=0;i<MAX-1;i++){
				if(sum[i]>=10){
					sum[i+1] += sum[i]/10;
					sum[i] = sum[i]%10;
				}
			}

			for(i=MAX-1;i>=0;i--){
				if(sum[i]!=0)break;
			}
			printf("Add Ans : ");
			printf("-");
			for(;i>=0;i--){
				printf("%d",sum[i]);
			}
        		printf("\n");
	
		}
	}
	//bignum Subtraction
	if(argv[2][0]=='-'){
		printf("----Sub----\n");
		for(i=0;i<=MAX;i++){
                        sum[i]=bn0[i]-bn1[i]+barrow;
                        barrow = 0;
                        if(sum[i]<0){
                                barrow = -1;
                                sum[i] = sum[i]+10;
                        }
                }
		if(sum[MAX-1]==0){
			for(i=MAX-1;i>=0;i--){
				if(sum[i]!=0)break;
			}
			printf("Sub Ans : ");
			for(;i>=0;i--){
				printf("%d",sum[i]);
			}
			printf("\n");}
		else{	
			for(i=0;i<MAX;i++){
				sum[i]=9-sum[i];
			}
			sum[0]+=1;
			for(i=0;i<MAX-1;i++){
				if(sum[i]>=10){
					sum[i+1] += sum[i]/10;
					sum[i] = sum[i]%10;
				}
			}
			for(i=MAX-1;i>=0;i--){
				if(sum[i]!=0)break;}
			printf("Sub Ans : -");
			for(;i>=0;i--){
				printf("%d",sum[i]);}
			printf("\n");
		}
	}

	return 0;
}
