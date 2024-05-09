#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_ENT 100
#define Max_col 10
#define Max_row 10
#define MAX_STR 100

typedef struct
{
    int column;
    int row;
    float size1;
    float size2;
    int Price;
    char status[MAX_STR];
} Lot_Info;

typedef struct
{
    int column;
    int row;
    char fullname[MAX_STR];
    int BirthMonth;
    int BirthDay;
    int BirthYear;
    int DeathMonth;
    int DeathDay;
    int DeathYear;
    char package;
    char qoute[MAX_STR];
} Info;

Info Read_Entry(FILE *fileptr);
Lot_Info Lot_details(FILE *lotptr);
void MarkLot(char Lot[][Max_col], Info entry);
void PLot(char Lot[][Max_col]);
void Print_info(int col, int row, Info deads[MAX_ENT], Lot_Info lotdet[MAX_ENT]);
void cMonth(int num_month, char *monthptr);
Info Register(Info entry_again, int curr_year, int column, int row);
char Choose_pckg();
float getPackagePrice(char package);
void Save(Info holder[MAX_ENT], Lot_Info lot[MAX_ENT], FILE *tp, FILE *lp);
void print_text();
void pkg_des();

int main()
{
    int Menu_Option;
    int LvisMenu; // Variable for Menu
    int Pckg_menu;
    int Pay_menu;
    int confirm_menu;
    int index = 0; // Index in the array from row and column
    int Lot_index = 0;
    int again;             // Variable for the loop in Choosing Lot Info
    int row_id, column_id; // Column and Row
    float packagePrice;
    int curr_year;

    Info deads[MAX_ENT]; // Arrays of Struct, 100
    Lot_Info lotdet[MAX_ENT];
    Info entry;         // For the masterlits file with taken lot
    Info entry_again;   // For the input entry
    Lot_Info lot_entry; // For scanning the lot details
    static const Info empty;

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

    ifp = fopen("MASTERLIST.in", "r+");
    lfp = fopen("Lotfile.in", "r+");

    if (feof(ifp) && ferror(ifp) || feof(lfp) || ferror(lfp))
    {
        printf("Error opening files.\n");
        return 1;
    }

    while (1)
    {

        entry = Read_Entry(ifp);
        lot_entry = Lot_details(lfp);

        if (feof(ifp) && ferror(ifp) || feof(lfp) || ferror(lfp))
            break;

        if (entry.row >= 1 && entry.row <= Max_row && entry.column >= 0 && entry.column <= Max_col)
        {
            index = ((entry.row - 1) * Max_row) + entry.column;
            deads[index] = entry;
            MarkLot(Lot, entry);
        }

        if (lot_entry.row >= 1 && lot_entry.row <= Max_row && lot_entry.column >= 0 && lot_entry.column <= Max_col)
        {
            Lot_index = ((lot_entry.row - 1) * Max_row) + lot_entry.column;
            lotdet[Lot_index] = lot_entry;
        }
    }
    while (1)
    {
        system("cls");
        print_text();
        sleep(0.5);
        printf("\t\t\t\t\t\tBurial Lot Information System\n\n");
        printf("\t\t\t\t\t\tChoose one of the Options below:\n\n");
        printf("\t\t\t\t\t\t    1 => New Entry\n");
        printf("\t\t\t\t\t\t    2 => Lot Visuals\n");
        printf("\t\t\t\t\t\t    3 => Package Descriptions\n");
        printf("\t\t\t\t\t\t    4 => Exit\n\n");
        printf("\t\t\t\t\t\t  Option: ");
        scanf("%d", &Menu_Option);
        switch (Menu_Option)
        {
        case 1:
            system("cls");
            sleep(1);
            do
            {
                system("cls");
                sleep(0.5);
                printf("\n\t\t\t\t    ======================>  NEW ENTRY <=========================\n");
                // Display the Lot visuals
                PLot(Lot);
                printf("\n\t\t\t\t    Enter Lot ID (Column-Row): ");
                scanf(" %c-%d", &colid, &row_id);
                column_id = toupper(colid) - 'A';

                index = ((row_id - 1) * Max_row) + column_id;

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
                            case 1:
                                MarkLot(Lot, deads[index]);
                                strcpy(lotdet[index].status, "taken");
                                PLot(Lot);
                                printf("\t\t\t\t\t\tYou have succesfully registered...");
                                sleep(1);
                                break;
                            case 2:
                                deads[index] = empty;
                                break;
                            default:
                                printf("\t\t\t\t\t\t Not an option, Try Again\n");
                                sleep(1);
                                printf("\n");
                                break;
                            }
                            break;
                        case 2:
                            deads[index] = empty;
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
                PLot(Lot);
                printf("\n\t\t\t\t    =====================> LOT VISUALS <=========================\n");
                printf("\n\t\t\t\t\t\t Choose one of the Options below:\n\n");
                printf("\t\t\t\t\t\t    1 => Lot Information\n");
                printf("\t\t\t\t\t\t    2 => Main Menu\n");
                printf("\t\t\t\t\t\t    3 => Exit\n\n");
                printf("\t\t\t\t\t\t  Option: ");
                scanf("%d", &LvisMenu);

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

                        column_id = toupper(colid) - 'A';

                        if (column_id < 0 || column_id > ('J' - 'A') || row_id < 0 || row_id > Max_row)
                        {

                            printf("Not an option. Try Again\n");
                        }
                        else
                        {

                            Print_info(column_id, row_id, deads, lotdet);

                            printf("\n\t\t\t\t\t\t Choose one of the Options below:\n\n");
                            printf("\t\t\t\t\t\t    1 => Choose Again\n");
                            printf("\t\t\t\t\t\t    2 => Main Menu\n");
                            printf("\t\t\t\t\t\t    3 => Exit\n\n");
                            printf("\t\t\t\t\t\t  Option: ");

                            scanf("%d", &again);

                            switch (again)
                            {
                            case 1:
                                sleep(1);
                                break;
                            case 2:
                                break;
                            case 3:
                                printf("Exiting Program...\n");
                                Save(deads, lotdet, ifp, lfp);
                                fclose(ifp);
                                fclose(lfp);
                                exit(0);
                            default:
                                printf("Not an option, Try Again\n");
                                printf("\n");
                                break;
                            }
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
                case 1:
                    system("cls");
                    sleep(1);
                    PLot(Lot);
                    printf("\n\t\t\t\t    Enter Lot ID (Column-Row): ");
                    scanf(" %c-%d", &colid, &row_id);
                    column_id = toupper(colid) - 'A';

                    index = ((row_id - 1) * Max_row) + column_id;

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
                                case 1:
                                    MarkLot(Lot, deads[index]);
                                    strcpy(lotdet[index].status, "taken");
                                    PLot(Lot);
                                    printf("\t\t\t\t\t\tYou have succesfully registered...");
                                    sleep(1);
                                    break;
                                case 2:
                                    deads[index] = empty;
                                    break;
                                default:
                                    printf("\t\t\t\t\t\t Not an option, Try Again\n");
                                    sleep(1);
                                    printf("\n");
                                    break;
                                }
                                break;
                            case 2:
                                deads[index] = empty;
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

                    break;
                case 2:
                    system("cls");
                    sleep(2);
                    break;
                case 3:
                    Save(deads, lotdet, ifp, lfp);
                    fclose(ifp);
                    fclose(lfp);
                    exit(0);
                default:
                    printf("Not an option, Try Again\n");
                    sleep(1);
                    break;
                }
            } while (1);
        case 4: // Exit Program
            printf("Exiting Program...\n");
            Save(deads, lotdet, ifp, lfp);
            fclose(ifp);
            fclose(lfp);
            exit(0);
        default: // Loop if Not an Option
            printf("Not an option, Try Again\n");
            sleep(1);
            break;
        }
    }

    Save(deads, lotdet, ifp, lfp);
    fclose(ifp);
    fclose(lfp);

    return 0;
}

