/* 
 * File:   main.cpp
 * Author: Brynn Caddel
 * Assignment: C++ Assignment 2 - Spring 2015 43950
 * Created on March 28, 2015, 5:14 PM
 */

//System Libraries
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

//User Libraries

//Global Constants

//Function Prototypes
void menu(); //Main menu function
void clrscrn(); //Clear Screen function
short slct(); //menu selection
bool again(); //problem repeater
void prnt(int *,int,int);//prints dynamic array, perline amount
void prbavg();//mean, median, and mode calculation problem
void prb92();//Driver for problem 9.2, Test Scores #1
void prb93();//driver for problem 9.3, Drop Lowest Score
void prb95();//driver for problem 9.5, Pointer Rewrite
void prb910();//driver for problem 9.10, Reverse Array
void prb911();//driver for problem 9.11, Array Expander
int *scores();//Creates dynamic array for test scores
float savg(int *);//calculates average score
void prntscr(int *);//outputs score array with headers
int *mode(int[],int); //Mode determining function
float median(int[],int); //median determining function
float mean(int[],int); //Mean determining function
void bubSort(int *, int);//bubsort for dynamic array with size
void bubSort(int *);//bubble sort for dynamic array with size in array
int *curve(int *);//drops the lowest score
int tenSwap(int *, int *);//swaps the and multiplies both by 10
int *reverse(int[],int);//reverses the order of an int array
int *expand(int[],int);//expands the given array


//Let the games begin!
int main(int argc, char** argv) {
    srand(time(0));
    menu();

   
 
    return 0;
}

//Menu function, displays all assignments and prompts for which
//the user would like to go to.
void menu(){
    
    clrscrn(); //clears the screen of all text
    short pick = slct();
    switch(pick){
        case 1:
            clrscrn();
            prbavg();
            break;
        case 2:
            clrscrn();
            prb92();
            break;
        case 3:
            clrscrn();
            prb93();
            break;
        case 4:
            clrscrn();
            prb95();
            break;
        case 5:
            clrscrn();
            prb910();
            break;
        case 6:
            clrscrn();
            prb911();
            break;
        case -1:
            cout<<"That's all, folks."<<endl;
        default:
            cout<<"Until next time."<<endl;
    }
}//end

//Clear screen function outputs a ton of new lines in order to clear
//the command prompt to look nice
void clrscrn(){
    for(int i=0; i<100; i++)
        cout<<endl;
}//end

