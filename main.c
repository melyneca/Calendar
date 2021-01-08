#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct _time{
    int hour;
    int minute;
}tTime;

typedef struct _date{
    int day;
    int month;
    int year;
}tDate;

typedef struct _event{
    long long id; // Stores the id of the event
    char name[15]; // Stores the short name of the event
    char description[100]; // Stores the description of the event
    tDate Date; // Stores the day, month and year of the event
    tTime Time; // Stores the hour and minute of the event
    tTime alarm; // Stores the hour and minute of the alarm
    int AlarmState; // Stores the alarm state of the alarm (1 or 0)
    struct _event *next; // Stores the pointer to the next event
}tEvent;

int main()
{
    tEvent* Event_Sorter(tEvent* root, tEvent* new_event); // Add events into the linked list in a sorted way
    tEvent* Day_Check(tEvent *iter, tDate today, int times);
    tEvent* Month_Load(); // Gets all the events in that month and creates a sorted linked list
    tEvent* File_Load_All(); // Gets all the events and creates a sorted linked list
    tEvent* Event_Finder(long long id); // Finds the event according to its id and returns a pointer to that event
    tEvent* Load_Alarm(); // Gets all the events which's alarm has not activated yet
    tEvent* Create_Event(char * name, char * description , tTime Time, tDate Date, int AlarmState, tTime alarm); // Creates the event, returns a pointer to that event

    long long Id_Creater(tTime Time, tDate date); //Creates an id according to its date and time

    void Print_Day(tEvent *root); //Prints event
    void Print_All(tEvent *root); //Prints all the events with id's
    void Month_View (tEvent *root); // Draws the grid and prints the events in them
    void Free_Link (tEvent *root); //frees linked list
    void Box(char *text); // Draw a box outside of the text
    void Alarm_Checker( tEvent *alarm );
    void Delete_Event(long long id); // Deletes event from the file

    int Time_Check(tEvent *Alarm, struct tm *current); // Checks whether the time of the alarm is reached
    int Event_Editer(long long id, tEvent* new_event); // Edits the event in the file
    int Add_File(tEvent *new_event); //Add new event to end of the file

    int choice; // Stores the intiger typed by user
    char newline; // Stores the id
    char yn; // Stores y/n

    long long id; // Stores the id
    char name[15]; // Stores the short name of the event
    char description[100]; // Stores the description of the event
    tDate Date; // Day/Month/Year of the event
    tTime Time; // Hour:Minute of the event
    tTime alarm; // Stores the alarm time for the
    int AlarmState; // If alarm is set or has not activated yet it 1. Else 0.

    tEvent *alarmPtr = NULL;
    tEvent *startPtr = NULL;

    system("chcp 65001");
    system("cls");

    do
    {

        alarmPtr = Load_Alarm(); // Reads all events and crates a linked list from events which's alarm state is 1
        Alarm_Checker(alarmPtr); //If the alarm time is passed, prints the event
        Free_Link(alarmPtr); //Frees the linked list
        alarmPtr = NULL;

        //Prints the main menu
        for(int i =0; i<3 ; i++){
            for(int j=0;j<3; j++){

                if( i == 0 && j == 0){
                    printf("\u2554");
                }
                else if( i == 0 && j == 2){
                    printf("\u2557");
                }
                else if(j == 2 && i == 0){
                    printf("\u255A");
                }
                else if(j==2 && i==2){
                    printf("\u255D");
                }
                else if(j==0 && i==2){
                    printf("\u255A");
                }
                else if( i == 0 || i == 2){
                    for(int k = 0; k < 38; k++){
                        printf("\u2550");
                    }
                }
                else if(j==2 && i==2){
                    printf("\u255D");
                }
                else if(j == 0 || j == 2){
                    printf("");
                }
                else{
                    printf("\u2551                                      \u2551\n"
                           "\u2551             MAIN MENU                \u2551\n"
                           "\u2551                                      \u2551\n"
                           "\u2551          1. Add an event             \u2551\n"
                           "\u2551          2. Delete an event          \u2551\n"
                           "\u2551          3. Edit an event            \u2551\n"
                           "\u2551          4. Day view                 \u2551\n"
                           "\u2551          5. Month view               \u2551\n"
                           "\u2551                                      \u2551\n"
                           "\u2551          Press q to exit             \u2551\n"
                           "\u2551                                      \u2551");

                }
            }
            printf("\n");
        }

        printf("\nType your choice: ");
        scanf("%d",&choice);
        newline = getchar();
        system("cls");

        if (newline != '\n'){
            choice = tolower(newline) - 0;
        }

        switch(choice)
        {
            case 1:

                system("cls"); //Clears the screen
                printf("\n");

                Box("Add Event Menu");

                //gets inputs from user
                printf("\nEvent name:");
                scanf("%14[^\n]s ",name);
                newline = getchar();
                name[sizeof(name) - 1] = '\0'; //Puts \0 at the end of the string

                printf("Event description:");
                scanf("%99[^\n]s ",description);
                newline = getchar();
                description[sizeof(description) - 1] = '\0'; //Puts '\0' at the end of the string

                printf("Event date(dd/mm/yyyy): ");
                scanf("%d/%d/%d", &Date.day, &Date.month, &Date.year);
                newline = getchar();

                printf("Set the time( hh:mm ): ");
                scanf("%d:%d", &Time.hour, &Time.minute);
                newline = getchar();

                printf("Do you want to set an alarm(y/n)");
                scanf("%c",&yn);
                newline = getchar();

                if( yn == 'y' || yn == 'Y'){ //If user wants to set an alarm takes the time for the alarm
                    printf("Set the time for alarm( hh:mm ):");
                    scanf("%d:%d", &alarm.hour, &alarm.minute);
                    newline = getchar();
                    AlarmState = 1;
                }
                else if ( yn == 'n'){ //else sets alarm to 0
                    alarm.hour = 0;
                    alarm.minute=0;
                    AlarmState = 0;
                }

                tEvent *new_event;

                new_event = Create_Event( name, description , Time , Date , AlarmState, alarm);  // Gets inputs and creates a new event

                Add_File(new_event);  // Writes event to the file

                free(new_event); // Frees the memory allocated for new_event

                break;

            case 2:

                system("cls");// Clears the screen

                Box("Delete Menu"); // Menu headher

                startPtr = File_Load_All();  // Reads all the events from the file, creates a sorted linked list
                Print_All(startPtr); // Print all the event's id,name and date to the screen

                printf("\n\nIf you don't want to delete an event type 1\n\n");
                printf("Write the id of the event you want to delete:");

                scanf("%lli",&id);

                if( id == 1){ // If user inputs 1, breaks
                    system("cls");
                    break;
                }

                Delete_Event(id); //Deletes the event which has the id typed by user
                Free_Link(startPtr);
                break;

            case 3:

                system("cls");  // Clears the screen
                Box(" Edit Menu ");

                startPtr = File_Load_All();  // Loads all events from file, creates a sorted linked list

                Print_All(startPtr); // Print all the event's id,name and date to the screen

                Free_Link(startPtr); //Frees the linked list
                startPtr = NULL;

                printf("\n\nIf you don't want to edit an event write 1\n\n");
                printf("Write the id of the event you want to edit:");

                scanf("%lli",&id);
                scanf("%c",&newline);

                if( id == 1){
                    system("cls");
                    break;
                }

                tEvent *edited = Event_Finder(id); //Finds the event which has the id typed by user

                if (edited != NULL){

                    system("cls");  // Clears the screen
                    Box(" Edit Menu ");

                    // Prints the event's current informations
                    printf("\n\nName:         %s\n", edited->name);
                    printf("Description:  %s\n", edited->description);
                    printf("Date:         %02d/%02d/%02d\n", edited->Date.day, edited->Date.month, edited->Date.year);
                    printf("Time:         %02d:%02d\n", edited->Time.hour, edited->Time.minute);
                    printf("Alarm State:  %d\n", edited->AlarmState);
                    printf("Alarm time:   %02d:%02d\n", edited->alarm.hour, edited->alarm.minute);

                    //Asks if user wants to edit the informations in the event and the the inputs
                    printf("\nDo you want to change the name of the event?(y/n): ");
                    yn = getch();
                    printf("\n\n");

                    if(yn == 'y' || yn == 'Y'){
                        printf("\tType what you want to edit name as: ");
                        scanf("%[^\n]s ",name);
                        newline = getchar();
                        printf("\n");
                    }
                    else{
                        strncpy(name, edited->name , sizeof(name)); // Copies the existing name to the name
                    }

                    printf("Do you want to change the description of the event?(y/n): ");
                    yn = getch();
                    printf("\n\n");

                    if(yn == 'y' || yn == 'Y'){
                        printf("\tType what you want to edit description as: ");
                        scanf("%[^\n]s ", description);
                        newline = getchar();
                        printf("\n");
                    }
                    else{
                        strncpy(description, edited->description , sizeof(description)); // Copies the existing description to the description
                    }

                    printf("Do you want to change the date of the event?(y/n): ");
                    yn = getch();
                    printf("\n\n");

                    if(yn == 'y' || yn == 'Y'){
                        printf("\tType what you want to edit date as (dd/mm/yyyy): ");
                        scanf("%d/%d/%d",&Date.day , &Date.month, &Date.year);
                        newline = getchar();
                        printf("\n");
                    }
                    else{
                        Date = edited->Date;
                    }

                    printf("Do you want to change the time of the event?(y/n): ");
                    yn = getch();
                    printf("\n\n");

                    if(yn == 'y' || yn == 'Y'){
                        printf("\tType what you want to edit time as (hh:mm): ");
                        scanf("%d:%d",&Time.hour, &Time.minute);
                        newline = getchar();
                        printf("\n");
                    }
                    else{
                        Time = edited->Time;
                    }

                    printf("Do you want to change the alarm state of the event?(y/n): ");
                    yn = getch();
                    printf("\n\n");

                    if(yn == 'y' || yn == 'Y'){
                        printf("\tType what you want to edit alarm state as: ");
                        scanf("%d",&AlarmState);
                        newline = getchar();
                        printf("\n");
                    }
                    else{
                        AlarmState = edited->AlarmState;
                    }

                    printf("Do you want to change the alarm time of the event?(y/n): ");
                    yn = getch();
                    printf("\n\n");

                    if(yn == 'y' || yn == 'Y'){
                        printf("\tType what you want to edit alarm time as (hh:mm): ");
                        scanf("%d:%d",&alarm.hour,&alarm.minute);
                        newline = getchar();
                        printf("\n");
                    }
                    else{
                        alarm = edited->alarm;
                    }

                    free(edited); // Frees the memory allocated for "edited"
                    new_event = Create_Event(name, description , Time , Date, AlarmState, alarm); // Creates the new event

                    if (Event_Editer(id,new_event) == 1){ // Writes to the file
                        Box("Successfully Edited");
                    }
                    sleep(1);


                    break;
                }
                else{
                    Box("Event couldn't found");
                    sleep(1);
                    system("cls");
                    break;
                }

            case 4:
                system("cls");

                Box("TODAY'S EVENTS"); // Header of the menu

                startPtr = File_Load_All(); // Loads all events from file, creates a sorted linked list

                Print_Day(startPtr); // Prints the events scheduled today

                Free_Link(startPtr); // Frees the created linked list
                startPtr = NULL;

                break;

            case 5:

                system("cls");

                startPtr = Month_Load(); // Reads the event that is scheduled this from the file,creates a sorted linked list

                Month_View (startPtr); // Prints events

                Free_Link(startPtr);  // Frees the created linked list
                startPtr = NULL;

                break;

            case 113:

                printf("Do you want to quit(y/n):");
                newline = getchar();
                scanf("%c",&yn);

                if(yn == 'n' || yn == 'N'){
                    break;
                }
                else if (yn == 'y' || yn == 'Y'){

                    return 0;
                }
        }

    }
    while(1);
}

