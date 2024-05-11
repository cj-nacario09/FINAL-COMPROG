#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_ENT 100 // Maximum number of entries.
#define Max_col 10  // Maximum number of columns for lots.
#define Max_row 10  // Maximum number of rows for lots.
#define MAX_STR 100 // Maximum length of strings.

// Struct for information about a burial lot.
typedef struct
{
    int column;           // Column number of the lot.
    int row;              // Row number of the lot.
    float size1;          // Size parameter 1 of the lot.
    float size2;          // Size parameter 2 of the lot.
    int Price;            // Price of the lot.
    char status[MAX_STR]; // Provides status of the lot.
} Lot_Info;

// Struct for information about a burial entry.
typedef struct
{
    int column;             // Column number of the entry.
    int row;                // Row number of the entry.
    char fullname[MAX_STR]; // Full name of the deceased.
    int BirthMonth;         // Birth month of the deceased.
    int BirthDay;           // Birth day of the deceased.
    int BirthYear;          // Birth year of the deceased.
    int DeathMonth;         // Death month of the deceased.
    int DeathDay;           // Death day of the deceased.
    int DeathYear;          // Death year of the deceased.
    char package;           // Chosen package for the burial.
    char qoute[MAX_STR];    // Quotation for the burial.
} Info;

// Function prototypes.
Info Read_Entry(FILE *fileptr);                // Function to read entry from file.
Lot_Info Lot_details(FILE *lotptr);            // Function to read lot details from file.
void MarkLot(char Lot[][Max_col], Info entry); // Function to mark a lot as taken.
void PLot(char Lot[][Max_col]);                // Function to display lot visuals.
void Print_info(int col, int row, Info deads[MAX_ENT], Lot_Info lotdet[MAX_ENT]);
// Function to print information about a specific burial lot.
void cMonth(int num_month, char *monthptr);                                                                                                     // Function to convert month number to month name.
Info Register(Info entry_again, int curr_year, int column, int row);                                                                            // Function to register a new entry.
char Choose_pckg();                                                                                                                             // Function to choose a package.
float getPackagePrice(char package);                                                                                                            // Function to get the price of a package.
void Save(Info holder[MAX_ENT], Lot_Info lot[MAX_ENT], FILE *tp, FILE *lp);                                                                     // Function to save data to files.
void print_text();                                                                                                                              // Function to print introductory text.
void pkg_des();                                                                                                                                 // Function to display package descriptions
void print_rcpt(Lot_Info lotdet, Info deads, float packagePrice);                                                                               // Function to print a receipt for a burial transaction.
void history(Lot_Info lotdet, Info deads, float packagePrice);                                                                                  // Function to display the history of burial transactions.
void update(int index, char colid, int row_id, Info deads[MAX_ENT], Info empty, Lot_Info lotdet[MAX_ENT], FILE *ifp, FILE *lfp, int curr_year); // Function to update information for a burial entry.

