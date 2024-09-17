#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


#define GRID_WIDTH 50
#define GRID_HEIGHT 17


enum SpotType {
    EMPTY, KEY, OBSTACLE, DOOR1, DOOR2, INTERACTABLE1, INTERACTABLE2, INTERACTABLE3, PAPER_TRASH, METAL_TRASH, PLASTIC_TRASH, PLASTIC_BIN, METAL_BIN, PAPER_BIN
};

enum SpotType **grid;


typedef struct {
    int x;
    int y;
    bool hasKey;
    int hasItem;
} Player;

int currentRoom = 1;
int count = 0;

void allocate_grid(int width, int height) {
    grid = malloc(width * sizeof(enum SpotType *));
    if (grid == NULL) {
        fprintf(stderr, "Failed to allocate memory for grid rows.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < width; i++) {
        grid[i] = malloc(height * sizeof(enum SpotType));
        if (grid[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for grid row %d.\n", i);
            // Free already allocated rows before exiting
            for (int j = 0; j < i; j++) {
                free(grid[j]);
            }
            free(grid);
            exit(EXIT_FAILURE);
        }
    }
}

void free_grid(int height) {
    for (int i = 0; i < height; i++) {
        free(grid[i]);
    }
    free(grid);
}

void initialize_player(Player* player, int x, int y) {
    player->x = x;
    player->y = y;
    player->hasKey = false;
    player->hasItem = 0;
}

void initialize_grid1() {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            grid[j][i] = EMPTY;
            if (i == 0 || j == 0 || i == GRID_HEIGHT - 1 || j == GRID_WIDTH - 1 || (j == 10 && i > 7) || (j == 25 && i < 9) || (j == 40 && i > 7) || (i == 8)) {
                grid[j][i] = OBSTACLE;
            }
        }
    }
    for(int i = 10; i < GRID_HEIGHT; i++){
        grid[10][i] = OBSTACLE;
    }
    for(int i = 1; i < 9; i++){
        grid[25][i] = OBSTACLE;
    }
    for(int i = 10; i < GRID_HEIGHT; i++){
        grid[40][i] = OBSTACLE;
    }

    grid[49][4] = DOOR1;
    grid[49][12] = DOOR2;
    grid[10][4] = INTERACTABLE1;
    grid[25][12] = INTERACTABLE2;
    grid[40][4] = INTERACTABLE3;
    grid[44][12] = KEY;
}
void initialize_grid2() {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            grid[j][i] = EMPTY;
            if (i == 0 || j == 0 || i == GRID_HEIGHT - 1 || j == GRID_WIDTH - 1 || (i == 8)) {
                grid[j][i] = OBSTACLE;
            }
        }
    }

    grid[49][4] = DOOR1;
    grid[49][12] = DOOR2;
    grid[24][9] = METAL_BIN;
    grid[15][4] = PAPER_BIN;
    grid[33][4] = PLASTIC_BIN;
    grid[24][13] = PAPER_TRASH;
    grid[15][13] = METAL_TRASH;
    grid[33][13] = PLASTIC_TRASH;

}

void print_grid(Player player1, Player player2) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            char printChar = ' ';
            if (grid[j][i] == OBSTACLE) printChar = '#';
            else if (grid[j][i] == PAPER_TRASH) printChar = '*';
            else if (grid[j][i] == PLASTIC_TRASH) printChar = '&';
            else if (grid[j][i] == METAL_TRASH) printChar = '~';
            else if (grid[j][i] == METAL_BIN) printChar = '%';
            else if (grid[j][i] == PLASTIC_BIN) printChar = '@';
            else if (grid[j][i] == PAPER_BIN) printChar = '$';
            else if (grid[j][i] == KEY) printChar = 'K';
            else if (grid[j][i] == INTERACTABLE1 || grid[j][i] == INTERACTABLE2 || grid[j][i] == INTERACTABLE3) printChar = '+';
            else if (grid[j][i] == DOOR1 || grid[j][i] == DOOR2) printChar = 'D';
            if (i == player1.y && j == player1.x) printChar = 'X';
            if (i == player2.y && j == player2.x) printChar = 'Y';
            printf("%c ", printChar);
        }
        printf("\n");
    }
    

}


