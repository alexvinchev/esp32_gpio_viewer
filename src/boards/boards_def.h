#define boards_definitions

typedef struct BoardGPIO
{
    int gpio;
    float topPosition;
    float leftPosition;
};

typedef struct BoardDetail
{
    BoardGPIO *gpios;
    int numberOfGpios;
    String board_image;
    String board_name;
};

class ESPBoard
{
private:
    BoardDetail boardDef;

public:
    ESPBoard(BoardGPIO *gpioArray, int numGpios,String image,String name) {
        boardDef.gpios = gpioArray;
        boardDef.numberOfGpios = numGpios;
        boardDef.board_image = image;
        boardDef.board_name = name;
    }
    String getImage(void) {
        return(boardDef.board_image);
    }

    BoardGPIO* getGPIOs(void) {
        return(boardDef.gpios);
    }

    String getBoardModelName(void) {
        return boardDef.board_name;
    }

    int getGPIOsCount(void) {
        return boardDef.numberOfGpios;
    }
    int* getGPIOsPins(void) {
        int *pins = new int[boardDef.numberOfGpios];
        for (int i = 0; i < boardDef.numberOfGpios; i++)
        {
            pins[i] = boardDef.gpios[i].gpio;
        }
        return pins;
    }
};