#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> 

//structure for IP addresses
struct address_t
{
	int octet[4];
	char alias[11];
	struct address_t *next;
};

//initializing the linked list
struct address_t *head = NULL;

//definitions of the functions
int  insert(struct address_t **head,struct  address_t *newIp);
int insertAddress(int a,int b,int c ,int d,char *alias,int user);
int updateAddress();
int length(struct address_t *Q);
void printIPAddresses(struct address_t *Q);
int  saveIPAddressesToList(struct address_t *Q);
struct address_t * findByAlias(struct address_t **head,char *alias);
struct address_t * findByIP(struct address_t **head,int a,int b,int c,int d);
struct address_t * findIPByLocation();

//function to find the length of the list
int length(struct address_t *Q) {
   int length = 0;
   struct address_t *current;
   for(current = Q; current != NULL; current = current->next) {
      length++;
   }	
   return length;
}
//function to upadate ip address for the given alias name
int updateAddress(){
	char alias[11];
	printf("Enter alias name:");
	scanf("%s",alias);
	//checking for duplicate alias name
	struct address_t *add=findByAlias(&head,alias);
	if(add==NULL) {
		printf("%s does not exists\n" ,alias);	
		return -1;
	}
	int loc[4];
	for(int i=0;i<4;i++){
		while(1){
			printf("enter address locataion # %d\n",i+1);
			scanf("%d",&loc[i]);
			if(loc[i]>=0 && loc[i]<=255){
				break;
			}
			else{
				printf("illegal entry\n");
			}
		}
	}
	//checking if ip address already exist in the list
	struct address_t *add2=findByIP(&head,loc[0],loc[1],loc[2],loc[3]);
	if(findByIP(&head,loc[0],loc[1],loc[2],loc[3])!=NULL) {
		//if ip address is already present we cannot use it again 
		printf("ip Address already exists in the list\n");
		return -1;
	}
	for(int i=0;i<4;i++){
		add->octet[i]=loc[i];
	}
return 0;	
}

//printing the list
void printIPAddresses(struct address_t *Q) {
   struct address_t *ptr = Q;
   int l=0;
   if(ptr==NULL){
   	printf("the list is empty\n");
   	return ;
   }
   //printing the whole list with IP Address and Alias name
   while(ptr != NULL) {
      printf("%d.%d.%d.%d %s\n",ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3],ptr->alias);
      l++;
      ptr = ptr->next;
   }	
  printf("Total nodes:%d \n",l);
}

//creating new node and adding the new IP address to the list
int insertAddress(int a,int b,int c ,int d,char *alias,int user){
	if(user==1){
		printf("Enter Alias Name:");
		scanf("%s",alias);
		struct address_t *t;
		t=findByAlias(&head,alias);
		if(t!=NULL) {
			printf("Alias Name %s already exists in the list\n" ,alias);	
			return 0;
		}
		printf("\n");
		char ip[25];
		char dot;
		printf("Enter address for %s: ",alias);
		scanf("%s",ip);
		sscanf(ip,"%d%c%d%c%d%c%d", &a,&dot,&b,&dot,&c,&dot,&d);
		if(findByIP(&head,a,b,c,d)!=NULL) {
			printf("ip Address already exists\n");	
			return 0;
		}
	}
	if(a<0 || a>255){
		printf("ERROR: octet must be in range of 0 to 255\n");
		return -1;		 	
 	}
 	if(b<0 || b>255){
 		printf("ERROR: octet must be in range of 0 to 255\n");
		return -1;	
 	}
 	if(c<0 || c>255){
 		printf("ERROR: octet must be in range of 0 to 255\n");
		return -1;	
 	}
 	if(d<0 || d>255){
 		printf("ERROR: octet must be in range of 0 to 255\n");
		return -1;	
 	}
	// creating node from the passed data  	
    struct address_t *newIp = malloc(sizeof(struct address_t));
	if(newIp==NULL){
		printf("ERROR: memory allocation for new node failed\n");
		return -1;
	}
	strcpy(newIp->alias,alias);	
	newIp->octet[0]=a;
	newIp->octet[1]=b;
	newIp->octet[2]=c;
	newIp->octet[3]=d;
	newIp->next=NULL;
	// inserting node 
	if(insert(&head,newIp)==-1){
		free(newIp);
	}
return 0;	
}

