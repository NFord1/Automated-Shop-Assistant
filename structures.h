#fndef 

#define BUFFER 64

typedef struct{
    int itemCode;
    char itemName[BUFFER];
    int previousStock;
    int currentStock;
    double productWidth;
    double price;
} Product;