//Creates an id according to date and time
long long Id_Creater(tTime Time, tDate Date){

    long long id = (long long)(Date.year%100)*100000000 +Date.month*1000000+Date.day*10000+Time.hour*100 +Time.minute;
    return id;
}

// Allocates memory for an event, puts the parameters into the event and return a pointer to that event
tEvent* Create_Event(char * name, char * description , tTime Time, tDate Date, int AlarmState, tTime alarm){

    long long id = Id_Creater(Time,Date); // Creates the id

    tEvent *new_event = (tEvent*) malloc(sizeof(tEvent)); // Allocates a memory for the event

    // Copies inputs into the event
    strncpy(new_event->name, name , sizeof(new_event->name));
    strncpy(new_event->description, description , sizeof(new_event->description));
    new_event->Time = Time;
    new_event->Date = Date;
    new_event->id = id;
    new_event->AlarmState = AlarmState;
    new_event->alarm = alarm;
    new_event->next = NULL;
}

// Adds new event at the end of the file
int Add_File(tEvent *new_event){

    FILE *pFile; // File pointer
    pFile = fopen("eventFile.bin","ab"); // Opens the file

    if (pFile != NULL){

        fseek(pFile,0,SEEK_END);
        fwrite( new_event , sizeof(tEvent), 1 , pFile);
		fclose(pFile);
		return 1;
    }

    else{
        printf("FILE DOES NOT EXIST\n");
    }
}