int main()
{
    int Menu_Option; // Variable to store the main menu option
    int LvisMenu;    // Variable for Menu
    int Pckg_menu;   // Variable to store the package menu option
    int Pay_menu;
    int confirm_menu;
    int index = 0; // Index in the array from row and column
    int Lot_index = 0;
    int again;             // Variable for the loop in Choosing Lot Info
    int row_id, column_id; // Column and Row
    float packagePrice;
    int curr_year;

    Info deads[MAX_ENT];      // Arrays of Struct, 100 & array of burial entries
    Lot_Info lotdet[MAX_ENT]; // Array of lot details
    Info entry;               // For the masterlits file with taken lot
    Info entry_again;         // For the input entry
    Lot_Info lot_entry;       // For scanning the lot details
    static const Info empty;  // Empty struct instance

    char Lot[Max_row][Max_col]; // 10x10 matrix for Lot
    char colid;                 // Char Version of Column

    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    curr_year = date.tm_year + 1900;

    // Initialize Lot and the array deads as empty
    memset(deads, 0, sizeof(deads));
    memset(lotdet, 0, sizeof(lotdet));
    memset(Lot, ' ', sizeof(Lot));

    FILE *ifp; // File pointer for input
    FILE *lfp; // File pointer for lot input file

    // Open input files for reading and writing
    ifp = fopen("MASTERLIST.in", "r+");
    lfp = fopen("Lotfile.in", "r+");

    // Move file pointers to the beginning
    fseek(lfp, 0, SEEK_SET);
    fseek(ifp, 0, SEEK_SET);

    // Check if files were opened successfully
    if (!ifp || !lfp)
    {
        printf("Error opening files.\n");
        return 1;
    }

    // Read entries from input files until end of file
    while (1)
    {
        entry = Read_Entry(ifp);
        lot_entry = Lot_details(lfp);

        if (feof(ifp) && ferror(ifp) || feof(lfp) || ferror(lfp))
            break;

        // Check if entry is within valid range and update arrays
        if (entry.row >= 1 && entry.row <= Max_row && entry.column >= 0 && entry.column <= Max_col)
        {
            index = ((entry.row - 1) * Max_row) + entry.column;
            deads[index] = entry;
            MarkLot(Lot, entry);
        }
        // Check if lot details are within valid range and update array
        if (lot_entry.row >= 1 && lot_entry.row <= Max_row && lot_entry.column >= 0 && lot_entry.column <= Max_col)
        {
            Lot_index = ((lot_entry.row - 1) * Max_row) + lot_entry.column;
            lotdet[Lot_index] = lot_entry;
        }
    }

    // Main menu loop
    while (1)
    {
        system("cls"); // to clear the screen for better user experience.
        print_text();  // Print additional text (if any)
        sleep(0.5);    // function to introduce delays for better readability and pacing.
        printf("\t\t\t\t\t\tBurial Lot Information System\n\n");
        printf("\t\t\t\t\t\tChoose one of the Options below:\n\n");
        printf("\t\t\t\t\t\t    1 => New Entry\n");
        printf("\t\t\t\t\t\t    2 => Lot Visuals\n");
        printf("\t\t\t\t\t\t    3 => Package Descriptions\n");
        printf("\t\t\t\t\t\t    4 => Update Info\n");
        printf("\t\t\t\t\t\t    5 => Exit\n\n");
        printf("\t\t\t\t\t\t  Option: ");
        scanf("%d", &Menu_Option);
        switch (Menu_Option)
        {
        case 1:            // New entry
            system("cls"); // Clear the screen
            sleep(1);      // Introduce a delay for better user experience
            do
            {
                system("cls");
                sleep(0.5);
                printf("\n\t\t\t\t    ======================>  NEW ENTRY <=========================\n");
                // Display the Lot visuals
                PLot(Lot);
                printf("\n\t\t\t\t    Enter Lot ID (Column-Row): ");
                scanf(" %c-%d", &colid, &row_id); // Get user input for lot ID
                column_id = toupper(colid) - 'A'; // Convert column ID to uppercase

                index = ((row_id - 1) * Max_row) + column_id; // Calculate index

                // Check if the selected lot is valid
                if (column_id < 0 || column_id > Max_col || row_id < 0 || row_id > Max_row)
                {
                    printf("\t\t\t\t    Not an option. Try again.\n\n");
                    sleep(1);
                }
                else if (strcmp(lotdet[index].status, "taken") == 0 && strcmp(deads[index].fullname, "") != 0)
                {
                    printf("\n\t\t\t\t    Lot is already taken\n");
                    sleep(1);
                }
                else
                {
                    system("cls");
                    sleep(0.5);
                    deads[index] = Register(entry_again, curr_year, column_id, row_id); // Register a new entry
                    do
                    {
                        system("cls");
                        sleep(0.5);
                        printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                        printf("\t\t\t\t\t\tChoose one of the Options below:\n\n");
                        printf("\t\t\t\t\t\t    1 => Proceed to Payment\n");
                        printf("\t\t\t\t\t\t    2 => Cancel\n");
                        printf("\n\t\t\t\t\t\t  Option: ");
                        scanf("%d", &Pay_menu); // Get user input for payment menu

                        switch (Pay_menu)
                        {
                        case 1:                                                   // Proceed to payment
                            packagePrice = getPackagePrice(deads[index].package); // Shows the price of each package
                            system("cls");
                            sleep(0.5);
                            printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                            printf("\t\t\t\t\t PACKAGE PRICE >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice);
                            printf("\t\t\t\t\t LOT PRICE     >>>>>>>>>>>>>>> PHP %d\n", lotdet[index].Price);
                            printf("\t\t\t\t\t TOTAL         >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice + lotdet[index].Price);
                            printf("\n\t\t\t\t    =============================================================\n\n");

                            printf("\t\t\t\t\t\t    1 => Confirm\n");
                            printf("\t\t\t\t\t\t    2 => Cancel\n");
                            printf("\n\t\t\t\t\t\t   Option: ");
                            scanf("%d", &confirm_menu);

                            switch (confirm_menu)
                            {
                            case 1:                                    // Confirm
                                MarkLot(Lot, deads[index]);            // mark the lot
                                strcpy(lotdet[index].status, "taken"); // change the status of lot into taken
                                system("cls");
                                sleep(0.5);
                                printf("\n\n\t\t\t\t    =====================>  ENTRY SAVED <========================\n");
                                PLot(Lot);
                                sleep(4);
                                print_rcpt(lotdet[index], deads[index], packagePrice); // print the receipt
                                history(lotdet[index], deads[index], packagePrice);    // add to history
                                sleep(1);
                                printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tYou have succesfully registered...");
                                sleep(1);
                                break;
                            case 2:                   // Cancel
                                deads[index] = empty; // cancel
                                break;
                            default:
                                printf("\t\t\t\t\t\t Not an option, Try Again\n");
                                sleep(1);
                                printf("\n");
                                break;
                            }
                            break;
                        case 2:
                            deads[index] = empty; // Resets entry
                            system("cls");
                            break;
                        default:
                            printf("Not an option, Try Again\n");
                            sleep(1);
                            break;
                        }

                    } while (Pay_menu != 2 && Pay_menu != 1 && Pay_menu != 3);
                    break;
                }
            } while (1);
            break;
        case 2: // Lot Visualization
            system("cls");
            sleep(1.3);
            do
            {
                system("cls");
                PLot(Lot); // Print lot visuals
                printf("\n\t\t\t\t    =====================> LOT VISUALS <=========================\n");
                printf("\n\t\t\t\t\t\t Choose one of the Options below:\n\n");
                printf("\t\t\t\t\t\t    1 => Lot Information\n");
                printf("\t\t\t\t\t\t    2 => Main Menu\n");
                printf("\t\t\t\t\t\t    3 => Exit\n\n");
                printf("\t\t\t\t\t\t  Option: ");
                scanf("%d", &LvisMenu);
                // Get user input for lot visualization menu
                switch (LvisMenu)
                {
                case 1:
                    do
                    {
                        system("cls");
                        PLot(Lot);
                        printf("\n\t\t\t\t    =====================> LOT VISUALS <=========================\n");
                        printf("\n\t\t\t\t    Enter Lot ID (Column-Row): ");
                        scanf(" %c-%d", &colid, &row_id);
                        // Get user input for lot ID
                        column_id = toupper(colid) - 'A';
                        // Convert column ID to uppercase

                        if (column_id < 0 || column_id > Max_col || row_id < 1 || row_id > Max_row) // invalid input
                        {

                            printf("\n\t\t\t\t    Not an option. Try again.");
                            sleep(1.5);
                        }
                        else
                        {
                            system("cls");
                            do
                            {
                                Print_info(column_id, row_id, deads, lotdet);
                                printf("\n\t\t\t\t\t\t Choose one of the Options below:\n\n");
                                printf("\t\t\t\t\t\t    1 => Choose Again\n");
                                printf("\t\t\t\t\t\t    2 => Main Menu\n");
                                printf("\t\t\t\t\t\t    3 => Exit\n\n");
                                printf("\t\t\t\t\t\t  Option: ");

                                scanf("%d", &again);
                                // Get user input for next action
                                switch (again)
                                {
                                case 1:
                                    sleep(1);
                                    break;
                                case 2:
                                    break;
                                case 3:
                                    printf("Exiting Program...\n");
                                    Save(deads, lotdet, ifp, lfp); // Save data
                                    fclose(ifp);                   // Close file pointer
                                    fclose(lfp);                   // Close file pointer
                                    exit(0);                       // Exit program
                                default:
                                    printf("\t\t\t\t\t\t Not an option, Try Again\n");
                                    sleep(1.5);
                                    printf("\n");
                                    break;
                                }
                                if (again == 2)
                                {
                                    break;
                                }

                            } while (again != 2 && again != 1);
                        }

                    } while (again != 2);
                    break;
                case 2:
                    system("cls");
                    break;
                case 3:
                    printf("Exiting Program...\n");
                    Save(deads, lotdet, ifp, lfp);
                    fclose(ifp);
                    fclose(lfp);
                    exit(0);
                default:
                    printf("\t\t\t  Not an option, Try Again\n");
                    sleep(1);
                    break;
                }

                if (again == 2)
                {
                    system("cls");
                    break;
                }

                system("cls");
            } while (LvisMenu != 2 && LvisMenu != 3);

            break;

        case 3: // Package Descriptions
            system("cls");
            sleep(2);
            pkg_des();
            sleep(5);
            do
            {
                system("cls");
                sleep(1);
                printf("\n\n\n\n\n\n\n\n\n\t\t\t\t    =============================================================\n\n");
                printf("\t\t\t\t\t\tChoose one of the Options below:\n\n");
                printf("\t\t\t\t\t\t    1 => Avail a Package\n");
                printf("\t\t\t\t\t\t    2 => Main Menu\n");
                printf("\t\t\t\t\t\t    3 => Exit\n");
                printf("\n\t\t\t\t\t\t  Option: ");
                scanf("%d", &Pckg_menu);
                switch (Pckg_menu)
                {
                case 1: // Avail a package
                    do
                    {
                        system("cls");
                        sleep(1);
                        PLot(Lot);
                        printf("\n\t\t\t\t    Enter Lot ID (Column-Row): ");
                        scanf(" %c-%d", &colid, &row_id);
                        // get user input for lot ID
                        column_id = toupper(colid) - 'A';
                        // get column id from int to char
                        index = ((row_id - 1) * Max_row) + column_id;

                        if (column_id < 0 || column_id > Max_col || row_id < 0 || row_id > Max_row) // invalid input
                        {
                            printf("\t\t\t\t    Not an option. Try again.\n\n");
                            sleep(1);
                        }
                        else if (strcmp(lotdet[index].status, "taken") == 0 && strcmp(deads[index].fullname, "") != 0) // if lot is taken
                        {
                            printf("\n\t\t\t\t    Lot is already taken\n");
                            sleep(1);
                        }
                        else // else means available
                        {
                            system("cls");
                            sleep(0.5);
                            deads[index] = Register(entry_again, curr_year, column_id, row_id);
                            do
                            {
                                system("cls");
                                sleep(0.5);
                                printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                                printf("\t\t\t\t\t\tChoose one of the Options below:\n\n");
                                printf("\t\t\t\t\t\t    1 => Proceed to Payment\n");
                                printf("\t\t\t\t\t\t    2 => Cancel\n");
                                printf("\n\t\t\t\t\t\t  Option: ");
                                scanf("%d", &Pay_menu);

                                switch (Pay_menu)
                                {
                                case 1:
                                    packagePrice = getPackagePrice(deads[index].package);
                                    do
                                    {
                                        system("cls");
                                        sleep(0.5);
                                        // Display payment details
                                        printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                                        printf("\t\t\t\t\t PACKAGE PRICE >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice);
                                        printf("\t\t\t\t\t LOT PRICE     >>>>>>>>>>>>>>> PHP %d\n", lotdet[index].Price);
                                        printf("\t\t\t\t\t TOTAL         >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice + lotdet[index].Price);
                                        printf("\n\t\t\t\t    =============================================================\n\n");

                                        printf("\t\t\t\t\t\t    1 => Confirm\n");
                                        printf("\t\t\t\t\t\t    2 => Cancel\n");
                                        printf("\n\t\t\t\t\t\t   Option: ");
                                        scanf("%d", &confirm_menu);

                                        switch (confirm_menu)
                                        {
                                        case 1: // confirm payment
                                            MarkLot(Lot, deads[index]);
                                            strcpy(lotdet[index].status, "taken");
                                            system("cls");
                                            sleep(0.5);
                                            printf("\n\n\t\t\t\t    =====================>  ENTRY SAVED <========================\n");
                                            PLot(Lot);
                                            sleep(4);
                                            print_rcpt(lotdet[index], deads[index], packagePrice);
                                            history(lotdet[index], deads[index], packagePrice);
                                            sleep(1);
                                            printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tYou have succesfully registered...");
                                            sleep(1);
                                            break;
                                        case 2: // cancel payment
                                            deads[index] = empty;
                                            break;
                                        default:
                                            printf("\t\t\t\t\t\t Not an option, Try Again\n");
                                            sleep(1);
                                            printf("\n");
                                            break;
                                        }

                                    } while (confirm_menu != 1 && confirm_menu != 2);
                                    break;
                                case 2:
                                    deads[index] = empty;
                                    system("cls");
                                    break;
                                default:
                                    printf("\t\t\t\t\t\t Not an option, Try Again\n");
                                    sleep(1);
                                    break;
                                }

                            } while (Pay_menu != 2 && Pay_menu != 1 && Pay_menu != 3);
                            // Loop until valid payment option is chosen
                            break;
                        }
                    } while (1);
                case 2: // Main menu
                    system("cls");
                    sleep(2);
                    break;
                case 3: // exit program
                    Save(deads, lotdet, ifp, lfp);
                    fclose(ifp);
                    fclose(lfp);
                    printf("Exiting Program...\n");
                    exit(0);
                default:
                    printf("\t\t\t\t\t\t Not an option, Try Again\n");
                    sleep(1);
                    break;
                }
            } while (Pckg_menu != 1 && Pckg_menu != 2);
            break;
        case 5: // Exit Program
            printf("Exiting Program...\n");
            Save(deads, lotdet, ifp, lfp);
            fclose(ifp);
            fclose(lfp);
            exit(0);
        case 4: //
            system("cls");
            sleep(1);
            do
            {
                system("cls");
                sleep(0.5);
                printf("\n\t\t\t\t    ======================>  UPDATE INFO <=======================\n");
                // Display the Lot visuals
                PLot(Lot);
                // prompt user for Lot ID input
                printf("\n\t\t\t\t    Enter Lot ID (Column-Row): ");
                scanf(" %c-%d", &colid, &row_id);
                column_id = toupper(colid) - 'A';
                // calculate the index of the corresponding input
                index = ((row_id - 1) * Max_row) + column_id;

                if (column_id < 0 || column_id > Max_col || row_id < 1 || row_id > Max_row) // Invalid or out of range input
                {
                    printf("\t\t\t\t    Not an option. Try again.\n\n");
                    sleep(1);
                }
                else if (strcmp(lotdet[index].status, "taken") == 0 && strcmp(deads[index].fullname, "") != 0) // if valid update info
                {
                    update(index, colid, row_id, deads, empty, lotdet, ifp, lfp, curr_year);
                    break;
                }
                else // else mea slot is free
                {
                    printf("\n\t\t\t\t    Lot is Free\n");
                    sleep(1);
                }
            } while (1);

            break;
        default: // Loop if Not an Option
            printf("\t\t\t\t\t\t Not an option, Try Again\n");
            sleep(1);
            break;
        }
    }

    Save(deads, lotdet, ifp, lfp);
    fclose(ifp);
    fclose(lfp);

    return 0;
}

// Function to read entry details from file
Info Read_Entry(FILE *fileptr)
{
    Info entry; // Declare Info struct variable
    char coletter;
    // Read lot coordinates
    fscanf(fileptr, " %c-%d", &coletter, &entry.row);
    entry.column = toupper(coletter) - 'A';     // Convert column letter to uppercase and calculate column index
    fscanf(fileptr, " %[^\n]", entry.fullname); // Read full name
    fscanf(fileptr, "%d", &entry.BirthMonth);   // Read birth month
    fscanf(fileptr, "%d", &entry.BirthDay);     // Read birth day
    fscanf(fileptr, "%d", &entry.BirthYear);    // Read birth year
    fscanf(fileptr, "%d", &entry.DeathMonth);   // Read death month
    fscanf(fileptr, "%d", &entry.DeathDay);     // Read death day
    fscanf(fileptr, "%d", &entry.DeathYear);    // Read death year
    fscanf(fileptr, " %c", &entry.package);     // Read package
    fscanf(fileptr, " %[^\n]", entry.qoute);    // Read quote

    return entry; // Return entry details
}
Lot_Info Lot_details(FILE *lotptr)
{
    Lot_Info loentr; // Declare Lot_Info struct variable
    char lettercol;

    fscanf(lotptr, " %c-%d", &lettercol, &loentr.row); // Read lot coordinates
    loentr.column = toupper(lettercol) - 'A';          // Convert column letter to uppercase and calculate column index
    fscanf(lotptr, "%f", &loentr.size1);               // Read size 1
    fscanf(lotptr, "%f", &loentr.size2);               // Read size 2
    fscanf(lotptr, "%d", &loentr.Price);               // Read price
    fscanf(lotptr, " %[^\n]", loentr.status);          // Read status

    return loentr; // Return lot details
}

void MarkLot(char Lot[][Max_col], Info entry)
{
    Lot[entry.row - 1][entry.column] = 'X'; // Mark the lot based on 1-based indexing
}
// Function to display available lots
void PLot(char Lot[][Max_col])
{
    printf("\n\n\t\t\t\t       A     B     C     D     E     F     G     H     I     J\n\n");
    printf("\t\t\t\t    =============================================================\n");
    for (int i = 0; i < Max_row; i++)
    {
        printf("\t\t\t\t%3d |", i + 1);
        for (int j = 0; j < Max_col; j++)
        {
            printf("  %c  |", Lot[i][j]);
        }
        printf("\n\t\t\t\t    =============================================================\n");
    }
}

void cMonth(int num_month, char *monthptr) // input number based on months of the year
{

    switch (num_month)
    {
    case 1:
        strcpy(monthptr, "January");
        break;
    case 2:
        strcpy(monthptr, "February");
        break;
    case 3:
        strcpy(monthptr, "March");
        break;
    case 4:
        strcpy(monthptr, "April");
        break;
    case 5:
        strcpy(monthptr, "May");
        break;
    case 6:
        strcpy(monthptr, "June");
        break;
    case 7:
        strcpy(monthptr, "July");
        break;
    case 8:
        strcpy(monthptr, "August");
        break;
    case 9:
        strcpy(monthptr, "September");
        break;
    case 10:
        strcpy(monthptr, "October");
        break;
    case 11:
        strcpy(monthptr, "November");
        break;
    case 12:
        strcpy(monthptr, "December");
        break;
    default:
        break;
    }
}

void Print_info(int col, int row, Info deads[MAX_ENT], Lot_Info lotdet[MAX_ENT])
{
    int index;
    char colleter;
    char bmonth[MAX_STR];
    char dmonth[MAX_STR];
    char stats[MAX_STR];

    // Convert column index to corresponding letter
    colleter = 'A' + col;

    // Calculate index in the array based on row and column
    index = ((row - 1) * Max_row) + col;
    system("cls");
    sleep(1);
    // Convert birth month and death month integers to strings
    cMonth(deads[index].BirthMonth, bmonth);
    cMonth(deads[index].DeathMonth, dmonth);

    // Check if lot is taken or free and assign status accordingly
    if (strcmp(lotdet[index].status, "taken") == 0)
    {
        strcpy(stats, "Taken");
    }
    else
    {
        strcpy(stats, "Free");
    }

    printf("\n\n\n\n\n\n\n");
    // Display lot information based on whether it's taken or not
    if (strcmp(deads[index].fullname, "") != 0 && strcmp(lotdet[index].status, "taken") == 0)
    {
        printf("\n\t\t\t\t                          LOT INFORMATION");
        printf("\n\t\t\t\t    =============================================================\n\n");
        printf("\t\t\t\t\t LOT ID        >>>>>>>>>>>>>>> %c-%d\n", colleter, deads[index].row);
        printf("\t\t\t\t\t NAME          >>>>>>>>>>>>>>> %s\n", deads[index].fullname);
        printf("\t\t\t\t\t DATE OF BIRTH >>>>>>>>>>>>>>> %s /%3d/%d\n", bmonth, deads[index].BirthDay, deads[index].BirthYear);
        printf("\t\t\t\t\t DATE OF DEATH >>>>>>>>>>>>>>> %s /%3d/%d\n", dmonth, deads[index].DeathDay, deads[index].DeathYear);
        printf("\t\t\t\t\t PACKAGE       >>>>>>>>>>>>>>> %c\n", deads[index].package);
        printf("\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> %s\n", deads[index].qoute);
        printf("\n\t\t\t\t    =============================================================\n");
    }
    else
    {
        printf("\n\t\t\t\t                          LOT INFORMATION");
        printf("\n\t\t\t\t    =============================================================\n\n");
        printf("\t\t\t\t\t LOT ID        >>>>>>>>>>>>>>> %c-%d\n", colleter, lotdet[index].row);
        printf("\t\t\t\t\t SIZE          >>>>>>>>>>>>>>> %.2fm x %.2fm\n", lotdet[index].size1, lotdet[index].size2);
        printf("\t\t\t\t\t PRICE         >>>>>>>>>>>>>>> PHP %d\n", lotdet[index].Price);
        printf("\t\t\t\t\t STATUS        >>>>>>>>>>>>>>> %s\n", stats);
        printf("\n\t\t\t\t    =============================================================\n");
    }
}

Info Register(Info entry_again, int curr_year, int column, int row)
{
    int check;
    char col = 'A' + column;
    entry_again.column = column;         // Set column value for the entry
    entry_again.row = row;               // Set row value for the entry
    system("cls");                       // Clear screen
    entry_again.package = Choose_pckg(); // Get the chosen package for the entry
    sleep(1);                            // Wait for 1 second
    char bmont[MAX_STR];
    char dmont[MAX_STR];
    do
    {
        system("cls");
        sleep(1);
        printf("\n\n\n\n\t\t\t\t                          LOT INFORMATION");
        printf("\n\t\t\t\t    =============================================================\n\n");
        printf("\t\t\t\t\t NAME          >>>>>>>>>>>>>>> ");
        scanf(" %[^\n]", entry_again.fullname);

        // Get user input for birth month, day, and year
        do
        {
            printf("\n\t\t\t\t\t BIRTH MONTH   >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.BirthMonth);

            if (entry_again.BirthMonth < 1 || entry_again.BirthMonth > 12)
            {
                printf("\n\t\t\t>>>>>>>>>>>>>>>Invalid month. Please enter a value between 1 and 12<<<<<<<<<<<<<<<<\n");
                sleep(1);
            }

        } while (entry_again.BirthMonth < 1 || entry_again.BirthMonth > 12);

        // Repeat for birth day
        do
        {
            printf("\n\t\t\t\t\t BIRTH DAY     >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.BirthDay);

            if (entry_again.BirthDay < 1 || entry_again.BirthDay > 31)
            {
                printf("\n\t\t\t>>>>>>>>>>>>>>> Invalid Day. Please enter a value between 1 and 31 <<<<<<<<<<<<<<<<\n");
            }

        } while (entry_again.BirthDay < 1 || entry_again.BirthDay > 31);

        // Repeat for birth year
        do
        {
            printf("\n\t\t\t\t\t BIRTH YEAR    >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.BirthYear);

            if (entry_again.BirthYear < 1800 || entry_again.BirthYear > curr_year)
            {
                printf("\n\t\t\t>>>>>>>>>Invalid Year. Please enter a value between 1800 and Current Year<<<<<<<<<\n");
            }

        } while (entry_again.BirthYear < 1800 || entry_again.BirthYear > curr_year);

        // Repeat for death month
        do
        {
            printf("\n\t\t\t\t\t DEATH MONTH   >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.DeathMonth);

            if (entry_again.DeathMonth < 1 || entry_again.DeathMonth > 12)
            {
                printf("\n\t\t\t>>>>>>>>>>>>>>>Invalid month. Please enter a value between 1 and 12<<<<<<<<<<<<<<<<\n");
            }
        } while (entry_again.DeathMonth < 1 || entry_again.DeathMonth > 12);

        // Repeat for death day
        do
        {
            printf("\n\t\t\t\t\t DEATH DAY     >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.DeathDay);

            if (entry_again.DeathDay < 1 || entry_again.DeathDay > 31)
            {
                printf("\n\t\t\t>>>>>>>>>>>>>>> Invalid Day. Please enter a value between 1 and 31 <<<<<<<<<<<<<<<<\n");
            }

        } while (entry_again.DeathDay < 1 || entry_again.DeathDay > 31);

        // repeat for death year
        do
        {
            printf("\n\t\t\t\t\t DEATH YEAR    >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.DeathYear);

            if (entry_again.DeathYear < 1800 || entry_again.DeathYear > curr_year)
            {
                printf("\n\t\t\t>>>>>>>>>Invalid Year. Please enter a value between 1800 and Current Year<<<<<<<<<\n");
            }

        } while (entry_again.DeathYear < 1800 || entry_again.DeathYear > curr_year);

        printf("\n\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> ");
        scanf(" %[^\n]", entry_again.qoute);

        cMonth(entry_again.BirthMonth, bmont);
        cMonth(entry_again.DeathMonth, dmont);

        // loop for confirming or editing details
        do
        {
            system("cls");
            sleep(1);
            printf("\n\n\n\t\t\t\t\t\t Please confirm the details below:\n\n");
            printf("\n\t\t\t\t                          LOT INFORMATION");
            printf("\n\t\t\t\t    =============================================================\n\n");
            printf("\t\t\t\t\t LOT ID        >>>>>>>>>>>>>>> %c-%d\n", col, entry_again.row);
            printf("\t\t\t\t\t NAME          >>>>>>>>>>>>>>> %s\n", entry_again.fullname);
            printf("\t\t\t\t\t DATE OF BIRTH >>>>>>>>>>>>>>> %s / %02d /%d\n", bmont, entry_again.BirthDay, entry_again.BirthYear);
            printf("\t\t\t\t\t DATE OF DEATH >>>>>>>>>>>>>>> %s / %02d /%d\n", dmont, entry_again.DeathDay, entry_again.DeathYear);
            printf("\t\t\t\t\t PACKAGE       >>>>>>>>>>>>>>> %c\n", entry_again.package);
            printf("\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> %s\n", entry_again.qoute);
            printf("\n\t\t\t\t    =============================================================\n");

            printf("\n\t\t\t\t\t\t Choose one of the Options below:\n\n");
            printf("\t\t\t\t\t\t    1 => Confirm\n");
            printf("\t\t\t\t\t\t    2 => Edit\n");
            printf("\n\t\t\t\t\t\t  Option: ");
            scanf("%d", &check);
            // Handle user's choice
            switch (check)
            {
            case 1:
                return entry_again;
                break;
            case 2:
                break;
            default:
                printf("\n\t\t\t\t\t\t Invalid Input. Try Again\n\n");
                sleep(0.5);
                break;
            }
            if (check == 2)
                break;

        } while (check != 1); // Continue loop until confirmation

    } while (check == 2); // Continue loop if user chooses to edit
}
char Choose_pckg()
{
    char choice;
    while (1)
    {
        system("cls");
        sleep(1);
        pkg_des();
        printf("\t\t\t\t    Choose a package: ");

        if (scanf(" %c", &choice) == 1)
        {
            switch (choice)
            {
            case 'A':
            case 'a':
                system("cls");
                sleep(1);
                printf("\n\n\n\n\t\t\t\t    =======================>NORMAL PACKAGE<======================");
                printf("\n\n\t\t\t\t\tThis package includes the burial plot itself, the\n\t\t\t\t\topening and closing of the grave and a grave care\n\t\t\t\t\tof once every 6 months.\n\n");
                printf("\n\t\t\t\t    =============================================================\n");
                sleep(6);
                return 'A'; // Exit the function after a valid choice is made
            case 'B':
            case 'b':
                system("cls");
                sleep(1);
                printf("\n\n\n\n\t\t\t\t    ========================SILVER PACKAGE=======================");
                printf("\n\n\t\t\t\t\tThis package is one level above the Normal package.\n\t\t\t\t\tThis package offers a more enhanced service  than\n\t\t\t\t\tthe previous level such as additional customization\n\t\t\t\t\toption, upgraded materials for the headstone or marker\n\t\t\t\t\tand a more frequent grave care of once every 3 months.\n\n");
                printf("\n\t\t\t\t    =============================================================\n");
                sleep(6);
                return 'B'; // Exit the function after a valid choice is made
            case 'C':
            case 'c':
                system("cls");
                sleep(1);
                printf("\n\n\n\n\t\t\t\t    =======================PREMIUM PACKAGE=======================");
                printf("\n\n\t\t\t\t\tIn addition to the features of the premium package,\n\t\t\t\t\tthe Silver package may include amenities like occasional\n\t\t\t\t\tfloral arrangements, access to support services for the\n\t\t\t\t\tbereaved, a selection of standard markers or headstones\n\t\t\t\t\twith basic engraving options, priority scheduling for\n\t\t\t\t\tcemetery visits or special events, ensuring ease of\n\t\t\t\t\ttaccess and convenience for family members and visitors.\n\n");
                printf("\t\t\t\t    =============================================================\n");
                sleep(6);
                return 'C'; // Exit the function after a valid choice is made
            case 'D':
            case 'd':
                system("cls");
                sleep(1);
                printf("\n\n\n\n\t\t\t\t    =========================GOLD PACKAGE========================");
                printf("\n\n\t\t\t\t\tThe Gold package includes all the benefits of the Silver\n\t\t\t\t\tpackage and offers the additional feature of a mausoleum\n\t\t\t\t\tfor above-ground interment. This may include a designated\n\t\t\t\t\tspace within the mausoleum, personalized inscriptions,\n\t\t\t\t\tongoing maintenance of the structure and exclusive access\n\t\t\t\t\tto certain areas or facilities within the cemetery grounds\n\t\t\t\t\tfor private ceremonies or gatherings.\n\n");
                printf("\t\t\t\t    =============================================================\n");
                return 'D'; // Exit the function after a valid choice is made
                sleep(6);
            default:
                printf("Invalid choice. Please choose from A, B, C, or D.\n");
                break;
            }
        }
        else
        {
            printf("\n\nFailed to read input. Please try again.\n");
            sleep(4);
        }
    }
}

// get price of eacg package
float getPackagePrice(char package)
{
    float price = 0.0;
    switch (package)
    {
    case 'A':
        price = 8000.00;
        return price;
        break;
    case 'B':
        price = 15000.00;
        return price;
        break;
    case 'C':
        price = 22000.00;
        return price;
        break;
    case 'D':
        price = 500000.00;
        return price;
        break;
    default:
        printf("Invalid package selection.\n");
        break;
    }
}

void Save(Info holder[MAX_ENT], Lot_Info lot[MAX_ENT], FILE *tp, FILE *lp) // saves the data stored in the arrays of structures "
{
    if (tp == NULL || lp == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    else
    {
        fseek(tp, 0, SEEK_SET);           // Set file pointers to the beginning of the files
        for (int i = 0; i < MAX_ENT; i++) // Loop through entries in the holder array
        {
            if (strcmp(holder[i].fullname, "") != 0) // Check if the entry is not empty
            {
                char colmn = 'A' + holder[i].column; // Convert column index to letter representation
                fprintf(tp, "%c-%d\n", colmn, holder[i].row);
                fprintf(tp, "%s\n", holder[i].fullname);
                fprintf(tp, "%d\n", holder[i].BirthMonth);
                fprintf(tp, "%d\n", holder[i].BirthDay);
                fprintf(tp, "%d\n", holder[i].BirthYear);
                fprintf(tp, "%d\n", holder[i].DeathMonth);
                fprintf(tp, "%d\n", holder[i].DeathDay);
                fprintf(tp, "%d\n", holder[i].DeathYear);
                fprintf(tp, "%c\n", holder[i].package);
                fprintf(tp, "%s\n\n", holder[i].qoute);
            }
        }

        // Set file pointer to the beginning of the lot information file
        fseek(lp, 0, SEEK_SET);
        // Loop through entries in the lot array
        for (int j = 0; j < MAX_ENT; j++)
        {
            char lcol = 'A' + lot[j].column;
            // Write lot details to the text file
            fprintf(lp, "%c-%d\n", lcol, lot[j].row);
            fprintf(lp, "%.2f\n", lot[j].size1);
            fprintf(lp, "%.2f\n", lot[j].size2);
            fprintf(lp, "%d\n", lot[j].Price);
            fprintf(lp, "%s\n\n", lot[j].status);
        }
    }
}

// Welcome to BLIS ASCII ART
void print_text()
{
    printf("\n\n\n");
    printf("\t\t\t\t================================================================\n\n");
    printf("\t\t\t\t\t\\ \\    / /| __|| |    / __| / _ \\ |  \\/  || __| \n");
    printf("\t\t\t\t\t \\ \\/\\/ / | _| | |__ | (__ | (_) || |\\/| || _|  \n");
    printf("\t\t\t\t\t  \\_/\\_/  |___||____| \\___| \\___/ |_|  |_||___| \n");
    printf("\t\t\t\t\t\t\t   _____   ___  \n");
    printf("\t\t\t\t\t\t\t  |_   _| / _ \\ \n");
    printf("\t\t\t\t\t\t\t    | |  | (_) |\n");
    printf("\t\t\t\t\t\t\t    |_|   \\___/ \n");
    printf("\t\t\t\t\t\t       ___  _     ___  ___ \n");
    printf("\t\t\t\t\t\t      | _ )| |   |_ _|/ __|\n");
    printf("\t\t\t\t\t\t      | _ \\| |__  | | \\__ \\\n");
    printf("\t\t\t\t\t\t      |___/|____||___||___/\n\n");
    printf("\t\t\t\t================================================================\n");
}
// a description of the packages
void pkg_des()
{
    printf("\n\t\t\t\t    =====================PACKAGE DESCRIPTIONS====================");
    printf("\n\t\t\t\t    =============================================================\n");
    printf("\t\t\t\t    A => NORMAL PACKAGE [PHP 8000.00]\n");
    printf("\n\t\t\t\t\tThis package includes the burial plot itself, the\n\t\t\t\t\topening and closing of the grave and a grave care\n\t\t\t\t\tof once every 6 months.\n");
    printf("\t\t\t\t    =============================================================\n");
    printf("\t\t\t\t    B => SILVER PACKAGE [PHP 18000.00]\n");
    printf("\n\t\t\t\t\tThis package is one level above the Normal package.\n\t\t\t\t\tThis package offers a more enhanced service  than\n\t\t\t\t\tthe previous level such as additional customization\n\t\t\t\t\toption, upgraded materials for the headstone or marker\n\t\t\t\t\tand a more frequent grave care of once every 3 months.\n");
    printf("\t\t\t\t    =============================================================\n");
    printf("\n\t\t\t\t    C => PREMIUM PACKAGE [PHP 28000.00]\n");
    printf("\n \t\t\t\t\tIn addition to the features of the premium package,\n\t\t\t\t\tthe Silver package may include amenities like occasional\n\t\t\t\t\tfloral arrangements, access to support services for the\n\t\t\t\t\tbereaved, a selection of standard markers or headstones\n\t\t\t\t\twith basic engraving options, priority scheduling for\n\t\t\t\t\tcemetery visits or special events, ensuring ease of\n\t\t\t\t\ttaccess and convenience for family members and visitors.\n");
    printf("\t\t\t\t    =============================================================\n");
    printf("\t\t\t\t    D => GOLD PACKAGE [PHP 250000.00]\n");
    printf("\n\t\t\t\t\tThe Gold package includes all the benefits of the Silver\n\t\t\t\t\tpackage and offers the additional feature of a mausoleum\n\t\t\t\t\tfor above-ground interment. This may include a designated\n\t\t\t\t\tspace within the mausoleum, personalized inscriptions,\n\t\t\t\t\tongoing maintenance of the structure and exclusive access\n\t\t\t\t\tto certain areas or facilities within the cemetery grounds\n\t\t\t\t\tfor private ceremonies or gatherings.\n");
    printf("\t\t\t\t    =============================================================\n");
}

// A function that writes receipts
void print_rcpt(Lot_Info lotdet, Info deads, float packagePrice)
{
    // open a file pointer
    FILE *rfp;
    // using the file pointer open a file for wrting
    rfp = fopen("Reciept.in", "wt");
    // geting the current time
    time_t clock = time(NULL);
    struct tm date = *localtime(&clock);
    char bmonth[MAX_STR], dmonth[MAX_STR];
    // Since the .column member is int, change it into char
    char colleter = 'A' + deads.column;

    system("cls");
    sleep(1);
    // get corresponding month to word e.g. 1 = January
    cMonth(deads.BirthMonth, bmonth);
    cMonth(deads.DeathMonth, dmonth);

    // print the reciept
    fprintf(rfp, "\n\n\t\t   =============================================================\n\n");
    fprintf(rfp, "\t\t\t\t\t\t  _        _    _   _   _     _   _  __\n");
    fprintf(rfp, "\t\t\t\t\t\t /_) /  / /_`  /_/ /_` / ` / /_` /_/ / \n");
    fprintf(rfp, "\t\t\t\t\t\t/_) /_,/ ._/  / \\ /_, /_, / /_, /   /  \n");
    fprintf(rfp, "\t\t\t\t\t                                      \n");
    fprintf(rfp, "\n\t\t   =============================================================");
    fprintf(rfp, "\n\t\t\t\t    %02d/%02d/%02d \t\t\t\t\t\t    %02d:%02d\n", date.tm_mon, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min);

    fprintf(rfp, "\n\t\t\t\t\t LOT ID        >>>>>>>>>>>>>>> %c-%d\n", colleter, deads.row);
    fprintf(rfp, "\t\t\t\t\t NAME          >>>>>>>>>>>>>>> %s\n", deads.fullname);
    fprintf(rfp, "\t\t\t\t\t DATE OF BIRTH >>>>>>>>>>>>>>> %s /%02d/%d\n", bmonth, deads.BirthDay, deads.BirthYear);
    fprintf(rfp, "\t\t\t\t\t DATE OF DEATH >>>>>>>>>>>>>>> %s /%02d/%d\n", dmonth, deads.DeathDay, deads.DeathYear);
    fprintf(rfp, "\t\t\t\t\t PACKAGE       >>>>>>>>>>>>>>> %c\n", deads.package);
    fprintf(rfp, "\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> %s\n", deads.qoute);
    fprintf(rfp, "\t\t\t\t\t PACKAGE PRICE >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice);
    fprintf(rfp, "\t\t\t\t\t LOT PRICE     >>>>>>>>>>>>>>> PHP %d\n", lotdet.Price);
    fprintf(rfp, "\t\t\t\t\t TOTAL         >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice + lotdet.Price);
    fprintf(rfp, "\n\t\t    =============================================================\n\n");

    fclose(rfp);
}

void history(Lot_Info lotdet, Info deads, float packagePrice)
{
    // A function just like print-rcpt but instead of write it appends
    FILE *hfp;
    hfp = fopen("History.in", "a");
    time_t clock = time(NULL);
    struct tm date = *localtime(&clock);
    char bmonth[MAX_STR], dmonth[MAX_STR];
    char colleter = 'A' + deads.column;
    cMonth(deads.BirthMonth, bmonth);
    cMonth(deads.DeathMonth, dmonth);

    cMonth(deads.BirthMonth, bmonth);
    cMonth(deads.DeathMonth, dmonth);
    fprintf(hfp, "\t\t\t\t    %02d/%02d/%02d \t\t\t\t\t\t    %02d:%02d\n", date.tm_mon, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min);

    fprintf(hfp, "\n\t\t\t\t\t LOT ID        >>>>>>>>>>>>>>> %c-%d\n", colleter, deads.row);
    fprintf(hfp, "\t\t\t\t\t NAME          >>>>>>>>>>>>>>> %s\n", deads.fullname);
    fprintf(hfp, "\t\t\t\t\t DATE OF BIRTH >>>>>>>>>>>>>>> %s /%02d/%d\n", bmonth, deads.BirthDay, deads.BirthYear);
    fprintf(hfp, "\t\t\t\t\t DATE OF DEATH >>>>>>>>>>>>>>> %s /%02d/%d\n", dmonth, deads.DeathDay, deads.DeathYear);
    fprintf(hfp, "\t\t\t\t\t PACKAGE       >>>>>>>>>>>>>>> %c\n", deads.package);
    fprintf(hfp, "\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> %s\n", deads.qoute);
    fprintf(hfp, "\t\t\t\t\t PACKAGE PRICE >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice);
    fprintf(hfp, "\t\t\t\t\t LOT PRICE     >>>>>>>>>>>>>>> PHP %d\n", lotdet.Price);
    fprintf(hfp, "\t\t\t\t\t TOTAL         >>>>>>>>>>>>>>> PHP %.2f\n", packagePrice + lotdet.Price);
    fprintf(hfp, "\n\t\t    =============================================================\n");

    fclose(hfp);
}

void update(int index, char colid, int row_id, Info deads[MAX_ENT], Info empty, Lot_Info lotdet[MAX_ENT], FILE *ifp, FILE *lfp, int curr_year)
{
    Info ifcancel = deads[index]; // have a copy of the struct data
    int option, check;
    char name[MAX_STR];
    int month;
    int day;
    int year;
    char packg;
    char qoute[MAX_STR];
    char bmont[MAX_STR];
    char dmont[MAX_STR];
    char b2mont[MAX_STR];
    char d2mont[MAX_STR];

    // int to words of month 1 = january
    cMonth(deads[index].BirthMonth, bmont);
    cMonth(deads[index].DeathMonth, dmont);
    do
    {

        do
        {
            system("cls");
            sleep(1);
            // print current info
            printf("\n\n\n\n\t\t\t\t                          INFORMATION UPDATE");
            printf("\n\t\t\t\t    =============================================================\n\n");
            printf("\t\t\t\t\t LOT ID        >>>>>>>>>>>>>>> %c-%d\n", colid, deads[index].row);
            printf("\t\t\t\t\t NAME          >>>>>>>>>>>>>>> %s\n", deads[index].fullname);
            printf("\t\t\t\t\t DATE OF BIRTH >>>>>>>>>>>>>>> %s / %02d /%d\n", bmont, deads[index].BirthDay, deads[index].BirthYear);
            printf("\t\t\t\t\t DATE OF DEATH >>>>>>>>>>>>>>> %s / %02d /%d\n", dmont, deads[index].DeathDay, deads[index].DeathYear);
            printf("\t\t\t\t\t PACKAGE       >>>>>>>>>>>>>>> %c\n", deads[index].package);
            printf("\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> %s\n", deads[index].qoute);
            printf("\n\t\t\t\t    =============================================================\n");
            // prompt th user on what to update
            printf("\t\t\t\t\t\tChoose what to update:\n\n");
            printf("\t\t\t\t\t\t    1 => Name\n");
            printf("\t\t\t\t\t\t    2 => Date of Birth\n");
            printf("\t\t\t\t\t\t    3 => Date of Death\n");
            printf("\t\t\t\t\t\t    4 => Qoute\n");
            printf("\t\t\t\t\t\t    5 => Package\n");
            printf("\t\t\t\t\t\t    6 => Clear Lot\n\n");
            printf("\t\t\t\t\t\t  Option: ");
            scanf("%d", &option);

            switch (option)
            {
            case 1:
                system("cls");
                sleep(0.5);
                printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                printf("\t\t\t\t\t OLD NAME          >>>>>>>>>>>>>>> %s", deads[index].fullname);
                printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                printf("\t\t\t\t\t NEW NAME          >>>>>>>>>>>>>>> ");
                scanf(" %[^\n]", name);
                strcpy(deads[index].fullname, name);
                break;
            case 2:
                system("cls");
                sleep(0.5);
                printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                printf("\t\t\t\t\tOLD DATE OF BIRTH >>>>>>>>>>>>>>> %s / %02d /%d\n", bmont, deads[index].BirthDay, deads[index].BirthYear);
                do
                {
                    printf("\n\n\t\t\t\t    =============================================================\n\n");
                    printf("\n\t\t\t\t\t NEW BIRTH MONTH   >>>>>>>>>>>>>>> ");
                    scanf("%d", &month);

                    if (month < 1 || month > 12)
                    {
                        printf("\n\t\t\t>>>>>>>>>>>>>>>Invalid month. Please enter a value between 1 and 12<<<<<<<<<<<<<<<<\n");
                        sleep(1);
                    }
                    else
                    {
                        deads[index].BirthMonth = month;
                    }
                } while (month < 1 || month > 12);

                do
                {
                    printf("\n\n\t\t\t\t    =============================================================\n\n");
                    printf("\n\t\t\t\t\t BIRTH DAY     >>>>>>>>>>>>>>> ");
                    scanf("%d", &day);

                    if (day < 1 || day > 31)
                    {
                        printf("\n\t\t\t>>>>>>>>>>>>>>> Invalid Day. Please enter a value between 1 and 31 <<<<<<<<<<<<<<<<\n");
                    }
                    else
                    {
                        deads[index].BirthDay = day;
                    }

                } while (day < 1 || day > 31);

                do
                {
                    printf("\n\n\t\t\t\t    =============================================================\n\n");
                    printf("\n\t\t\t\t\t BIRTH YEAR    >>>>>>>>>>>>>>> ");
                    scanf("%d", &year);

                    if (year < 1800 || year > curr_year)
                    {
                        printf("\n\t\t\t>>>>>>>>>Invalid Year. Please enter a value between 1800 and Current Year<<<<<<<<<\n");
                    }
                    else
                    {
                        deads[index].BirthYear = year;
                    }
                } while (year < 1800 || year > curr_year);
                break;
            case 3:
                system("cls");
                sleep(0.5);
                printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                printf("\t\t\t\t\t OLD DATE OF DEATH >>>>>>>>>>>>>>> %s / %02d /%d\n", dmont, deads[index].DeathDay, deads[index].DeathYear);
                do
                {
                    printf("\n\n\t\t\t\t    =============================================================\n\n");
                    printf("\n\t\t\t\t\t DEATH MONTH   >>>>>>>>>>>>>>> ");
                    scanf("%d", &month);

                    if (month < 1 || month > 12)
                    {
                        printf("\n\t\t\t>>>>>>>>>>>>>>>Invalid month. Please enter a value between 1 and 12<<<<<<<<<<<<<<<<\n");
                        sleep(1);
                    }
                    else
                    {
                        deads[index].DeathMonth = month;
                    }
                } while (month < 1 || month > 12);

                do
                {
                    printf("\n\n\t\t\t\t    =============================================================\n\n");
                    printf("\n\t\t\t\t\t DEATH DAY     >>>>>>>>>>>>>>> ");
                    scanf("%d", &day);

                    if (day < 1 || day > 31)
                    {
                        printf("\n\t\t\t>>>>>>>>>>>>>>> Invalid Day. Please enter a value between 1 and 31 <<<<<<<<<<<<<<<<\n");
                    }
                    else
                    {
                        deads[index].DeathDay = day;
                    }

                } while (day < 1 || day > 31);
                do
                {
                    printf("\n\n\t\t\t\t    =============================================================\n\n");
                    printf("\n\t\t\t\t\t DEATH YEAR    >>>>>>>>>>>>>>> ");
                    scanf("%d", &year);

                    if (year < 1800 || year > curr_year)
                    {
                        printf("\n\t\t\t>>>>>>>>>Invalid Year. Please enter a value between 1800 and Current Year<<<<<<<<<\n");
                    }
                    else
                    {
                        deads[index].DeathYear = year;
                    }

                } while (year < 1800 || year > curr_year);

                break;
            case 4:
                system("cls");
                sleep(0.5);
                printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                printf("\t\t\t\t\t OLD QOUTE         >>>>>>>>>>>>>>> %s\n", deads[index].qoute);
                printf("\n\n\t\t\t\t    =============================================================\n\n");
                printf("\n\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> ");
                scanf(" %[^\n]", qoute);
                strcpy(deads[index].qoute, qoute);
                break;
            case 5:
                system("cls");
                sleep(0.5);
                printf("\n\n\n\n\t\t\t\t    =============================================================\n\n");
                printf("\t\t\t\t\t OLD PACKAGE       >>>>>>>>>>>>>>> %c\n", deads[index].package);
                do
                {

                    printf("\n\n\t\t\t\t    =============================================================\n\n");
                    printf("\n\t\t\t\t\t PACKAGE       >>>>>>>>>>>>>>> ");
                    scanf(" %c", &packg);
                    if (packg < 'A' || packg > 'D')
                    {
                        printf("\n\t\t\t>>>>>>>>>>>>>>> Invalid Package. Please enter a value between A and D <<<<<<<<<<<<<<<<\n");
                    }
                    else
                        deads[index].package = packg;

                } while (packg < 'A' || packg > 'D');
                break;
            case 6:
                system("cls");
                sleep(0.5);
                printf("\n\n\n\n\t\t\t\t\t\t Removing Data.....\n\n\n");
                sleep(2);
                deads[index] = empty;
                strcpy(lotdet[index].status, "Free");
                break;
            default:
                system("cls");
                sleep(0.5);
                printf("\n\n\n\n\t\t\t\t\t\t Invalid Input. Try Again\n\n");
                sleep(2.5);
                break;
            }

        } while (option != 1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 6);

        cMonth(deads[index].BirthMonth, b2mont);
        cMonth(deads[index].DeathMonth, d2mont);
        do
        {
            // print updated info
            system("cls");
            sleep(1);
            printf("\n\n\n\t\t\t\t\t\t Please confirm the details below:\n\n");
            printf("\n\t\t\t\t                      UPDATED LOT INFORMATION");
            printf("\n\t\t\t\t    =============================================================\n\n");
            printf("\t\t\t\t\t LOT ID        >>>>>>>>>>>>>>> %c-%d\n", colid, deads[index].row);
            printf("\t\t\t\t\t NAME          >>>>>>>>>>>>>>> %s\n", deads[index].fullname);
            printf("\t\t\t\t\t DATE OF BIRTH >>>>>>>>>>>>>>> %s / %02d /%d\n", bmont, deads[index].BirthDay, deads[index].BirthYear);
            printf("\t\t\t\t\t DATE OF DEATH >>>>>>>>>>>>>>> %s / %02d /%d\n", dmont, deads[index].DeathDay, deads[index].DeathYear);
            printf("\t\t\t\t\t PACKAGE       >>>>>>>>>>>>>>> %c\n", deads[index].package);
            printf("\t\t\t\t\t QOUTE         >>>>>>>>>>>>>>> %s\n", deads[index].qoute);
            printf("\n\t\t\t\t    =============================================================\n");

            // Ask confirmation
            printf("\n\t\t\t\t\t\t Choose one of the Options below:\n\n");
            printf("\t\t\t\t\t\t    1 => Confirm\n");
            printf("\t\t\t\t\t\t    2 => Again\n");
            printf("\t\t\t\t\t\t    3 => Cancel\n");
            printf("\n\t\t\t\t\t\t  Option: ");
            scanf("%d", &check);
            switch (check)
            {
            case 1: // Confirm ---> Save info
                Save(deads, lotdet, ifp, lfp);
                return;
                break;
            case 2: // Update something Again
                break;
            case 3: // If cancel set the info back into its original datas
                deads[index] = ifcancel;
                return;
            default:
                printf("\n\t\t\t\t\t\t Invalid Input. Try Again\n\n");
                break;
            }
            if (check == 2)
                break;

        } while (check != 1);
    } while (1);
}
