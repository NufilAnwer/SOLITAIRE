
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
using namespace std;

class Card {
public:
    static const int Hearts = 0;
    static const int Diamonds = 1;
    static const int Clubs = 2;
    static const int Spades = 3;

    static const int Ace = 1;
    static const int Two = 2;
    static const int Three = 3;
    static const int Four = 4;
    static const int Five = 5;
    static const int Six = 6;
    static const int Seven = 7;
    static const int Eight = 8;
    static const int Nine = 9;
    static const int Ten = 10;
    static const int Jack = 11;
    static const int Queen = 12;
    static const int King = 13;

    Card(int s, int r) : suit(s), rank(r), faceUp(false) {}

    int getSuit() const { return suit; }
    int getRank() const { return rank; }
    bool isFaceUp() const { return faceUp; }
    void flip() { faceUp = true; }
    int getColor() const {
        return (suit == Hearts || suit == Diamonds) ? 1 : 0; // 1 for red, 0 for black
    }
    string toString() const {
        const char* suits[] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        const char* ranks[] = { "", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
        return ranks[rank] + string((suit == Hearts || suit == Diamonds) ? " (red)" : " (black)") + " " + suits[suit];
    }

    string abbreviated() const {
        const char* suits[] = { "H", "D", "C", "S" }; // Hearts, Diamonds, Clubs, Spades
        const char* ranks[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
        return ranks[rank] + string(suits[suit]);
    }

private:
    int suit;
    int rank;
    bool faceUp;
};

class Linkedlist {
public:
    Card* card;
    Linkedlist* next;

    Linkedlist(Card* c) : card(c), next(nullptr) {}
};

class Stack {
public:
    Stack() : top(nullptr) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(Card* card) {
        Linkedlist* newLinkedlist = new Linkedlist(card);
        newLinkedlist->next = top;
        top = newLinkedlist;
    }

    Card* pop() {
        if (isEmpty()) return nullptr;
        Linkedlist* temp = top;
        Card* card = top->card;
        top = top->next;
        delete temp;
        return card;
    }

    Card* peek() const {
        return top ? top->card : nullptr;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    Linkedlist* getTopLinkedlist() const {
        return top;
    }

    int count() const {
        Linkedlist* current = top;
        int count = 0;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

private:
    Linkedlist* top;
};

class Deck {
public:
    Deck() {
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 1; rank <= 13; ++rank) {
                Card* card = new Card(suit, rank);
                stack.push(card);
            }
        }
        shuffle();
    }

    ~Deck() {
        while (!stack.isEmpty()) {
            delete stack.pop();
        }
    }

    void shuffle() {
        Card* cards[52];
        int index = 0;

        while (!stack.isEmpty()) {
            cards[index++] = stack.pop();
        }

        random_device rd;
        mt19937 g(rd());
        for (int i = 0; i < 52; ++i) {
            int j = i + (rand() % (52 - i)); // Random index for shuffle
            swap(cards[i], cards[j]);
        }

        for (int i = 0; i < 52; ++i) {
            stack.push(cards[i]);
        }
    }

    Card* deal() {
        return stack.pop();
    }

    int cardsRemaining() const {
        return stack.count();
    }

private:
    Stack stack;
    Stack tableauStacks[7];

};

class Tableau {
public:
    Tableau(Deck& deck) {
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j <= i; ++j) {
                Card* card = deck.deal();
                if (j == i) {
                    card->flip();  // Only the top card is face up
                }
                tableauStacks[i].push(card);
            }
        }
    }
    //   Card* peek(int index) {
    //    return tableauStacks[index].getTopLinkedlist()->card; // Assuming getTopLinkedlist() returns a pointer to a struct that contains a Card* pointer
    //}
    Card* peek(int index) {
        if (index < 0 || index >= 7) {

            throw std::out_of_range("Index out of bounds");
        }

        return tableauStacks[index].getTopLinkedlist()->card;
    }
    ~Tableau() {
        for (int i = 0; i < 7; ++i) {
            while (!tableauStacks[i].isEmpty()) {
                delete tableauStacks[i].pop();
            }
        }
    }
    Card* peekTopCard(int index) {
        if (tableauStacks[index].isEmpty()) { // Replace with your condition to check if empty
            return nullptr; // Or throw an error
        }
        return tableauStacks[index].pop(); // Assuming you're using a stack-like structure
    }
    void display() const {
        cout << "Tableau\n";
        for (int i = 0; i < 7; ++i) {
            cout << "\t" << (i + 1);
        }
        cout << "\n";

        for (int i = 0; i < 7; ++i) {
            Linkedlist* current = tableauStacks[i].getTopLinkedlist();
            cout << "\t";
            while (current) {
                if (current->card->isFaceUp()) {
                    cout << current->card->abbreviated();
                }
                else {
                    cout << "x"; // Indicate face-down cards
                }
                cout << "\t";
                current = current->next;
            }
            cout << "\n";
        }
    }