void move_player(Player* player1, Player* player2, char direction) {
    int newX = player1->x, newY = player1->y;
    int newX2 = player2->x, newY2= player2->y;
    switch (direction) {
        case 'w': case 'W': newY--; break;
        case 's': case 'S': newY++; break;
        case 'a': case 'A': newX--; break;
        case 'd': case 'D': newX++; break;
        case 'i': case 'I': newY2--; break;
        case 'k': case 'K': newY2++; break;
        case 'j': case 'J': newX2--; break;
        case 'l': case 'L': newX2++; break;

    }
    if (newX >= 0 && newX < GRID_WIDTH && newY >= 0 && newY < GRID_HEIGHT && grid[newX][newY] != OBSTACLE) {
        player1->x = newX;
        player1->y = newY;
    }
    if (newX2 >= 0 && newX2 < GRID_WIDTH && newY2 >= 0 && newY2 < GRID_HEIGHT && grid[newX2][newY2] != OBSTACLE) {
        player2->x = newX2;
        player2->y = newY2;
    }
}

void interact_with_spot(Player* player1, Player* player2) {
    int spotType = grid[player1->x][player1->y];
    int spotType2 = grid[player2->x][player2->y];
    if (spotType == KEY) {
        player1->hasKey = true;
            printf("Player 1 picked up a key from (%d, %d)\n", player1->x, player1->y);
            grid[player1->x][player1->y] = EMPTY;
    }
    if (spotType2 == KEY) {
        player2->hasKey = true;
            printf("Player 2 picked up a key from (%d, %d)\n", player2->x, player2->y);
            grid[player2->x][player2->y] = EMPTY;
    }
    char choice;
    if (spotType == INTERACTABLE1 && currentRoom == 1) { 
        do{
            printf("Answer this question to open your partner's barrier.\n");
            printf("How many children and youth will be out of school by 2030 if we don't provide any additional attention?\n");
            printf("A. 10 Million\n");
            printf("B. 45 Million\n");
            printf("C. 85 Million\n");
            scanf(" %c", &choice);
            if (choice != 'C'){
                printf("Incorrect. Try Again!\n");
            }
        }while(choice != 'C');
        
        printf("Correct! Furthermore, 300 million children will not have basic numeracy/literary skills. We need to do something about that!\n");
        
        grid[player1->x][player1->y] = EMPTY;
        for(int i = 9; i < GRID_HEIGHT-1; i++){
            grid[10][i] = EMPTY;
        }
    }
    if (spotType2 == INTERACTABLE2 && currentRoom == 1) { 
        do{
            printf("Answer this question to open your partner's barrier.\n");
            printf("How many people still live in the dark?(no electric lights)\n");
            printf("A. 30 Million\n");
            printf("B. 675 Million\n");
            printf("C. 325 Million\n");
            scanf(" %c", &choice);
            if (choice != 'B'){
                printf("Incorrect. Try Again!\n");
            }
        }while(choice != 'B');
        printf("Correct! 4 in 5 of those 675 million people live in Sub-Saharan Africa.\n");
        
        grid[player2->x][player2->y] = EMPTY;
        for(int i = 1; i < 8; i++){
            grid[25][i] = EMPTY;
        }
    }
    if (spotType == INTERACTABLE3 && currentRoom == 1) { 
        do{
            printf("Answer this question to open your partner's barrier.\n");
            printf("How many people are projected to face hunger in 2030?\n");
            printf("A. 600 Million\n");
            printf("B. 400 Million\n");
            printf("C. 250 Million\n");
            scanf(" %c", &choice);
            if (choice != 'A'){
                printf("Incorrect. Try Again!\n");
            }
        }while(choice != 'A');
        printf("Correct! Malnutrition like this impeads a child's development and well-being.\n");
        
        grid[player1->x][player1->y] = EMPTY;
        for(int i = 9; i < GRID_HEIGHT; i++){
            grid[40][i] = EMPTY;
        }
    }
    if (spotType == METAL_BIN) { 
        printf("You moved the Metal Bin to the other side.\n");

        grid[24][9] = METAL_BIN;
        grid[15][4] = PLASTIC_BIN;
        grid[33][4] = PAPER_BIN;
        player1->y = player1->y - 1;
    }
    if (spotType == PLASTIC_BIN) { 
        printf("You moved the Plastic Bin to the other side.\n");

        grid[24][9] = PLASTIC_BIN;
        grid[15][4] = METAL_BIN;
        grid[33][4] = PAPER_BIN;
        player1->y = player1->y - 1;
    }
    if (spotType == PAPER_BIN) { 
        printf("You moved the Paper Bin to the other side.\n");

        grid[24][9] = PAPER_BIN;
        grid[15][4] = PLASTIC_BIN;
        grid[33][4] = METAL_BIN;
        player1->y = player1->y - 1;
    }
    if (spotType2 == METAL_TRASH) { 
        if(player2->hasItem == 2 || player2->hasItem == 3){
            printf("You already have an item in your hand.\n");
            player2->y = player2->y - 1;

        }
        else{
            printf("You picked up the metal trash.\n");

            player2->hasItem = 1;
            grid[player2->x][player2->y] = EMPTY;

            player2->y = player2->y - 1;
        }
    }
    if (spotType2 == PAPER_TRASH) { 
        if(player2->hasItem == 1 || player2->hasItem == 3){
            printf("You already have an item in your hand.\n");
            player2->y = player2->y - 1;

        }
        else{
            printf("You picked up the paper trash.\n");

            player2->hasItem = 2;
            grid[player2->x][player2->y] = EMPTY;

            player2->y = player2->y - 1;
        }
    }
    if (spotType2 == PLASTIC_TRASH) { 
        if(player2->hasItem == 1 || player2->hasItem == 2){
            printf("You already have an item in your hand.\n");
            player2->y = player2->y - 1;

        }
        else{
            printf("You picked up the plastic trash.\n");

            player2->hasItem = 3;
            grid[player2->x][player2->y] = EMPTY;

            player2->y = player2->y - 1;
        }
    }
    
    if (spotType2 == METAL_BIN) { 
        if(player2->hasItem == 1){
            printf("You placed the metal trash into the correct bin!\n");
            ++count;

            if (count == 3){
                printf("A key has appeared!\n");
                grid[46][4] = KEY;
            }
            player2->hasItem = 0;
            player2->y = player2->y + 1;
        }
        else if(player2->hasItem == 0){
            printf("You are not holding an item.\n");
            player2->y = player2->y + 1;
        }
        else{
            printf("This is the wrong bin. Player 1 needs to choose the right bin.\n");
            player2->y = player2->y + 1;
        }
    }
    if (spotType2 == PAPER_BIN) { 
        if(player2->hasItem == 2){
            printf("You placed the paper trash into the correct bin!\n");
            ++count;
            if (count == 3){
                printf("A key has appeared!\n");
                grid[46][4] = KEY;
            }
            player2->hasItem = 0;
            player2->y = player2->y + 1;
        }
        else if(player2->hasItem == 0){
            printf("You are not holding an item.\n");
            player2->y = player2->y + 1;
        }
        else{
            printf("This is the wrong bin. Player 1 needs to choose the right bin.\n");
            player2->y = player2->y + 1;
        }
    }
    if (spotType2 == PLASTIC_BIN) { 
        if(player2->hasItem == 3){
            printf("You placed the plastic trash into the correct bin!\n");
            
            player2->hasItem = 0;
            player2->y = player2->y + 1;
            ++count;

            if (count == 3){
                printf("A key has appeared!\n");
                grid[46][4] = KEY;
            }
        }
        else if(player2->hasItem == 0){
            printf("You are not holding an item.\n");
            player2->y = player2->y + 1;
        }
        else{
            printf("This is the wrong bin. Player 1 needs to choose the right bin.\n");
            player2->y = player2->y + 1;
        }
    }
}