// Creates a sorted linked list according to its id and returns start of the linked list
tEvent* Event_Sorter(tEvent *startPtr, tEvent *new_event ){

    if( startPtr == NULL){ // When linked list is empty
        new_event->next = NULL;
        return new_event; // New event is the start of the linked list
    }

    if ( startPtr->id > new_event->id){ // If we have to add the new event at the beginning of the list

        new_event->next = startPtr; // New event's next is startPrt
        return new_event; // Returns new event as the start of the linked list
    }

    tEvent *iterPtr = startPtr;

    //if we have to add to the middle or at to very end.
    while(iterPtr->next != NULL && iterPtr->next->id < new_event->id){
        iterPtr = iterPtr->next;
    }
    new_event->next = iterPtr->next;
    iterPtr->next = new_event;
    return startPtr; // startPtr does not change at this condition
}

// Deletes an event from the file
void Delete_Event(long long id){

    FILE *pFile, *pFiletemp;
    pFile = fopen("eventFile.bin","rb"); //Reads all the events from this file
    pFiletemp = fopen("eventFiletemp.bin","wb"); //Writes all the events except the event which has the id

    if( pFile != NULL){

        tEvent *event;

        fseek(pFile, 0, SEEK_END); // Goes end of the file
		long size = ftell(pFile); // Find the size of the file
		rewind(pFile); // Goes beginning of the file

        event = (tEvent*)malloc(sizeof(tEvent)); // Allocates memory

		int event_num = (int)(size /(sizeof(tEvent))); // Calculates the number of events in the file

		for(int i = 0 ; i < event_num ; i++){

            fseek(pFile, sizeof(tEvent)*i , SEEK_SET); // Points to beginning of events
            fread(event, sizeof(tEvent), 1, pFile); // Reads the event

            if(event->id != id){ // If it's id is different than the one wanted to be deleted writes to the temporary file
                fseek(pFiletemp,0,SEEK_END); // Points to the end of the file
                fwrite( event , sizeof(tEvent), 1 , pFiletemp); // Writes the event
            }

		}

        fclose(pFile);
        fclose(pFiletemp);
        free(event);
        remove("eventFile.bin"); // Removes the first file
        rename("eventFiletemp.bin","eventFile.bin"); // Renames the temporary file as the first file
    }
    else{
        printf("ERROR");
    }
}