//save the output list to file
int  saveIPAddressesToList(struct address_t *Q) {
   char filename[100];
   struct address_t *ptr = Q;
   printf("Enter file name to save: ");
   scanf("%s",filename);
    FILE *fptr=fopen(filename,"w");
   if(fptr==NULL){
   	printf("cannot open output file\n");
   	return -1;
   }
   if(ptr==NULL){
   	printf("list is empty\n");
   	return -1;
   }
   while(ptr != NULL) {
      fprintf(fptr,"%d.%d.%d.%d %s\n",ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3],ptr->alias);
      ptr = ptr->next;
   }
   fclose(fptr);	
 return 0;
}

// read the input file
int read_file(char *file_path){
	char ip[15];
	char alias[32];
	int a , b,c,d;
	char dot;
	FILE *fptr=fopen(file_path,"r");
	if(fptr==NULL){
		printf("ERROR: file cannot be opened\n");
		return -1;
	}	
 	while (!feof(fptr)){
	 	fscanf(fptr,"%d%c%d%c%d%c%d %s", &a,&dot,&b,&dot,&c,&dot,&d,alias);
 	 	if(strlen(alias)!=0){
			int r=insertAddress(a,b,c,d,alias,0); 	 		
 	 	}	 	
	 	strcpy(ip,"");
	 	strcpy(alias,"");
 	}
 	fclose(fptr);
return 0;
}

//insert IP addresses to the list	
int  insert(struct address_t **head,struct  address_t *newIp) {
	struct address_t *temp=*head;	
	//Initializing HEAD
	if(*head==NULL){
		*head=newIp;
	}
    //finding empty location and inserting
	else{
		while(temp->next!=NULL ){
			temp=temp->next;		
		}
		temp->next=newIp;  
	}
return 0;	
}

//function to compare the alias names
int aliasComparision(char *s1,char *s2,int l1,int l2){
	if(l1!=l2){
		return 1;
	}
	for(int i=0;i<l1;i++){
		char a=toupper(s1[i]);
		char b=toupper(s2[i]);
		if( a!=b){
			return 1;
		}
	}
	return 0;
}


//delete IP for the given alias
void deleteIPAddress() {
	char alias[11];
	printf("Enter alias Name:");
	scanf("%s",alias);
	struct address_t *current=head;
   	struct address_t* previous = NULL;
   	if(head == NULL) {
   		printf("list is empty\n");
      		return ;
   	}	
   	int l1,l2;
   	l1=strlen(current->alias);
   	l2=strlen(alias);
   	while(aliasComparision(current->alias , alias,l1,l2)!=0) {
      		if(current->next == NULL) {
      			printf("%s does not exist\n",alias);
        		return ;
      		} 
		else {
         		previous = current;
         		current = current->next;
         		l1=strlen(current->alias);
      		}
   	}
   	char conf;
   	int a=current->octet[0];
   	int b=current->octet[1];
   	int c=current->octet[2];
   	int d=current->octet[3];
	printf("Do you want to delete %s %d.%d.%d.%d choose yes(y) or no(n)?\n",alias,a,b,c,d);
	scanf(" %c",&conf);
	if(conf=='n'){
		return;
	}
   	//if the node to be deleted is at start
   	if(current == head) {
      		head = (head)->next;
		
	} //if its in middle
	else {
      		//bypass the current link
      		previous->next = current->next;
			
   	}
   	// delete the node
	free(current);
}