void save_progress(Player *player1, Player *player2, const char *filename) {
    char fullname[60];
    sprintf(fullname, "%s.txt", filename);
    FILE *file = fopen(fullname, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "%d %d %d %d\n", player1->x, player1->y, player1->hasKey, player1->hasItem);
    fprintf(file, "%d %d %d %d\n", player2->x, player2->y, player2->hasKey, player2->hasItem);

    fprintf(file, "%d %d\n", currentRoom, count);
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            fprintf(file, "%d ", grid[j][i]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int load_progress(Player *player1, Player *player2, const char *filename) {
    char fullname[60];
    sprintf(fullname, "%s.txt", filename);
    FILE *file = fopen(fullname, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0; 
    }
    int tempHasKey1, tempHasKey2; 
    if (fscanf(file, "%d %d %d %d", &player1->x, &player1->y, &tempHasKey1, &player1->hasItem) != 4 ||
        fscanf(file, "%d %d %d %d", &player2->x, &player2->y, &tempHasKey2, &player2->hasItem) != 4) {
        fprintf(stderr, "Error reading player data from file.\n");
        fclose(file);
        return 0;
    }
    player1->hasKey = tempHasKey1;
    player2->hasKey = tempHasKey2;

    if (fscanf(file, "%d %d", &currentRoom, &count) != 2) {
        fprintf(stderr, "Error reading room and count data from file.\n");
        fclose(file);
        return 0;
    }

    int gridValue;
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (fscanf(file, "%d", &gridValue) != 1) {
                fprintf(stderr, "Error reading grid data from file at position (%d, %d).\n", j, i);
                fclose(file);
                return 0;
            }
            grid[j][i] = (enum SpotType)gridValue; 
        }
    }
    fclose(file);
    return 1;
}