    Card* removeTopCard(int index) {
        if (index < 0 || index >= 7) return nullptr;
        Card* card = tableauStacks[index].pop();
        if (tableauStacks[index].isEmpty()) return card; // No cards left to reveal
        tableauStacks[index].peek()->flip(); // Flip the new top card if it exists
        return card;
    }

    void pushCard(int index, Card* card) {
        if (index < 0 || index >= 7 || card == nullptr) return;
        tableauStacks[index].push(card);
        if (card->isFaceUp()) { // If moving a face-up card, check the new top
            if (!tableauStacks[index].isEmpty()) {
                tableauStacks[index].peek()->flip(); // Flip the new top card
            }
        }
    }

private:
    Stack tableauStacks[7];
};

class Foundation {
public:
    Foundation() {
        for (int i = 0; i < 4; ++i) {
            foundationStacks[i] = Stack();
        }
    }
    Card* peek(int index) {
        return foundationStacks[index].getTopLinkedlist()->card; // Assuming getTopLinkedlist() returns a pointer to a struct that contains a Card* pointer
    }

    bool isEmpty() const {
        // Check if any foundation stack is empty
        for (int i = 0; i < 4; ++i) {
            if (!foundationStacks[i].isEmpty()) {
                return false;
            }
        }
        return true;
    }
    void moveToFoundation(Card* card) {
        // If no card is provided, exit the function
        if (card == nullptr) return;

        // Get the index of the foundation pile based on the card's suit
        int suitIndex = card->getSuit();

        // Check if the foundation stack for the card's suit is empty
        if (foundationStacks[suitIndex].isEmpty()) {
            // If the foundation is empty, only an Ace can be placed
            if (card->getRank() == Card::Ace) {
                foundationStacks[suitIndex].push(card);
            }
        }
        else {
            // Get the top card of the foundation stack
            Card* topCard = foundationStacks[suitIndex].peek();

            // Check if the card can follow the top card (must be the next in rank)
            if (topCard->getRank() == card->getRank() - 1) {
                foundationStacks[suitIndex].push(card);
            }
        }
    }

    void display() const {
        cout << "Foundation\n";
        const char* suits[] = { "C", "H", "S", "D" }; // C: Clubs, H: Hearts, S: Spades, D: Diamonds
        for (int i = 0; i < 4; ++i) {
            cout << "\t" << suits[i];
            if (!foundationStacks[i].isEmpty()) {
                cout << foundationStacks[i].peek()->abbreviated();
            }
            else {
                cout << " ";
            }
            cout << "\n";
        }
    }

    int count() const {
        int total = 0;
        for (const auto& stack : foundationStacks) {
            total += stack.count();
        }
        return total;
    }

private:
    Stack foundationStacks[4];
};

class Solitaire {
public:
    Solitaire() : deck(), tableau(deck), foundations(), wasteCard(nullptr), undoStack(nullptr) {

        cout << "-------------------------------------------------------------------\n";
        cout << "Welcome to Nufil's Solitaire!\n";
        cout << "\nValid Commands: \n";
        cout << "\tmv - move card from Stock to Waste\n";
        cout << "\twf - move card from Waste to Foundation\n";
        cout << "\twt #T - move card from Waste to Tableau\n";
        cout << "\tmt #T - move top card from Tableau to Foundation\n";
        cout << "\tmt #T1 #T2 - move top card from Tableau T1 to Tableau T2\n";
        cout << "\tundo - undo last move\n";
        cout << "\texit - exit the game\n";
        cout << "-------------------------------------------------------------------\n";
        play();
    }