// If Alarm time is passed returns 1. Else returns 0
int Time_Check(tEvent *Alarm, struct tm *current){

    if( current->tm_year+1900 > Alarm->Date.year){
        return 1;
    }
    else if( current->tm_year+1900 == Alarm->Date.year && current->tm_mon+1 > Alarm->Date.month){
        return 1;
    }
    else if( current->tm_year+1900 == Alarm->Date.year && current->tm_mon+1 == Alarm->Date.month && current->tm_mday > Alarm->Date.day){
        return 1;
    }
    else if( current->tm_year+1900 == Alarm->Date.year && current->tm_mon+1 == Alarm->Date.month && current->tm_mday == Alarm->Date.day
            && current->tm_hour > Alarm->alarm.hour){
        return 1;
    }
    else if(current->tm_year+1900 == Alarm->Date.year && current->tm_mon+1 == Alarm->Date.month && current->tm_mday == Alarm->Date.day
            && current->tm_hour == Alarm->alarm.hour && current->tm_min >= Alarm->alarm.minute){
        return 1;
    }
    else{
        return 0;

    }

}
// Draws a box around the text
void Box(char *text){

    int n = strlen(text);

    for(int i =0; i<3 ; i++){
        for(int j=0;j<3; j++){

            if( i == 0 && j == 0){
                printf("\u2554");
            }
            else if( i == 0 && j == 2){
                printf("\u2557");
            }
            else if(j == 2 && i == 0){
                printf("\u255A");
            }
            else if(j==2 && i==2){
                printf("\u255D");
            }
            else if(j==0 && i==2){
                printf("\u255A");
            }
            else if( i == 0 || i == 2){
                for(int k = 0; k < n+52; k++){
                    printf("\u2550");
                }
            }
            else if(j==2 && i==2){
                printf("\u255D");
            }
            else if(j == 0 || j == 2){
                printf("");
            }
            else{
                printf("\u2551");
                for(int i = 0; i < n+52;i++){
                    printf(" ");
                }
                printf("\u2551");
                printf("\n");
                printf("\u2551                          %s                          \u2551\n",text);
                printf("\u2551");
                for(int i = 0; i < n+52;i++){
                    printf(" ");
                }
                printf("\u2551");
            }
        }
        printf("\n");
    }
}
// Iterates through every event and prints alarm message if time is passed for the event
void Alarm_Checker( tEvent *AlarmPtr ){

    // Gets the current time
    time_t now = time(NULL);
    struct tm *current = localtime(&now);

    tEvent *new_event;
    tEvent *iterPtr = AlarmPtr; // Points to the start of the alarm linked list

    while(iterPtr!= NULL){

        if(Time_Check(iterPtr,current)==1){ // If alarm time is passed  Time_Check returns 1

            Box("Alarm"); // Header for the alarm
            printf("\a"); // Alarm beep

            //Prints the informations of the event
            printf("Name: %s\n", iterPtr->name);
            printf("Description: %s\n", iterPtr->description);
            printf("Date: %d/%d/%d\n", iterPtr->Date.day, iterPtr->Date.month, iterPtr->Date.year);
            printf("Scheduled to %d:%d\n", iterPtr->Time.hour, iterPtr->Time.minute);

            for(int i = 0; i < 57;i++){
                printf("\u2550");
            }
            printf("\n");

            // Changes the alarm state to 0
            new_event = Create_Event(iterPtr->name, iterPtr->description, iterPtr->Time,iterPtr->Date,0,iterPtr->alarm);
            Event_Editer(iterPtr->id, new_event);

        }
        iterPtr = iterPtr->next;
    }
}
// Prints the events in the current day
void Print_Day(tEvent *startPtr){

    // Gets the current date
    time_t now = time(NULL);
    struct tm *current = localtime(&now);
    int day = current->tm_mday;
    int month = current->tm_mon+1;
    int year = current->tm_year + 1900;

    tEvent *iterPtr = startPtr;

    printf("\nEvent \t\t\t\t\t\t\t Time\n");

    for(int k = 0; k < 68; k++){
        printf("\u2550");
    }
    printf("\n");

    // Prints the event if it is scheduled to the same day, month and year
    while(iterPtr != NULL){
        if(iterPtr->Date.day == day){
            if(iterPtr->Date.month== month){
                if(iterPtr->Date.year == year){
                    printf("%-20s \t\t\t\t\t %02d:%02d\n", iterPtr->name, iterPtr->Time.hour, iterPtr->Time.minute);
                }
            }
        }
        iterPtr = iterPtr->next;
    }
    printf("\n");
}

