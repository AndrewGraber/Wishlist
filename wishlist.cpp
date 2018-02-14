#include <stdio.h>
#include <stdlib.h>

#define DESIRE_VALUE 1
#define USE_VALUE 1.25
#define URGENT_VALUE 1.5

#define ITEM_VALUE(desire,use,urgent) ((DESIRE_VALUE * desire) + (USE_VALUE * use) + (URGENT_VALUE * urgent))

//Represents an item in the wishlist
struct ListItem {
    char *name;
    float desire;
    float use;
    float urgent;
};

//Creates an item. Sets floats to -1, because 0 will be a valid option.
struct ListItem * createItem() {
    struct ListItem * item = (struct ListItem *)malloc(sizeof(struct ListItem));
    item->name = NULL;
    item->desire = -1;
    item->use = -1;
    item->urgent = -1;
    return item;
}

//Frees memory used by an item and sets it to null
void removeItem(struct ListItem * item) {
    if(item->name != NULL) free(item);
    free(item);
    item = NULL;
}

//Compares two items by determining their overall value
int compareItems(struct ListItem * item1, struct ListItem * item2) {
    return ITEM_VALUE(item1->desire, item1->use, item1->urgent) - ITEM_VALUE(item2->desire, item2->use, item2->urgent);
}

//Counts the number of lines in the file
int num_lines(FILE *fd) {
    char curr;
    int lines = 0;

    while(!feof(fd)) {
      curr = fgetc(fd);
      if(curr == '\n') lines++;
    }

    //Return pointer to beginning of file
    fseek(fd, 0, SEEK_SET);

    return lines;
}

//Helper print function
void invalidFormat() {
    printf("Error: The file is not properly formatted!\n");
    exit(-1);
}

//Tries to get a name from the file
char * getName(FILE *fd) {
    if(feof(fd)) {
        printf("Unexpected EOF!\n");
        invalidFormat();
    }
    char *name = (char *)malloc(21*sizeof(char));
    char curr;
    int n_chars = 0;
    while((curr = fgetc(fd)) != ':') {
        n_chars++;
        if(n_chars > 20) {
            printf("Name exceeds max of 20 characters!\n");
            invalidFormat();
        }
        name[n_chars-1] = curr;
        if(feof(fd)) {
            printf("Unexpected EOF!\n");
            invalidFormat();
        }
    }
    name[n_chars] = '\0'; //Null terminate it
    return name;
}

//Literally the worst sort ever. Will change this in the future
void sortItems(struct ListItem ** items, int n_items) {
    bool changeMade;
    do {
        changeMade = false;
        for(int i=0; i<n_items-1; i++) {
            if(compareItems(items[i], items[i+1]) < 0) {
                ListItem * temp = items[i];
                items[i] = items[i+1];
                items[i+1] = temp;
                changeMade = true;
            }
        }
    } while(!changeMade);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: wishlist <file-in> <file-out>\n");
        return -1;
    }

    FILE *file_in = fopen(argv[1], "r");
    FILE *file_out = fopen(argv[2], "w");
    if(file_in == NULL) { //Error opening file - probably does not exist
        printf("Cannot find file: %s\n", argv[0]);
        return -1;
    }

    int n_items = num_lines(file_in);

    struct ListItem **items = (struct ListItem **)malloc(n_items*sizeof(struct ListItem *));

    //Populate items
    for(int i=0; i<n_items; i++) {
        items[i] = createItem(); //Allocates memory
        items[i]->name = getName(file_in);
        if(fscanf(file_in, "%f %f %f\n", &items[i]->desire, &items[i]->use, &items[i]->urgent) != 3) {
            printf("Malformed List Item!\n");
            invalidFormat();
        }
    }

    sortItems(items, n_items);

    fprintf(file_out, "Wishlist Priority Order:\n");

    //Write list out to file
    for(int i=0; i<n_items; i++) {
        fprintf(file_out, "%d: %s (Value: %.2f)\n", i+1, items[i]->name, ITEM_VALUE(items[i]->desire, items[i]->use, items[i]->urgent));
    }

    //Free all the items for good practice
    for(int i=0; i<n_items; i++) {
        removeItem(items[i]);
    }

    //Close files and exit program
    fflush(file_out);
    fclose(file_in);
    fclose(file_out);
    return 0;
}