    void play() {
        string command;
        while (true) {
            displayGameState();
            cout << "Enter command: ";
            cin >> command;

            if (command == "mv") {
                moveFromStockToWaste();
            }
            else if (command == "wf") {
                moveFromWasteToFoundation();
            }
            else if (command == "wt") {
                int tableauIndex;
                cin >> tableauIndex;
                if (cin.fail() || tableauIndex < 1 || tableauIndex > 7) {
                    cout << "Invalid input for Tableau index. Please try again.\n";
                    cin.clear();
                    // Clear the input buffer manually by reading until newline
                    while (cin.get() != '\n');
                }
                else {
                    moveFromWasteToTableau(tableauIndex - 1);
                }
            }
            else if (command == "mt") {
                string tableauIndex1, tableauIndex2;
                cin >> tableauIndex1;
                if (isdigit(tableauIndex1[0])) {
                    if (cin >> tableauIndex2 && isdigit(tableauIndex2[0])) {
                        moveBetweenTableaus(stoi(tableauIndex1) - 1, stoi(tableauIndex2) - 1);
                    }
                    else {
                        moveFromTableauToFoundation(stoi(tableauIndex1) - 1);
                    }
                }
                else {
                    cout << "Invalid input for Tableau index. Please try again.\n";
                    cin.clear();
                    // Clear the input buffer manually by reading until newline
                    while (cin.get() != '\n');
                }
            }
            else if (command == "undo") {
                undoLastMove();
            }
            else if (command == "exit") {
                cout << "Thank you for playing!\n";
                break;
            }
            else {
                cout << "Invalid command. Please try again.\n";
            }
            checkWinCondition();
        }
    }


    void moveFromStockToWaste() {
        if (deck.cardsRemaining() > 0) {
            Card* card = deck.deal();
            card->flip(); // Flip the card face up
            wasteCard = card;
            addToUndoStack("Stock to Waste", card);
            cout << "Moved from Stock to Waste: " << card->toString() << endl;
        }
        else {
            cout << "No cards left in the stock!\n";
        }
    }


    void moveFromWasteToFoundation() {
        if (wasteCard) {
            // Check if foundation is empty
            if (foundations.isEmpty()) {
                foundations.moveToFoundation(wasteCard);
                addToUndoStack("Waste to Foundation", wasteCard);
                wasteCard = nullptr; // Reset waste card
                cout << "Moved from Waste to Foundation.\n";
            }
            else {
                // Get the card currently in the foundation
                int index = 0;
                Card* foundationCard = foundations.peek(index); // Assuming peek() gets the top card without removing it

                // Check if the waste card is opposite in color and smaller
                if (isOppositeColor(wasteCard, foundationCard) && isSmaller(wasteCard, foundationCard)) {
                    foundations.moveToFoundation(wasteCard);
                    addToUndoStack("Waste to Foundation", wasteCard);
                    wasteCard = nullptr; // Reset waste card
                    cout << "Moved from Waste to Foundation.\n";
                }
                else {
                    cout << "Invalid operation: Card must be opposite in color and smaller.\n";
                }
            }
        }
        else {
            cout << "No card in waste to move to Foundation!\n";
        }
    }

    // Helper functions to check color and size
    bool isOppositeColor(Card* wasteCard, Card* foundationCard) {
        // Assuming Card has a method to get color
        return (wasteCard->getColor() != foundationCard->getColor());
    }

    bool isSmaller(Card* wasteCard, Card* foundationCard) {
        // Assuming Card has a method to get rank
        return (wasteCard->getRank() < foundationCard->getRank());
    }


    void moveFromWasteToTableau(int index) {
        if (wasteCard) {
            // Get the card currently on top of the tableau
            Card* tableauCard = tableau.peekTopCard(index); // Assuming peekTopCard() retrieves the top card without removing it

            // Check if the tableau card is valid for comparison
            if (tableauCard) {
                if (isOppositeColor(wasteCard, tableauCard) && isSmaller(wasteCard, tableauCard)) {
                    tableau.pushCard(index, wasteCard);
                    addToUndoStack("Waste to Tableau", wasteCard);
                    wasteCard = nullptr; // Reset waste card
                    cout << "Moved from Waste to Tableau " << (index + 1) << ".\n";
                }
                else {
                    cout << "Invalid operation: Card must be opposite in color and smaller.\n";
                }
            }
            else {
                // If the tableau is empty, you can move the card directly
                tableau.pushCard(index, wasteCard);
                addToUndoStack("Waste to Tableau", wasteCard);
                wasteCard = nullptr; // Reset waste card
                cout << "Moved from Waste to Tableau " << (index + 1) << ".\n";
            }
        }
        else {
            cout << "No card in waste to move to Tableau!\n";
        }
    }