Info Read_Entry(FILE *fileptr)
{
    Info entry;
    char coletter;
    // Read lot coordinates
    fscanf(fileptr, " %c-%d", &coletter, &entry.row);
    entry.column = toupper(coletter) - 'A';
    fscanf(fileptr, " %[^\n]", entry.fullname);
    fscanf(fileptr, "%d", &entry.BirthMonth);
    fscanf(fileptr, "%d", &entry.BirthDay);
    fscanf(fileptr, "%d", &entry.BirthYear);
    fscanf(fileptr, "%d", &entry.DeathMonth);
    fscanf(fileptr, "%d", &entry.DeathDay);
    fscanf(fileptr, "%d", &entry.DeathYear);
    fscanf(fileptr, " %c", &entry.package);
    fscanf(fileptr, " %[^\n]", entry.qoute);

    return entry;
}

Lot_Info Lot_details(FILE *lotptr)
{
    Lot_Info loentr;
    char lettercol;

    fscanf(lotptr, " %c-%d", &lettercol, &loentr.row);
    loentr.column = toupper(lettercol) - 'A';
    fscanf(lotptr, "%f", &loentr.size1);
    fscanf(lotptr, "%f", &loentr.size2);
    fscanf(lotptr, "%d", &loentr.Price);
    fscanf(lotptr, " %[^\n]", loentr.status);

    return loentr;
}

