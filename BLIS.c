#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_ENT 100
#define Max_col 10
#define Max_row 10

typedef struct
{
    int column;
    int row; // Lot[0] = Row Lot[1] = Column
    char fullname[21];
    int BirthMonth;
    int BirthDay;
    int BirthYear;
    int DeathMonth;
    int DeathDay;
    int DeathYear;
} Info;

Info Read_Entry(FILE *fileptr)
{
    Info entry;
    char colLetter;
    // Read lot coordinates
    fscanf(fileptr, " %c %d", &colLetter, &entry.row);
    entry.column = toupper(colLetter) - 'A'; // Convert letter to column index

    // Read rest of the information
    fscanf(fileptr, "%20s", entry.fullname);
    fscanf(fileptr, "%d", &entry.BirthMonth);
    fscanf(fileptr, "%d", &entry.BirthDay);
    fscanf(fileptr, "%d", &entry.BirthYear);
    fscanf(fileptr, "%d", &entry.DeathMonth);
    fscanf(fileptr, "%d", &entry.DeathDay);
    fscanf(fileptr, "%d", &entry.DeathYear);

    return entry;
}

void MarkLot(char Lot[][Max_col], Info Sentry)
{
    Lot[Sentry.row - 1][Sentry.column - 1] = 'X'; // Mark the lot based on 1-based indexing
}

void PLot(char Lot[][Max_col])
{
    printf("\t\t--------------------------------------------------------------\n");
    for (int i = 0; i < Max_row; i++)
    {
        printf("\t\t| ");
        for (int j = 0; j < Max_col; j++)
        {
            printf("  %c  |", Lot[i][j]);
        }
        printf("\n\t\t--------------------------------------------------------------\n");
    }
}

int main()
{
    int Menu_Option;
    int LvisMenu;               // Variable for Menu
    Info deads[MAX_ENT];        // Arrays of Struct, 100
    char Lot[Max_row][Max_col]; // 10x10 matrix for Lot

    // Initialize Lot as empty
    for (int i = 0; i < Max_row; i++)
    {
        for (int j = 0; j < Max_col; j++)
        {
            Lot[i][j] = ' ';
        }
    }

    FILE *ifp; // File pointer for input
    FILE *ofp; // File pointer for Output

    ifp = fopen("MASTERLIST.txt", "rt");
    ofp = fopen("Copylist.txt", "wt");

    if (!ifp || !ofp)
    {
        printf("Error opening files.\n");
        return 1;
    }

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

        Info entry;
        char colLetter;

        switch (Menu_Option)
        {
        case 1:
            break;
        case 2: // Lot Visualization
            memset(deads, 0, sizeof(deads));
            memset(Lot, ' ', sizeof(Lot));
            int index = 0;
            while (!feof(ifp))
            {
                Info entry = Read_Entry(ifp);
                // Mark the lot and store the entry
                if (entry.row >= 1 && entry.row <= Max_row && entry.column >= 1 && entry.column <= Max_col)
                {
                    index = ((entry.row - 1) * 10 + (entry.column)) - 3;
                    deads[index] = entry;
                    MarkLot(Lot, entry);
                }
            }

            // Display the Lot visuals
            PLot(Lot);
            do
            {
                printf("\n%55s\n", "Choose one of the Options below:");
                printf("%55s\n", "1. Choose Lot");
                printf("%54s\n", "2. Main Menu");
                printf("Option:");
                scanf("%d", &LvisMenu);

                switch (LvisMenu)
                {
                case 1:
                    break;
                case 2:
                    break;
                default:
                    printf("Not an option, Try Again\n");
                    break;
                }

            } while (LvisMenu != 1 && LvisMenu != 2);

            fseek(ifp, 0, SEEK_SET);
            break;

        case 3: // Package Descriptions
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
            fclose(ifp);
            fclose(ofp);
            exit(0);
        default: // Loop if Not an Option
            printf("Not an option, Try Again\n");
            break;
        }
    }

    return 0;
}
