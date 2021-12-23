/*
G01337128
Yaswanth Reddy Manukonda
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//boolean flag used to handle error messages
bool ERROR;
//array of special characters that are not allowed
char splChars[] = {'#', '$', '%', '^', '@', '(', ')'};
//input character strings array
char inputStrings[10][25];
//temperoray char array, used for sorting
char temp[10];
//boolean flag used for validating the input strings
bool validated = false;
//flag used for conditions handling in input strings validation
bool res = true;
//flag used to check if A or D is only entered when asked for sorting type
bool correct = false;
//used to store sorting type 
char sortingType;

//size of the input character strings array
int sizeOfInputArray = sizeof inputStrings / sizeof inputStrings[0];

//function used to validate the input strings
bool validateInput(char input[][25], int i)
{
     if (input[i][0] == '\n'){
          printf("ERROR: string %d is empty, empty strings are not allowed\n", i);
          return false;
     }
     if (strlen(input[i]) - 1 > 25 || strlen(input[i]) - 1 < 1){
          printf("ERROR: The size of a character string should be in between 1 to 25. Your input string length is %lu\n", strlen(input[i]) - 1);
          return false;
     }
     for (int j = 1; j <= i - 1; j++){
          if (strcmp(inputStrings[j], input[i]) == 0){
               printf("ERROR: duplicate string found\n");
               return false;
          }
     }
     int count = 0;
     for (int j = 0; j < strlen(input[i]) - 1; j++){
          for (int k = 0; k < strlen(splChars) - 1; k++){
               if (input[i][j] == splChars[k]){
                    count++;
               }
               else{
                    res = true;
                    continue;
               }
          }
     }
     if (count == 1){
          printf("ERROR: %d character in the input string #%d are not allowed\n", count, i);
          res = false;
     }
     if (count > 1){
          printf("ERROR: %d characters in the input string #%d are not allowed\n", count, i);
          res = false;
     }
     return res;
}

//function used to validate the sorting type entered (only characters A or D is allowed)
bool checkSortingType(char sortingType)
{
     if (sortingType == 'A' || sortingType == 'D')
     {
          return true;
     }
     else
     {
          printf("ERROR: only A or D is allowed\n");
          return false;
     }
}

//function used to print the sorted input strings array in asceding order
void sortAscending(char input[][25], int n){
     printf("Array after ascending order:\n");
     for (int i = 1; i <= n; i++)
     {
          printf("%s", input[i]);
     }
}

//function used to print the sorted input strings array in descending order
void sortDescending(char input[][25], int n){
     printf("Array after descending order:\n");
     for (int i = n; i >= 1; i--)
     {
          printf("%s", input[i]);
     }
}

//main method
int main(){
     printf("====Start of the program==== \n");
     for (int i = 1; i <= sizeOfInputArray; i++){
          do{
               printf("Enter the input string #%d: \n", i);
               fgets(inputStrings[i], sizeof inputStrings, stdin);
               validated = validateInput(inputStrings, i);
          } while (validated == false);
     }
     printf("How do you like to sort the input character strings? \nenter A for ascending\nenter D for descencing\n");
     do{
          correct = checkSortingType(sortingType);
     } while (correct == false);

     //Sorting the input character strings array
     for (int i = 1; i <= sizeOfInputArray; i++){
          for (int j = 0; j <= sizeOfInputArray - i; j++){
               if (strcmp(inputStrings[j], inputStrings[j + 1]) > 0){
                    strcpy(temp, inputStrings[j]);
                    strcpy(inputStrings[j], inputStrings[j + 1]);
                    strcpy(inputStrings[j + 1], temp);
               }
          }
     }

     //The output printing is handled with switch, based on the sorting type user asked
     switch (sortingType)
     {
     case 'A':
          sortAscending(inputStrings, sizeOfInputArray);
          break;
     case 'D':
          sortDescending(inputStrings, sizeOfInputArray);
          break;
     default:
          break;
     }
     printf("Input string with Lowest ASCII value is %s\n",inputStrings[1]);
     printf("Input string with Highest ASCII value is %s\n",inputStrings[9]);
     printf("====End of the program==== \n");
}