// Finds the event which has the id from the file, allocates memory for it and returns a pointer to that event
tEvent* Event_Finder(long long id){

    FILE *pFile;
    pFile = fopen("eventFile.bin","rb"); // Opens the file in read mode

    if( pFile != NULL){

        fseek(pFile, 0, SEEK_END); // Points to end of the file
		long size = ftell(pFile); // Finds the size of the file

        tEvent *event = (tEvent*)malloc(sizeof(tEvent)); // Allocates memory for the event
		int event_num = (int)(size / (sizeof(tEvent))); // Calculates the number of events in the file

		for(int i = 0 ; i < event_num ; i++){

            fseek(pFile, sizeof(tEvent)*i, SEEK_SET); // Points to beginning of the events in the file
            fread(event, sizeof(tEvent), 1, pFile); // Reads from the file and copies into the event

            if(event->id == id){ // If there is an event with this id in the file
                fclose(pFile);
                return event; // Returns a pointer to that event
            }
		}
        fclose(pFile);
        free(event); // If the event could not found in file, frees the memory
        return NULL;
    }
    else{
        printf("No file");
        return NULL;
    }
}

int Event_Editer(long long id, tEvent* edited){

    FILE *pFile;
    pFile = fopen("eventFile.bin","rb+"); // In this mode it reads and writes to the file

    if( pFile != NULL){

        fseek(pFile, 0, SEEK_END); // Points to end of the file
		long size = ftell(pFile); // Finds the size of the file

		int event_num = (int)(size / (sizeof(tEvent))); // Calculates the number of events in the file
		tEvent *event = (tEvent*) malloc(sizeof(tEvent));

		for(int i = 0 ; i < event_num ; i++){

            fseek(pFile, sizeof(tEvent)*i , SEEK_SET); // Points to beginning of the events in the file
            fread(event, sizeof(tEvent), 1, pFile); // Reads from the file and copies into the event

            if(event->id == id){ // if the id is found
                fseek(pFile, sizeof(tEvent)*i , SEEK_SET); // Points the same location
                fwrite( edited , sizeof(tEvent), 1 , pFile); // Writes over the event
                free(event); // Frees the allocated memory
                free(edited);
                fclose(pFile);
                return 1; // Return 1 if the event successfully editted
            }
		}
		free(event);
        fclose(pFile); // Frees the allocated memory
        return 0;
    }
    else{
        printf("No File");
        return 0;
    }
}