void gameLoop(Player player1, Player player2){
    char input;

    if (currentRoom != 2){
        while (true) {
            print_grid(player1, player2);
            printf("Enter a direction (w, a, s, d for Player 1 or i, j, k, l for Player 2) or 'Q' to save and exit: ");
            scanf(" %c", &input);

            if (input == 'Q' || input == 'q') {
                char filename[50];
                printf("Enter name to save the game: ");
                scanf("%s", filename);
                save_progress(&player1, &player2, filename);
                printf("Game saved. Exiting...\n");
                exit(0);
            }
            
            move_player(&player1, &player2, input);
            interact_with_spot(&player1, &player2);
            int spotType = grid[player1.x][player1.y];
            int spotType2 = grid[player2.x][player2.y];
            if (spotType == DOOR1 || spotType2 == DOOR2) break;
        }
    }
    if (currentRoom != 2){
        printf("In this room, Player 2 will pick up trash and put them in the right bin.\n Player 1 will rotate the bins around so Player 2 can put it in the right bin.\n");
        printf("To rotate the bins, stand on the bin that you want on Player 2's side.\n To put the trash in the bin, stand on the bin.\n");
        printf("Learning to reduce, reuse, and recycle is important in order to lower environmental waste and protect our environment.");
        player1.x = 1;
        player1.y = 4;
        player2.x = 1;
        player2.y = 12;
        player2.hasKey = false;
        initialize_grid2();
    }
    
    
    while (true){
        currentRoom = 2;
        print_grid(player1, player2);
        printf("%%: Metal Bin, @: Plastic Bin, $: Paper Bin\n");
        
        printf("Enter a direction (w, a, s, d for Player 1; i, j, k, l for Player 2) or Q to save and exit: ");
        scanf(" %c", &input);

        if (input == 'Q' || input == 'q') {
            char filename[50];
            printf("Enter name to save the game: ");
            scanf("%s", filename);
            save_progress(&player1, &player2, filename);
            printf("Game saved. Exiting...\n");
            break;
        }

        move_player(&player1, &player2, input);
        interact_with_spot(&player1, &player2);
        int spotType = grid[player1.x][player1.y];
        int spotType2 = grid[player2.x][player2.y];
        if ((spotType == DOOR1 || spotType2 == DOOR2) && player1.hasKey == true) {
            printf("You completed the game! Congratulations!\n");
            exit(0);
        }
    
    }
        
    
}

int main() {
    allocate_grid(GRID_WIDTH, GRID_HEIGHT);
    char choice;

    printf("Welcome to the educational game!\n\n");
    printf("Do you want to load saved progress or start a new game?\n");
    printf("1. Load Saved Game\n");
    printf("2. Start New Game\n");
    printf("Enter your choice (1 or 2): ");
    scanf(" %c", &choice);

    Player player1;
    Player player2;
    if (choice == '1') {
        char filename[100];
        printf("Enter the filename to load: ");
        scanf("%s", filename);
        if (!load_progress(&player1, &player2, filename)) {
            printf("Failed to load the file. Starting a new game.\n");
            initialize_grid1(); 
            initialize_player(&player1, 1, 4);
            initialize_player(&player2, 1, 12);
        } else {
            printf("Game loaded successfully!\n");
        }
    } else if (choice == '2') {
        initialize_grid1(); // Initialize the grid for new game
        initialize_player(&player1, 1, 4);
        initialize_player(&player2, 1, 12);
    } else {
        printf("Invalid choice. Exiting program.\n");
        free_grid(GRID_HEIGHT);
        return 0;
    }

    // Start the game loop
    gameLoop(player1, player2);
    free_grid(GRID_HEIGHT);
    return 0;
}
