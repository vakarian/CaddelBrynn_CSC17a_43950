/* 
 * File:   main.cpp
 * Author: Brynn
 *
 * Created on April 1, 2015, 11:44 AM
 */

/* 
 * file: main.cpp
 * Author: Brynn Caddel
 * Assignment: C++ Assignment 1 Review - Spring 2015 43950
 * Created on March 15, 2015, 11:53 AM
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

//global constants
const float YEN_PER_DOLLAR=121.39f;//conversion rate for USD to Yen
const float EUROS_PER_DOLLAR=0.95f;//conversion rate for USD to Euros
const int MONTHS=3;//number of months for problem 7.06
const int DAYS=30;//number of days for problem 7.06

//Function prototypes
void menu(); //Main menu function
void clrscrn(); //Clear Screen function
short slct(); //menu selection
void prb312(); //driver for problem 3.12
void prb313(); //driver for problem 3.13
void prb410(); //driver for problem 4.10
void prb511(); //driver for problem 5.11
void prb607();//driver for problem 6.07
void prb706();//driver for problem 7.06
void prb807();//driver for problem 8.07
float getcels(); //gets input from user for temperature
float ctof(float);//converts celsius to fahrenehit
bool again(); //problem repeater
int dollars(); //prompts user for integer dollar amount and returns it
float yen(int); //converts usd to yen
float euro(int); //converts usd to euro
int month(); // returns integer for month 1-12
int year(); //returns integer for year
bool leap(int); //checks for leap year
void days(int, int); //output numbers of day of a given year and month
void getpop(float&,int&,int&); //function to gather population input
void incpop(float&,int,int); //function to perform population calculations
float celsius(int);// fahrenheit to celsius
void clstbl();//outputs a formatting table of fahrenheit to celsius conversions
void getWthr(char[][DAYS]);//reads from weather file
void cntWthr(char[][DAYS],int[][MONTHS]);//counts rainy, cloud, sunny days
void statWthr(int[][MONTHS]);//organizes and outputs the results of cntWthr
int rainy(int[][MONTHS]);//finds the month with most rain
void bubString(string[],int);//bubble sort for string array
int binString(string[],int,string);//binary search for string array
void output(string[],int);//outputs string array


//main
int main(int argc, char** argv) {
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
            clrscrn(); //clears screen before executing first problem
            prb312(); //calls the driver for problem 3.12
            break;
        case 2:
            clrscrn();
            prb313();
            break;
        case 3:
            clrscrn();
            prb410();
            break;
        case 4:
            clrscrn;
            prb511();
            break;
        case 5:
            clrscrn();
            prb607();
            break;
        case 6:
            clrscrn();
            prb706();
            break;
        case 7:
            clrscrn();
            prb807();
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
    cout<<"Assignment 1 Review"<<endl;
    cout<<"Choose an option from the menu: "<<endl
            <<"1. Problem 3.12 (Celsius to Fahrenheit)"<<endl
            <<"2. Problem 3.13 (Currency)"<<endl
            <<"3. Problem 4.10 (Days in a  Month)"<<endl
            <<"4. Problem 5.11 (Population)"<<endl
            <<"5. Problem 6.07 (Celsius Temperature Table)"<<endl
            <<"6. Problem 7.06 (Rain or Shine)"<<endl
            <<"7. Problem 8.07 (Binary String Search)"<<endl
            <<"-1 to quit"<<endl;
    do{
        cin>>pick;
        if(cin.fail()||pick<=0&&pick!=-1||pick>7){//error checking
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Invalid selection. Try again."<<endl;
        }
        else
            check=true;//valid input
    }while(!check);
    return pick;
}//end

//prb312 is the driver for problem 3.12
void prb312(){
    do{
        clrscrn();//clears console
        float c=getcels(); //calls celsius to get celsius temp from user
        float t=ctof(c);//converts c to fahrenheit
        cout<<c<<" degrees celsius is "<<
                setprecision(1)<<fixed<<t<<" degrees fahrenheit."<<endl;
    }while(again()); //repeats process if true, goes to menu if false
    menu();
}//end
//prb 313 is the main driver for problem 3.13
void prb313(){
    do{
        clrscrn();//clears console
        int usd=dollars(); //USD amount
        float e=euro(usd); //convers to euros
        float y=yen(usd); //converts to yen
        cout<<"$"<<usd<<setprecision(2)<<fixed//outputs to 2 decimal places
                <<" is "<<e<<" euros and "<<y<<" yen."<<endl;
    }while(again()); //repeats process if true, goes to menu if false
    menu();    
}//end
//prb410 is the main driver for problem 4.10
void prb410(){
    do{
        clrscrn();//clears console
        int m=month();//gets month
        int y=year();//gets year
        days(m,y);//displays days
    }while(again());//checks for repeat
    menu();//goes back to menu

}//end
//prb511 is the driver for problem 5.11, population.
void prb511(){
    int r, d;
    float p;
    do{
        clrscrn();
        getpop(p,r,d);//gets population information from user
        incpop(p,r,d);//performs population growth calculations
    }while(again());
    menu();
}//end
//prb 607 is the main driver for problem 6.7, celsius temperature table
void prb607(){
    do{
        clrscrn();
       short pick;
       bool check=false;
       clstbl();
    }while(again());
    menu();
}//end
//prb706 is the main driver for problem 7.6, rainy days
void prb706(){
    do{
        clrscrn();
        char w[MONTHS][DAYS]={};//stores weather from each day of month
        int l[MONTHS][MONTHS]={};//occurrences of each weather type
        getWthr(w);
        cntWthr(w,l);
        statWthr(l);    
    }while(again());
    menu();
}//end
//prb807 is the main driver for problem 8.7, binary string search
void prb807(){
    do{
        clrscrn();
        string n[20]={"Joseph","Brynn","Adam","Patrick","Nornu","Briana",
                        "Ricardo","Alex","Terry","Jason","Gabriela","Mark",
                        "Cody","Josh","Marlene","Phillip","Samuel","Jeff",
                        "Barack","Elon"};//Example array
        string name;
        cout<<"Before Sorting:" <<endl;
        output(n,20);//outputs the unsorted string array from above
        bubString(n,20);//sorts the string array using bubble sort
        cout<<endl<<"After Sorting: "<<endl;
        output(n,20);//outputs the sorted array
        cout<<endl
            <<"Choose a name to determine location "
                "in list (left-right,top-bottom)"
            <<endl;
        cin>>name;//for demonstrating the binary search for strings
        if(binString(n,20,name)==-1)//name not found by search
            cout<<"That name is not on the list."<<endl;
        else{//name found by search
            cout<<name<<" is number "<<binString(n,20,name)+1
                    <<" on the list."<< endl;
        }
    }while(again());
    menu();

}
//celsius prompts user to enter a temperature for conversion
float getcels(){
    bool check = false;
    float num; //stores user inputted value
    cout<<"Enter a temperature in celsius to convert to fahrenheit."<<endl;
    do{
        cin>>num;
        if(cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Invalid selection. Try again."<<endl;
        }
        else
            check=true;
    }while(!check);
    return num;
}//end
//ctof takes in a float and converts the value to fahrenheit temperature
float ctof(float celsius){
    return ((9*celsius)/5)+32;
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
//dollars prompts user to enter an integer amount for USD and
// returns it as an integer
int dollars(){
    int dollars;
    bool check = false;
    cout<<"Enter a dollar amount in USD for conversion to Euros and Yen."<<endl;
    do{
        cin>>dollars;
        if(cin.fail()||dollars<0){//user did not enter positive integer
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Invalid selection. Try again."<<endl;
        }
        else{ //user entered positive integer
            check=true;
            return dollars;
        }
    }while(!check);
}//end
//yen takes in an integer for USD and statically casts it to float
//and converts to yen, and then returns that value as a float
float yen(int usd){
    return static_cast<float>(usd)*YEN_PER_DOLLAR;
}//end
//euro takes in an integer for USD and statically casts it to float
//and converts to euro, and then returns that value as a float
float euro(int usd){
    return static_cast<float>(usd)*EUROS_PER_DOLLAR;
}//end
//month prompts user for an integer month 1-12, checks to see if it's positive
//and between 1-12, and returns it as an integer if it is
int month(){
    int m;
    bool check=false;
    cout<<"Enter a month 1-12"<<endl;
    do{
        cin>>m;
        if(cin.fail()||m<=0||m>12){
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Not a valid month."<<endl;
        }
        else
            check=true;
    }while(!check);
    return m;
}//end
//year prompts user for a year value, positive integer, and returns it
int year(){
    int y;
    bool check=false;
    cout<<"Enter a year"<<endl;
    do{
        cin>>y;
        if(cin.fail()|y<0){
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Not a valid year."<<endl;
        }
        else
            check=true;
    }while(!check);
    return y;   
}//end
//leap takes in an integer for a year and checks to see if it is a leap year
//returns true for leap, false for not leap
bool leap(int year){
    if(year%400==0||year%4==0)//leap years are divisible by 400 or 4
        return true; //leap year
    else
        return false; //not leap year
}//end
//days takes in an integer for month and an integer for year
//and outputs numbers of day corresponding to that month in that year
void days(int month, int year){
    if(month==2&&leap(year)) //leap year on the second month
        cout<<"29 days"<<endl;
    else if(month==2&&!leap(year)) //not leap year on the second month
        cout<<"28 days"<<endl;
    else if(month==4||month==6||month==9||month==11)
        cout<<"30 days"<<endl;
    else
        cout<<"31 days"<<endl;
}//end
//getpop prompts the user to enter the initial population size for an
//organism, its rate of growth as a percentage, and how many days it will
//be multiplying and stores them in referenced variables from the
//prb511 driver
void getpop(float& p,int& r,int&d){
    bool check=false;
    //get the initial population from the user, and store into p
    cout<<"Enter initial population (2 or greater)"<<endl;
    do{
        cin>>p;
        if(cin.fail()|p<2){ //can't be less than 2
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Not a valid initial population."<<endl;
        }
        else
            check=true;
    }while(!check);
    //reset bool for next round of input
    check=false;
    //get rate of growth of population and store as r
    cout<<"Enter rate of growth as a percentage. (ie, 10% = 10)"<<endl;
    do{
        cin>>r;
        if(cin.fail()|r<0){ //can't be less than 0
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Not a valid growth rate."<<endl;
        }
        else
            check=true;
    }while(!check);
    //reset bool for next round of input
    check=false;
    //get the number of days from the user and store as d
    cout<<"Enter the number of days the population will multiply for."
            <<" (1 or greater)"<<endl;
    do{
        cin>>d;
        if(cin.fail()|d<1){ //can't be less than 1
            cin.clear();
            cin.ignore(256,'\n');
            cout<<"Error. Not a valid number of days."<<endl;
        }
        else
            check=true;
    }while(!check);
}//end
//incpop takes the information gained from getpop and uses it to simulate
//population growth.
void incpop(float& p,int r,int d){
    cout<<"Day: 0, Population: "<<setprecision(0)<<fixed<<p<<endl;
    for(int i=0;i<d;i++){//will loop 'd' times to represent number of days
        p+=(p)*(static_cast<float>(r)*.01);//increases p by the growth rate
        cout<<"Day: "<<i+1<<", Population: "
                <<setprecision(0)<<fixed<<p<<endl;
        //will only output integer portion
    }
}//end
//celsius takes in an integer representing a fahrenheit temperature
//converts to celsius, and returns that value as a float
float celsius(int f){
    return (static_cast<float>(f)-32.0)*(5.0/9.0);
}//end
//clstbl is an output function that outputs a formatting table
//consisting of the first 20 degrees fahrenheit converted to celsius
void clstbl(){
    cout<<"Fahrenheit   "<<"Celsius"<<endl;
    for(int i=0; i<=20; i++){
        cout<<setw(5)<<right<<i<<setw(7)<<setfill(' ')<<" "
                <<setw(4)<<right<<setprecision(1)<<fixed<<celsius(i)<<endl;
    }
}//end
//getWhtr reads from a data file named RainOrShine.txt and stores the contents
//in a 2D array with rows representing month 1-3 and col representing days 1-30
void getWthr(char w[][DAYS]){
    ifstream infile;
    char l;//for storing the content of the data file
    infile.open("RainOrShine.txt");
    for(int i=0;i<MONTHS;i++){
        for(int j=0;j<DAYS;j++){
            infile>>l;
            w[i][j]=l; //stores contents of array sequentially into
                              //month, day format
        }
    }
    infile.close();
}//end
//cntWthr loops through the weather char array and counts the number
//of sunny, rainy, and cloudy days in each of 3 months. It stores the number
//in a 2D parallel (rows)integer array)
void cntWthr(char w[][DAYS],int l[][MONTHS]){
    int sunny=0; //counter for sunny days
    int rainy=0; //counter for rainy days
    int cloudy=0; //counter for cloudy days
    for(int i=0;i<MONTHS;i++){
        //reset counters at the beginning of each 'month'
        cloudy=0;
        sunny=0;
        rainy=0;
        for(int j=0;j<DAYS;j++){
            if(w[i][j]=='S')
                sunny++;
            else if(w[i][j]=='C')
                cloudy++;
            else
                rainy++;
        }
        l[i][0]=sunny;//stores the sunny days for month
        l[i][1]=cloudy;//stores the cloudy days for month
        l[i][2]=rainy;//stores the rainy days for month
    }
}//end
//rainy searches an array of rainy, cloud, and sunny counts
//and finds which month has the most rainy days and returns the location
//in the array
int rainy(int l[][MONTHS]){
    int index=0;
    for(int i=0;i<MONTHS;i++){
        if(l[i][2]>l[index][2])
            index=i;
    }
    return index;
}
//statWthr organizes and outputs the results of cntWthr. It lists the number
//of rainy, cloudy, and sunny days for each month and also announces
//which month had the most rainy days
void statWthr(int l[][MONTHS]){
    
    //output table of June, July, and August vs sunny, rainy, cloudy days
    cout<<setw(13)<<setfill(' ')<<" "
            <<"June  July  Agst"<<endl;
    cout<<"Sunny"<<setw(11)<<setfill(' ')<<right<<l[0][0]
                 <<setw(6)<<setfill(' ')<<right<<l[1][0]
                 <<setw(6)<<setfill(' ')<<right<<l[2][0]<<endl;
    cout<<"Cloudy"<<setw(10)<<setfill(' ')<<right<<l[0][1]
                 <<setw(6)<<setfill(' ')<<right<<l[1][1]
                 <<setw(6)<<setfill(' ')<<right<<l[2][1]<<endl;
    cout<<"Rainy"<<setw(11)<<setfill(' ')<<right<<l[0][2]
                 <<setw(6)<<setfill(' ')<<right<<l[1][2]
                 <<setw(6)<<setfill(' ')<<right<<l[2][2]<<endl;
    cout<<endl;
    
    //sum together total sunny, rainy, and cloudy days from each month
    cout<<"Total Sunny"<<setw(5)<<setfill(' ')<<right
            <<l[0][0]+l[1][0]+l[2][0]<<endl;
    cout<<"Total Cloudy"<<setw(4)<<setfill(' ')<<right
            <<l[0][1]+l[1][1]+l[2][1]<<endl;
    cout<<"Total Rainy"<<setw(5)<<setfill(' ')<<right
            <<l[0][2]+l[1][2]+l[2][2]<<endl;
    cout<<endl;
    
    //determine which month had most rainy days
    if(rainy(l)==0)
        cout<<"June had the most rainy days."<<endl;
    else if(rainy(l)==1)
        cout<<"July had the most rainy days."<<endl;
    else
        cout<<"Agst had the most rainy days."<<endl;
}//end
//bubString bubble sorts a given array of strings
void bubString(string a[],int size){
    for(int i=0; i<size; i++)
    {
        for (int j=0; j<size-1;j++)
        {
            if(a[j]>a[j+1])
                swap(a[j],a[j+1]);
        }
    }
}//end
//binString binary searches for a specified string in an array of strings
int binString(string a[],int size, string target){
    int low=0; //first location in array
    int high=size-1; //last location in array
    while(low<=high){ // if low > high, target is not in array
        int mid=(high+low)/2; //middle location
        if(a[mid]==target) //compares target to potential location
            return mid; //found target value at middle location
        else if(a[mid]<target)
            low=mid+1; //value is not to the left of middle location
        else
            high=mid-1; //value is not to the right of middle location
    }
    return -1; //value not found in array
}//end
//output outputs the contents of a string array, formatted to not be terrible
void output(string a[],int size){
    for(int i=0;i<20;i++){
        if(i%5==0)
            cout<<endl;
        cout<<a[i]<<setw(4)<<setfill(' ')<<" ";
    }
    cout<<endl;
}