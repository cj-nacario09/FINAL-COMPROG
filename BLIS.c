#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_ENT 100
#define Max_col 10
#define Max_row 10

typedef struct
{
    int Lot[2]; // Lot[0] = Row Lot[1] = Column
    char fullname[21];
    int BirthMonth;
    int BirthDay;
    int BirthYear;
    int DeathMonth;
    int DeathDay;
    int DeathYear;
    char qoute[51];
} Info;

void PLot(char Lot[][Max_col])
{
    printf("\t\t--------------------------------------------------------------\n");
    for (int i = 0; i < 10; i++)
    {
        printf("\t\t| ");
        for (int j = 0; j < 10; j++)
        {
            printf("  %c  |", Lot[i][j]);
        }
        printf("\n\t\t--------------------------------------------------------------\n");
    }
}

Info Read_Entry(FILE *fileptr)
{
    Info entry;
    char colLetter;

    // Read lot coordinates
    if (fscanf(fileptr, " %c %d", &colLetter, &entry.Lot[0]) != 2)
        return entry; // Return empty entry if unable to read

    entry.Lot[1] = toupper(colLetter) - 'A';

    // Read rest of the information
    fgets(entry.fullname, sizeof(entry.fullname), fileptr);
    fscanf(fileptr, "%d", &entry.BirthMonth);
    fscanf(fileptr, "%d", &entry.BirthDay);
    fscanf(fileptr, "%d", &entry.BirthYear);
    fscanf(fileptr, "%d", &entry.DeathMonth);
    fscanf(fileptr, "%d", &entry.DeathDay);
    fscanf(fileptr, "%d", &entry.DeathYear);
    fgets(entry.qoute, sizeof(entry.qoute), fileptr);

    return entry;
}

void MarkLot(char Lot[][Max_col], Info Sentry)
{
    Lot[Sentry.Lot[0] - 1][Sentry.Lot[1]] = 'X';
}

int main()
{
    int Menu_Option;     // Variable for Menu
    Info deads[MAX_ENT]; // Arrays of Struct, 100
    char Lot[10][10];    // 10x10 matrix for Lot

    // Intialize Lot as empty
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Lot[i][j] = ' ';
        }
    }

    FILE *ifp; // File pointer for input
    FILE *ofp; // File pointer for Output

    ifp = fopen("MASTERLIST.txt", "r");
    ofp = fopen("Copylist.txt", "w");

    while (1)
    {
        printf("\n\t\t\t\t\tWelcome to B.L.I.S.\n");
        printf("\t\t\t\t  Burial Lot Information System\n");

        printf("\n%55s\n", "Choose one of the Options below:");
        printf("%54s\n", "1. New Entry");
        printf("%56s\n", "2. Lot Visuals");
        printf("%65s\n", "3. Package Descriptions");
        printf("%49s\n", "4. Exit");
        printf("Option:");
        scanf("%d", &Menu_Option);

        switch (Menu_Option)
        {
        case 1:
            break;
        case 2: // Lot Visualization
            while (1)
            {
                // Read entry from the file
                Info Sentry = Read_Entry(ifp);

                if (!feof(ifp))
                    break; // break if EOF reached

                // Calculate index in the deads array based on the lot coordinates
                int index = (Sentry.Lot[0] - 1) * 10 + (Sentry.Lot[1] - 'A');

                // Store the entry in the corresponding position of the deads array
                deads[index] = Sentry;

                // Mark the lot
                MarkLot(Lot, Sentry);
            }
            PLot(Lot);

            break;
        case 3: // Paclage Descriptions
            printf("\t\tA: Normal\n");
            printf("\t\t\tThis package includes the burial plot itself, the\n\t\t\topening and closing of the grave and a grave care\n\t\t\tof once every 6 months.\n");
            printf("\t\tB: Premium\n");
            printf("\t\t\tThis package is one level above the Normal package.\n\t\t\tThis package offers a more enhanced service  than\n\t\t\tthe previous level such as additional customization\n\t\t\toption, upgraded materials for the headstone or marker\n\t\t\tand a more frequent grave care of once every 3 months.\n");
            printf("\t\tC: Silver\n");
            printf("\t\t\tIn addition to the features of the premium package,\n\t\t\tthe Silver package may include amenities like occasional\n\t\t\tfloral arrangements, access to support services for the\n\t\t\tbereaved, a selection of standard markers or headstones\n\t\t\twith basic engraving options, priority scheduling for\n\t\t\tcemetery visits or special events, ensuring ease of\n\t\t\taccess and convenience for family members and visitors.\n");
            printf("\t\tD: Gold:\n");
            printf("\t\t\tThe Gold package includes all the benefits of the Silver\n\t\t\tpackage and offers the additional feature of a mausoleum\n\t\t\tfor above-ground interment. This may include a designated\n\t\t\tspace within the mausoleum, personalized inscriptions,\n\t\t\tongoing maintenance of the structure and exclusive access\n\t\t\tto certain areas or facilities within the cemetery grounds\n\t\t\tfor private ceremonies or gatherings.\n");
            break;
        case 4: // Exit Program
            printf("Exiting Program...\n");
            exit(0);
            break;
        default: // Loop if Not an Option
            printf("Not an option, Try Again\n");
            break;
        }
    }

    fclose(ifp);
    fclose(ofp);
    return 0;
}