// Finds the event in that date and iterates if it is needed
tEvent* Day_Check(tEvent *startPtr, tDate today, int iterate){

    tEvent *iterPtr = startPtr;

    while(iterPtr != NULL){
        // Finds the first event which had the same date
        if(iterPtr->Date.year == today.year && iterPtr->Date.month == today.month && iterPtr->Date.day == today.day){
            break;
        }
    iterPtr = iterPtr->next;
    }

    if(iterPtr == NULL){ // Checks if the iter points to NULL or not
        return NULL;
    }
    // Iterates as much as needed
    for(int i = 0; i < iterate && iterPtr !=NULL ; i++){
        iterPtr = iterPtr->next;
    }

    if(iterPtr == NULL){
        return NULL;
    }
    // If it still has the same date returns the event
    if(iterPtr->Date.year == today.year && iterPtr->Date.month == today.month && iterPtr->Date.day == today.day){
        return iterPtr;
    }
    return NULL;
}

// Frees the memory allocated for a linked list
void Free_Link (tEvent *startPtr){

	tEvent *iterPtr = startPtr; // Starts from the beginning
	tEvent *holdPtr = NULL;

	while( iterPtr != NULL ) {

		holdPtr = iterPtr->next ; // Holds the next event

		free(iterPtr); // Frees the memory

		iterPtr = holdPtr; // Moves to the next event
	}
}