    void moveFromTableauToFoundation(int index) {
        Card* card = tableau.removeTopCard(index);
        if (card) {
            // Check if the foundation is empty
            if (foundations.isEmpty()) {
                foundations.moveToFoundation(card);
                addToUndoStack("Tableau to Foundation", card);
                cout << "Moved card from Tableau " << (index + 1) << " to Foundation: " << card->toString() << endl;
            }
            else {
                Card* foundationCard = foundations.peek(index); // Get the top card in the foundation
                if (isOppositeColor(card, foundationCard) && isSmaller(card, foundationCard)) {
                    foundations.moveToFoundation(card);
                    addToUndoStack("Tableau to Foundation", card);
                    cout << "Moved card from Tableau " << (index + 1) << " to Foundation: " << card->toString() << endl;
                }
                else {
                    cout << "Invalid operation: Card must be opposite in color and smaller.\n";
                    tableau.pushCard(index, card); // Put the card back to the tableau if move fails
                }
            }
        }
        else {
            cout << "No card to move from Tableau!\n";
        }
    }



    void moveBetweenTableaus(int fromIndex, int toIndex) {
        Card* card = tableau.removeTopCard(fromIndex);
        if (card) {
            // Get the card currently on top of the target tableau
            Card* targetTableauCard = tableau.peekTopCard(toIndex); // Assuming peekTopCard() retrieves the top card without removing it

            // Check if the target tableau card is valid for comparison
            if (targetTableauCard) {
                if (isOppositeColor(card, targetTableauCard) && isSmaller(card, targetTableauCard)) {
                    tableau.pushCard(toIndex, card);
                    addToUndoStack("Tableau to Tableau", card);
                    cout << "Moved card from Tableau " << (fromIndex + 1) << " to Tableau " << (toIndex + 1) << ": " << card->toString() << endl;
                }
                else {
                    cout << "Invalid operation: Card must be opposite in color and smaller.\n";
                    tableau.pushCard(fromIndex, card); // Put the card back to the original tableau if move fails
                }
            }
            else {
                // If the target tableau is empty, you can move the card directly
                tableau.pushCard(toIndex, card);
                addToUndoStack("Tableau to Tableau", card);
                cout << "Moved card from Tableau " << (fromIndex + 1) << " to Tableau " << (toIndex + 1) << ": " << card->toString() << endl;
            }
        }
        else {
            cout << "No card to move from Tableau!\n";
        }
    }


    void undoLastMove() {
        if (!undoStack) {
            cout << "No moves to undo!\n";
            return;
        }

        string lastAction = undoStack->action;
        Card* lastCard = undoStack->card;

        if (lastAction == "Stock to Waste") {
            wasteCard = lastCard; // Return card to waste
            cout << "Undid move: Stock to Waste, returned " << lastCard->toString() << " to waste.\n";
        }
        else if (lastAction == "Waste to Foundation") {
            foundations.moveToFoundation(lastCard); // Simulate returning card to waste
            wasteCard = lastCard;
            cout << "Undid move: Waste to Foundation, returned " << lastCard->toString() << " to waste.\n";
        }
        else if (lastAction == "Waste to Tableau") {
            tableau.pushCard(0, lastCard); // Return card to tableau (index 0 for simplicity)
            cout << "Undid move: Waste to Tableau, returned " << lastCard->toString() << " to tableau.\n";
        }
        else if (lastAction == "Tableau to Foundation") {
            tableau.pushCard(0, lastCard); // Return card to tableau (index 0 for simplicity)
            cout << "Undid move: Tableau to Foundation, returned " << lastCard->toString() << " to tableau.\n";
        }
        else if (lastAction == "Tableau to Tableau") {
            tableau.pushCard(0, lastCard); // Return card to tableau (index 0 for simplicity)
            cout << "Undid move: Tableau to Tableau, returned " << lastCard->toString() << " to tableau.\n";
        }

        UndoLinkedlist* temp = undoStack;
        undoStack = undoStack->next;
        delete temp;
    }

    void addToUndoStack(const string& action, Card* card) {
        UndoLinkedlist* newUndo = new UndoLinkedlist(action, card);
        newUndo->next = undoStack;
        undoStack = newUndo;
    }

    void displayGameState() const {
        cout << "\n--- Game State ---\n";
        foundations.display();
        tableau.display();
        cout << "Waste: ";
        if (wasteCard) {
            cout << wasteCard->toString() << endl;
        }
        else {
            cout << "Empty\n";
        }
        cout << "-------------------\n";
    }

    void checkWinCondition() {
        if (foundations.count() == 52) {
            cout << "Congratulations! You've won the game!\n";
            exit(0); // End the game
        }
    }

private:
    struct UndoLinkedlist {
        string action;
        Card* card;
        UndoLinkedlist* next;
        UndoLinkedlist(const string& act, Card* c) : action(act), card(c), next(nullptr) {}
    };

    Deck deck;
    Tableau tableau;
    Foundation foundations;
    Card* wasteCard;
    UndoLinkedlist* undoStack;

};

int main() {
    Solitaire solitaireGame;
    return 0;
}