//find the IP address
struct address_t * findByAlias(struct address_t **head,char *alias) {
	struct address_t *current=*head;	
   	if(*head == NULL) {
      		return NULL;
   	}
   	int l1,l2;
   	l1=strlen(current->alias);
   	l2=strlen(alias);
   	while(aliasComparision(current->alias , alias,l1,l2)!=0) {
      		if(current->next == NULL) {
        		return NULL;
      		} 
		else {
         		current = current->next;
         		l1=strlen(current->alias);
   			l2=strlen(alias);
      		}
   	}
	return current;	
}

//find the IP Address from the list by using Alias name
struct address_t * findByIP(struct address_t **head,int a,int b,int c,int d) {
	struct address_t *current=*head;	
   	if(*head == NULL) {
      		return NULL;
   	}
   	while(!(current->octet[0]==a && current->octet[1]==b && current->octet[2]==c && current->octet[3]==d)) {
      		if(current->next == NULL) {
        		return NULL;
      		} 
		else {
         		current = current->next;
      		}
   	}
	return current;	
}


//find ip address by location address
struct address_t * findIPByLocation() {
	int a;
	int b;
	int loc[2];
	for(int i=0;i<2;i++){
		while(1){
			printf("enter the address locataion to search # %d\n",i+1);
			scanf("%d",&loc[i]);
			if(loc[i]>=0 && loc[i]<=255){
				break;
			}
			else{
				printf("ERROR: illegal entry\n");
			}
		}
	}
	struct address_t *current=head;
	struct address_t *find=NULL;	
   	if(head == NULL) {
   		printf("list is empty\n");
      		return NULL;
   	}
	int flag=0;
   	while(current!=NULL) {
      		if(current->octet[0] == loc[0] && current->octet[1] == loc[1]){
      			flag=1;	
      			int a=current->octet[0];
      			int b=current->octet[1];
      			int c=current->octet[2];
      			int d=current->octet[3];
			    printf("%d.%d.%d.%d %s\n",a,b,c,d,current->alias);		
      			find =current;	
      		}      		
 		current = current->next;		
   	}
   	if(!flag)
	printf("no matching address location found\n");
	return find;	
}


int main(){
	int choice;
	char alias[11];
	int a ,b,c,d;
	int r= read_file("./cs531_inet.txt");
	if(r==-1){
		printf("terminating\n");
		return 1;
	}
	while(1){
		printf("\n*********************************************\n");
		printf("Please select an option from the below:\n");
		printf("1. Add address\n");
		printf("2. Look up address\n");
		printf("3. Update address\n");
		printf("4. Delete address\n");
		printf("5. Display list\n");
		printf("6. Display aliases for location\n");
		printf("7. Save to file\n");
		printf("8. Quit\n");
		printf("*********************************************\n");
		printf("Enter your selection: ");
		scanf("%d",&choice);
		switch(choice){
			case 1: {
				insertAddress(a, b,c ,d,alias,1);
				break;		
			}
			case 2: {
				printf("Enter the alias Name:");
				scanf("%s",alias);
				//duplicacy check for alias names
				struct address_t *add=findByAlias(&head,alias);
				if(add!=NULL) {
					a=add->octet[0];
					b=add->octet[1];
					c=add->octet[2];
					d=add->octet[3];
					printf("IP Address with alias name %s : %d.%d.%d.%d \n",add->alias,a,b,c,d);			
				}else{
					printf("alias name %s does not exists\n" ,alias);
				}
				break;		
			}
			case 3: {
				updateAddress();
				break;		
			}
			case 4: {
				deleteIPAddress();
				break;			
			}case 5: {
				printIPAddresses(head);
				break;		
			}
			case 6: {		
				findIPByLocation(); 
				break;	
			}
			case 7: {
				saveIPAddressesToList(head);
				break;	
			}
			case 8: {
				printf("Thank You!\n");
				return 0;	
				break;		
			}
			default: {	
				printf("ERROR: invalid selection \n");		
				break;		
			}
		}
	}
return 0;
}