// Reads the events which has alarm state 1 from the file and creates a sorted linked list
tEvent* Load_Alarm(){

    FILE *pFile;
    pFile = fopen("eventFile.bin","rb"); // Opens the file in read mode

    if (pFile != NULL){

        tEvent *event;
        tEvent *startPtr = NULL;

        fseek(pFile, 0, SEEK_END); // Points end of the file
		long size = ftell(pFile); // Find the size of the file

		int event_num = (int)(size / (sizeof(tEvent))); // Calculates the number of events in the file

		for(int i = 0 ; i < event_num ; i++){

            fseek(pFile, sizeof(tEvent)*i , SEEK_SET); // Points to beginning of the events in the file
            event = (tEvent*)malloc(sizeof(tEvent)); // Allocates memory
            fread(event, sizeof(tEvent), 1, pFile); // Reads from the file and copies into the event

            if( event->AlarmState == 1){
                startPtr = Event_Sorter (startPtr, event); // Creates a linked list from event which has alarm state 1
            }
		}
		fclose(pFile); // Closes the file
        return startPtr; // Returns the start point of the linked list
    }
    else{
        printf("File does not exist");
        return NULL;
    }
}
// Reads the events and creates a sorted linked list
tEvent* File_Load_All(){

    FILE *pFile;
    pFile = fopen("eventFile.bin","rb"); // Opens the file in read mode

    if (pFile != NULL){

        tEvent *new_event;
        tEvent *startPtr = NULL;

        fseek(pFile, 0, SEEK_END); // Points end of the file
		long size = ftell(pFile); // Finds the size of the file

		int event_num = (int)(size / (sizeof(tEvent))); // Calculates the number of events in the file

		for(int i = 0 ; i < event_num ; i++){

            fseek(pFile, sizeof(tEvent)*i , SEEK_SET); // Points the beginning of the event in the file
            new_event = (tEvent*)malloc(sizeof(tEvent)); // Allocates memory for new event
            fread(new_event, sizeof(tEvent), 1, pFile); // Reads from the file and copies to the new event

            startPtr = Event_Sorter( startPtr, new_event); // Sorts the events and creates a linked list
		}
		fclose(pFile);
        return startPtr;
    }

    else{
        return NULL;
    }
}

// Prints the events with id, name, date and time
void Print_All(tEvent *startPtr){

    tEvent *iterPtr = startPtr;
    // Prints all events until iter points to NULL
    while(iterPtr != NULL){
        printf("%llu \t %-15s \t\t %02d:%02d %02d/%02d/%d\n", iterPtr->id, iterPtr->name, iterPtr->Time.hour,
               iterPtr->Time.minute, iterPtr->Date.day, iterPtr->Date.month, iterPtr->Date.year);
        iterPtr = iterPtr->next;
    }
}