void MarkLot(char Lot[][Max_col], Info entry)
{
    Lot[entry.row - 1][entry.column] = 'X'; // Mark the lot based on 1-based indexing
}

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

void cMonth(int num_month, char *monthptr)
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

    colleter = 'A' + col;

    index = ((row - 1) * Max_row) + col;
    system("cls");
    sleep(1);

    cMonth(deads[index].BirthMonth, bmonth);
    cMonth(deads[index].DeathMonth, dmonth);

    if (strcmp(lotdet[index].status, "taken") == 0)
    {
        strcpy(stats, "Taken");
    }
    else
    {
        strcpy(stats, "Free");
    }

    printf("\n\n\n\n\n\n\n");

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
    entry_again.column = column;
    entry_again.row = row;
    system("cls");
    entry_again.package = Choose_pckg();
    sleep(1);
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

        do
        {
            printf("\n\t\t\t\t\t BIRTH DAY     >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.BirthDay);

            if (entry_again.BirthDay < 1 || entry_again.BirthDay > 31)
            {
                printf("\n\t\t\t>>>>>>>>>>>>>>> Invalid Day. Please enter a value between 1 and 31 <<<<<<<<<<<<<<<<\n");
            }

        } while (entry_again.BirthDay < 1 || entry_again.BirthDay > 31);

        do
        {
            printf("\n\t\t\t\t\t BIRTH YEAR    >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.BirthYear);

            if (entry_again.BirthYear < 1800 || entry_again.BirthYear > curr_year)
            {
                printf("\n\t\t\t>>>>>>>>>Invalid Year. Please enter a value between 1800 and Current Year<<<<<<<<<\n");
            }

        } while (entry_again.BirthYear < 1800 || entry_again.BirthYear > curr_year);

        do
        {
            printf("\n\t\t\t\t\t DEATH MONTH   >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.DeathMonth);

            if (entry_again.DeathMonth < 1 || entry_again.DeathMonth > 12)
            {
                printf("\n\t\t\t>>>>>>>>>>>>>>>Invalid month. Please enter a value between 1 and 12<<<<<<<<<<<<<<<<\n");
            }
        } while (entry_again.DeathMonth < 1 || entry_again.DeathMonth > 12);

        do
        {
            printf("\n\t\t\t\t\t DEATH DAY     >>>>>>>>>>>>>>> ");
            scanf("%d", &entry_again.DeathDay);

            if (entry_again.DeathDay < 1 || entry_again.DeathDay > 31)
            {
                printf("\n\t\t\t>>>>>>>>>>>>>>> Invalid Day. Please enter a value between 1 and 31 <<<<<<<<<<<<<<<<\n");
            }

        } while (entry_again.DeathDay < 1 || entry_again.DeathDay > 31);

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

        } while (check != 1);

    } while (check == 2);
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

void Save(Info holder[MAX_ENT], Lot_Info lot[MAX_ENT], FILE *tp, FILE *lp)
{
    if (tp == NULL || lp == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }
    else
    {
        fseek(tp, 0, SEEK_SET);
        for (int i = 0; i < MAX_ENT; i++)
        {
            if (strcmp(holder[i].fullname, "") != 0)
            {
                char colmn = 'A' + holder[i].column;
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

        fseek(lp, 0, SEEK_SET);
        for (int j = 0; j < MAX_ENT; j++)
        {
            char lcol = 'A' + lot[j].column;
            fprintf(lp, "%c-%d\n", lcol, lot[j].row);
            fprintf(lp, "%.2f\n", lot[j].size1);
            fprintf(lp, "%.2f\n", lot[j].size2);
            fprintf(lp, "%d\n", lot[j].Price);
            fprintf(lp, "%s\n\n", lot[j].status);
        }
    }
}

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
