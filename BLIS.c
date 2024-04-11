#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_ENT 100

int Lot[10][10];

typedef struct
{
    char Firstname[21];
    char MiddleName[21];
    char LastName[21];
    int Birthdate;
    int Deathdate;
    int internment;
    char Quote[101];
    int Lot[2]; // Lot[0] = Row Lot[1] = Column
} Info;

void InitializePlot()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Lot[i][j] = ' ';
        }
    }
}

void PLot()
{
    printf("\t\t--------------------------------------------------------------\n");
    for (int i = 0; i < 10; i++)
    {
        printf("\t\t| ");
        for (int j = 0; j < 10; j++)
        {
            printf(" %c  |", Lot[i][j]);
        }
        printf("\n\t\t--------------------------------------------------------------\n");
    }
}

void AssignPlot(int row, int column, Info *nEntryPTR)
{
    nEntryPTR->Lot[0] = row;
    nEntryPTR->Lot[1] = column;
    Lot[row][column] = 'X';
}

Info New_Entry(Info deads[], int *entryPTR)
{
    Info NewEntry;
    char colLetter;
    printf("Enter First Name: ");
    scanf("%s", NewEntry.Firstname);
    printf("Enter Middle Name: ");
    scanf("%s", NewEntry.MiddleName);
    printf("Enter Last Name: ");
    scanf("%s", NewEntry.LastName);
    printf("Enter Birth Date: ");
    scanf("%d", &(NewEntry.Birthdate));
    printf("Enter Death Date: ");
    scanf("%d", &(NewEntry.Deathdate));
    printf("Enter Date of Internment: ");
    scanf("%d", &(NewEntry.internment));
    printf("Enter Qoute: ");
    scanf("%s", &(NewEntry.Quote));
    printf("Choose Plot coordinated (e.g. A 7): ");
    scanf("%c %d", &colLetter, &(NewEntry.Lot[0]));
    NewEntry.Lot[1] = toupper(colLetter) - 'A';

    AssignPlot(NewEntry.Lot[0], NewEntry.Lot[1], &NewEntry);

    (*entryPTR)++;

    return NewEntry;
}

int main()
{
    int Menu_Option;
    Info deads[MAX_ENT];
    int entry = 0;

    InitializePlot();

    printf("\n\t\t\t\t\tWelcome to B.L.I.S.\n");
    printf("\t\t\t\t  Burial Lot Information System\n");

    printf("\n%55s\n", "Choose one of the Options below:");
    printf("%54s\n", "1. New Entry");
    printf("%56s\n", "2. Lot Visuals");
    printf("%65s\n", "3. Package Descriptions");
    printf("%49s\n", "4. Exit");

    while (1)
    {
        printf("Option:");
        scanf("%d", &Menu_Option);

        switch (Menu_Option)
        {
        case 1:
            deads[entry] = New_Entry(deads, &entry);
            PLot();
            break;
        case 2:
            PLot();
            break;
        case 3:
            break;
        case 4:
            printf("Exiting Program...\n");
            exit(0);
            break;
        default:
            printf("Not an option, Try Again\n");
            break;
        }
        if (Menu_Option >= 1 && Menu_Option <= 3)
        {
            break;
        }
    }
    return 0;
}