// Reads the events and creates a sorted linked list from events which has the same month and year
tEvent* Month_Load(){

    // Gets the current month and year
    time_t now = time(NULL);
    struct tm *current = localtime(&now);
    int month = current->tm_mon+1;
    int year = current->tm_year + 1900;

    FILE *pFile;
    pFile = fopen("eventFile.bin","rb"); // Opens the file in read mode

    tEvent *event; // Pointer to the event that is read from the file
    tEvent *startPtr = NULL;// pointer to the beginning of the linked list

    if (pFile != NULL){

        fseek(pFile, 0, SEEK_END); // Points end of the file
		long size = ftell(pFile); // Finds the size of the file

		int event_num = (int)(size / (sizeof(tEvent))); // Calculates the number of events in the file

		for(int i = 0 ; i < event_num ; i++){

            fseek(pFile, sizeof(tEvent)*i , SEEK_SET); // Points the beginning of the event in the file
            event = (tEvent*)malloc(sizeof(tEvent)); // Allocates memory for new event
            fread(event, sizeof(tEvent), 1, pFile); // Reads from the file and copies to the new event

            if(event->Date.year == year){
                if(event->Date.month == month){ // Id the event has the same month and year ads to the linked list
                    startPtr = Event_Sorter(startPtr, event);
                }
            }
            else{ // Else frees the memory
                free(event);
            }
		}
		fclose(pFile);//Closes the file
        return startPtr; // Returns the beginning of the linked list
    }
    else{
        return NULL;
    }
}

void Month_View (tEvent *root){

    // Gets the current time
    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    int Month = today->tm_mon;
    int day = today->tm_mday;
    int year = today->tm_year + 1900;

    // Calculates the fist day's column
    int beg = (today->tm_wday - day + 1)%7;
    while(beg<=0){
        beg = beg+7;
    }

    // Day of the months
    int dayinMonth[]={31,28,31,30,31,30,31,31,30,31,30,31};

    // If it is a leap year makes February 29 days
    if(year%4 == 0){
        dayinMonth[1]=29;
    }

    day = dayinMonth[Month]; // Number of days in that month
    int current = 1; // Starts from 1

    tDate date;
    date.month = Month+1;
    date.year = year;

    tEvent *iter;

    // Row number in calendar changes according to it's day number and beginning of the month
    int row;
    if( beg == 1 && day == 28){
        row = 28;
    }
    else if( beg == 6 && day == 31 ){
        row=42;
    }
    else if( beg == 7 && day>29){
        row = 42;
    }
    else{
        row=35;
    }
    printf("\n\tMonday \t\t    Tuesday\t\tWednesday\t      Thursday \t\t   Friday \t\tSaturday\t     Sunday\n");

    for(int i = 0; i<=row; i++){

        for(int j = 0; j <= 14; j++){
            // Prints the frame of the calendar
            if(j%2==0){
                    if(j==0 && i==0){
                        printf("\u2554");
                    }
                    else if(j==14 && i==0){
                        printf("\u2557");
                    }
                    else if(i == 0 ){
                        printf("\u2566");
                    }
                    else if(i == row && j == 0){
                        printf("\u255A");
                    }
                    else if(i == row && j == 14){
                        printf("\u255D");
                    }
                    else if(i== row){
                        printf("\u2569");
                    }
                    else if(j == 0 && i%7 ==0){
                        printf("\u2560");
                    }
                    else if(j == 14 && i%7 == 0){
                        printf("\u2563");
                    }
                    else if(i%7 == 0){
                        printf("\u256C");
                    }
                    else{
                        printf("\u2551");
                    }

            }
            else if(j%2!=0 && i%7==0){
                    for(int t = 0;t<20;t++){
                        printf("\u2550");
                    }
            }
            else if(i%7==1){
                // Prints the days to the corner of the box
                if( j >= 2*beg-1 || beg + current >= 9 ){
                printf("%02d                  ",current);
                current++;
                }
                else{
                    printf("                    ");
                }
            }
            else {

                int cal = (i/7)*7 - (6 - j/2) + (7-beg)+ 1; // Calculates which day correspond to that point
                date.day = cal;

                iter = Day_Check(root, date, i%7-2); // If there should be an event there, returns the event. Otherwise return NULL

                if(iter != NULL){
                    printf("%-14s %02d:%02d",iter->name, iter->Time.hour, iter->Time.minute); // Prints the event
                }
                else{
                    printf("                    "); // If there is no event, print blank
                }

            }

        }
        printf("\n");
    }
}