//slct serves to take in input for menu selection, performs error checks
//and then returns the value if it passes checks
short slct(){
    short pick; //for menu selection
    bool check=false;
    cout<<"Assignment 2"<<endl;
    cout<<"Choose an option from the menu: "<<endl
            <<"1. Median, Mean, and Mode"<<endl
            <<"2. Problem 9.2 (Test Scores #1)"<<endl
            <<"3. Problem 9.3 (Drop Lowest Score)"<<endl
            <<"4. Problem 9.5 (Pointer Rewrite)"<<endl
            <<"5. Problem 9.10 (Reverse Array)"<<endl
            <<"6. Problem 9.11 (Expand Array)"<<endl
            <<"-1 to quit"<<endl;
    do{
        cin>>pick;
        if(cin.fail()||pick<=0&&pick!=-1||pick>6){//error checking
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Invalid selection. Try again."<<endl;
        }
        else
            check=true;//valid input
    }while(!check);
    return pick;
}//end
//again prompts the user to see if they want to run the same problem again
//and returns to menu if not
bool again(){
    bool check=false;
    char pick;
    cout<<"Would you like to run this problem again? y/n"<<endl;
    do{
        cin>>pick;
        if(cin.fail()||tolower(pick)!='y'&&tolower(pick)!='n'){//only accepts
            cin.clear();                                       //y or n as input
            cin.ignore(256,'\n');
            cout<<"Error. Invalid selection. Try again."<<endl;
        }
        else if(tolower(pick)=='y'){//user wants to repeat
            check=true;
            return true;
        }
        else{ //user does not want to repeat
            check=true;
        return false;
        }
    }while(!check);
    
}//end
//prnt prints the contents of a dynamic array formatted to
//the desired line width
void prnt(int *a,int size,int perLine){
    for(int i=0;i<size;i++){
        cout<<a[i]<<" ";
        if(i%perLine==(perLine-1))cout<<endl;
    }
}
void prbavg(){
    clrscrn();
    do{
        int n=19;
        int pl=10;
        int a[n];
        for(int i=0;i<n;i++)
            a[i]=rand()%9+1;
        int *m=mode(a,n);
        cout<<"Randomly determined values"<<endl;
        prnt(a,n,pl);
        cout<<endl<<"The "<<m[0]<<" mode(s) are:"<<endl;
        for(int i=2;i<(m[0]+2);i++){//prints m from the start of mode values
            cout<<m[i]<<" ";
        }
        cout<<endl<<"With a frequency of "<<m[1]<<endl;
        cout<<endl;
        cout<<"The median is: "<<setprecision(1)<<fixed<<median(a,n)<<endl;
        cout<<"The mean is: "<<mean(a,n)<<endl;
    }while(again());
    menu();
    
}
int *mode(int a[],int n){
    //sorts the array
    bubSort(a,n);
    int val=a[0];//for tracking which values are modes
    int count=0, //counts occurrences of element in array
            freq=0, //tracks the highest frequency found in array so far
            numMode=1; //tracks the number of elements with frequency found
    for(int i=0;i<=n;i++){
        if(a[i]==val)
            count++;
        else{
            if(count>freq){//a[i] occurs more than val
                freq=count;
                count=1;
                numMode=1;
                val=a[i];
            }
            else if(count==freq){//a[i] occurs same amount as val
                numMode++;
                val=a[i];
                count=1;
            }
            else{//a[i] occurs less than val
                val=a[i];
                count=1;
            }
        }
    }
    int *m=new int[(numMode+2)];//+2 for # of modes and frequency
    m[0]=numMode;
    m[1]=freq;
    count=0;//compared to freq to find modes
    int index=2;//moves along m to store every mode found
    val=a[0];
    for(int i=0;i<n;i++){
        if(a[i]==val)
            count++;
        if(count==freq){
            m[index]=val; //adds val as a mode
            index++; //bumps to next slot in mode array
            val=a[i+1];
            count=0;
        }
        if(a[i]!=val){//start examining next value in array
            count=1;//value at i counts as 1
            val=a[i];
        }
}
    //end
    return m;
}//end
//median calculates the median of a sample of values
//and returns it as a float
float median(int a[],int n){
    float med=0.0f;
    for(int i=0;i<n;i++)
        med+=a[i];
    med=med/static_cast<float>(n);
    return med;
}//end
//mean calculates the mean value of a sample of values
//and returns it as a float
float mean(int a[],int n){
    float m=0.0;
    int mid=n/2;
    if(n%2==0){//even value so no clear middle value
        m=(a[mid]+a[mid+1])/2.0;//takes the average of the two "mids"
    }
    else
        m=a[mid]*1.0;
    return m;
}
void bubSort(int *a,int size){
    for(int i=0; i<size; i++)
    {
        for (int j=0; j<size-1;j++)
        {
            if(a[j]>a[j+1])
                swap(a[j],a[j+1]);
        }
    }
}//end
//scores prompts user to enter number of tests and their scores,
//and returns it as a dynamic array of the appropriate size
int *scores(){
    int s;//for storing the size
    int n;//for storing each score into array
    bool check=false;
    cout<<"Enter the number of test scores"<<endl;
    do{
        cin>>s;
        //error checking
        if(cin.fail()||s<=0){
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Must be positive, nonzero integer."<<endl;
        }
        else
            check=true;
    }while(!check);
    int *t=new int[s+1];//number of scores plus 1 for size of array
    *t=s+1;//can call t[0] to access size of t
    //loop 's' amount of times to fill array with grades from user
    for(int i=1;i<*t;i++){
        check=false;
        cout<<"Enter grade #"<<i<<endl;
        do{
            cin>>n;
            //error checking
            if(cin.fail()||n<0){
                cin.clear();
                cin.ignore(256,'\n');
                cout<<"Error. Must be positive, nonzero integer."<<endl;
            }
            else
                check=true;
        }while(!check);
        *(t+i)=n;
    }
    //finished
    return t;
}//end
void bubSort(int *a){
    for(int i=1; i<*a; i++)//a must contain the size of the array to work
    {
        for (int j=1; j<*a-1;j++)
        {
            if(*(a+j)>*(a+j+1))
                swap(*(a+j),*(a+j+1));
        }
    }
}//end
//savg calculates the average score from an array of test scores and
//returns the value as a float
float savg(int *a){
    float avg=0.0f;
    for(int i=1;i<*a;i++)
        avg+=*(a+i);
    avg=avg/((*a)-1);//actual amount of scores is one less than size
    return avg;
}//end
//prntscr prints the contents of the score array
//taking into consideration the presence of the size
//and also prints the average score
void prntscr(int *a){
    //print the number of scores being printed
    cout<<"The results from "<<*a-1<<" tests"<<endl;
    //print the test scores, labeled by number
    for(int i=1;i<*a;i++){
        cout<<"Score #"<<i<<": "<<setw(4)<<right<<*(a+i)<<endl;
    }
    cout<<endl<<"Average Score: "<<savg(a)<<endl;
}//end
void prb92(){
    do{
        int *tests=scores();
        bubSort(tests);
        clrscrn();
        prntscr(tests);
        delete []tests;
        delete tests;
    }while(again());
    menu();
}//end
void prb93(){
    do{
        int *tests=scores();
        bubSort(tests);
        clrscrn();
        prntscr(tests);
        cout<<endl;
        cout<<"After dropping the lowest score(s):"<<endl;
        int *c=curve(tests);
        prntscr(c);
        delete []tests;
        delete []c;
        delete tests;
        delete c;
    }while(again());
    menu();
}//end
void prb95(){
    do{
        int x, y;
        cout<<"Enter an integer for x"<<endl;
        cin>>x;
        cout<<"Enter another integer for y"<<endl;
        cin>>y;
        clrscrn();
        cout<<"Before:"<<endl;
        cout<<"x: "<<setw(3)<<right<<x<<" "<<"y: "<<setw(3)<<right<<y<<endl;
        cout<<endl<<"x & y will be swapped, multiplied by 10, and added."<<endl;
        cout<<"10(x+y)="<<tenSwap(&x,&y)<<endl;
        cout<<endl<<"After:"<<endl;
        cout<<"x: "<<setw(3)<<right<<x<<" "<<"y: "<<setw(3)<<right<<y<<endl;
    }while(again());
    menu();
}
void prb910(){
    do{
        int s=10;
        int array[10]={};
        for(int i=0;i<s;i++)
            array[i]=rand()%90+10;
        cout<<"The original array:"<<endl;
        prnt(array,s,10);
        cout<<endl;
        int *brray=reverse(array,s);
        cout<<"The reversed array:"<<endl;
        prnt(brray,s,10);
    }while(again());
    menu();

}//end
void prb911(){
    do{
    int a[10]={};
    for(int i=0;i<10;i++)
        a[i]=rand()%90+10;
    cout<<"The original array:"<<endl;
    prnt(a,10,10);
    int *b=expand(a,10);
    cout<<"The expanded array:"<<endl;
    prnt(b+1,b[0]-1,10);
    }while(again());
    menu();
}
//curve determines the lowest score from s and copies over every value
//except the lowest
int *curve(int *s){
    //determines lowest value
    int low=1000000;
    for(int i=1;i<*s;i++){
        if(*(s+i)<low)
            low=*(s+i);
    }
    //determines how many of the lowest is in the array
    int count=0;
    for(int i=1;i<*s;i++){
        if(*(s+i)==low)
            count++;
    }
    //copies over every element in s except the lowest values
    int *c=new int[*s-count];
    *c=(*s-count);
    count=0;
    for(int i=1;i<*s;i++){
        if(*(s+i)==low)
            count++;
        else
            *(c+i-count)=*(s+i);
    }
    return c;
}//end
//tenSwap swaps the values of x and y and multiples them by 10
//and returns the sum of the two... who knows why.
int tenSwap(int *x, int *y){
    int temp=*x;
    *x=*y*10;
    *y=temp*10;
    return *x+*y;
}//end
//reverse takes an integer array and reverses the order of the elements
//returns a pointer to the new reversed array
int *reverse(int a[],int size){
    int *r=new int[size];
    for(int i=0;i<size;i++)
        r[i]=a[size-1-i];
    return r;
}//end
//expand takes the given array and dynamically creates a new array of
//double size, copies the elements, and initializes new memory as 0
int *expand(int a[], int size){
    int *b=new int[(2*size)+1];
    b[0]=(2*size)+1;
    for(int i=1;i<(2*size)+1;i++)
        b[i]=0;
    for(int i=0;i<size;i++)
        b[i+1]=a[i];
    return b;
}