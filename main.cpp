#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <iterator>

namespace CardVals{
   constexpr std::array<int,150> init{
                            -2,-2,-2,-2,-2,0,0,0,0,0,
                            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                            0,0,0,0,0,0,0,0,0,0,
                            1,1,1,1,1,1,1,1,1,1,
                            2,2,2,2,2,2,2,2,2,2,
                            3,3,3,3,3,3,3,3,3,3,
                            4,4,4,4,4,4,4,4,4,4,
                            5,5,5,5,5,5,5,5,5,5,
                            6,6,6,6,6,6,6,6,6,6,
                            7,7,7,7,7,7,7,7,7,7,
                            8,8,8,8,8,8,8,8,8,8,
                            9,9,9,9,9,9,9,9,9,9,
                            10,10,10,10,10,10,10,10,10,10,
                            11,11,11,11,11,11,11,11,11,11,
                            12,12,12,12,12,12,12,12,12,12
                            };
}


namespace Vars{
    constexpr int NumRows{3};
    constexpr int NumCols{4};
    int FinalPlayer{-1};
}


class Deck{

    private:
    std::vector<int> deck_down{}; // cards to be drawn from
    std::vector<int> deck_pile{}; // cards that have been discarded

    public:

    // Default constructor, this will run when the game is initialized

    Deck(std::array<int,CardVals::init.size()> deck_init){

        std::random_device rd;
        std::mt19937 g(rd());
        
        std::shuffle(deck_init.begin(), deck_init.end(), g);

        for(int j{0};j<CardVals::init.size();j++){
            deck_down.push_back(deck_init[j]);
        };
    }; 

    // We leave out the possibility to implement reshuffling with a constructor for a future work

    /*reshuffles deck according to in game rules*/
    void reshuffle(){
        
        std::random_device rd;
        std::mt19937 g(rd());
                
        int last_card{deck_pile.back()};
        deck_pile.pop_back();

        std::shuffle(deck_pile.begin(), deck_pile.end(), g);

        for(int i{0};i<deck_pile.size();i++){
            deck_down.push_back(deck_pile[i]);
        }

        deck_pile.erase(deck_pile.begin(),deck_pile.end());
        deck_pile.push_back(last_card);
    }

    int draw(){
        
        int currentCard{deck_down.back()};
        deck_down.pop_back();
        return currentCard;

        };

    int peekDeckUp(){return deck_down.back();};
    void drawFromPile(){deck_pile.pop_back();};
    int peekDeckDown(){return deck_pile.back();};

    void discardCard(int card){deck_pile.push_back(card);};

    int cards_left(){return deck_down.size();};
    int discarded_cards(){return deck_pile.size();};
};


class Hand{
    
    private:

        int hand [Vars::NumRows][Vars::NumCols]{}; // the hand to be used by hand[i][j] =  value of card at slot [i,j]
        bool mask [Vars::NumRows][Vars::NumCols]{}; // whether a card is face up or face down

    public: 
        void setHand(Deck& deck){
            int c{0};
            for(int j{0};j<Vars::NumRows;j++){
                for(int k{0};k<Vars::NumCols;k++){
                    hand[j][k] = deck.draw(); // good for initialization, but once the game actually starts, you can not do this.
                    mask[j][k] = 0;
                }
            }
        };

        const int getCard(int j, int k){return hand[j][k];}

        // Later you can replace displayHand() by overloading operator<<

        void displayHand(Deck& deck){

               std::cout<<"\n\n\n";
        
               for(int j{0};j<Vars::NumRows;j++){
                for(int k{0};k<Vars::NumCols;k++){

                    if(mask[j][k]){  
                    std::cout<<hand[j][k]<<' ';}
                    else{std::cout << '*'<<' ';}

                    if(j==1 && k==Vars::NumRows){std::cout<<"\t Card on Table: "<<deck.peekDeckDown();};
                    
                    if(k==Vars::NumRows){std::cout<<'\n';};
                    
                    
                }
                }

                std::cout<<"\n\n\n";
        }

        bool checkColumn(int k){
            
            if(mask[0][k] && mask[1][k] && mask[2][k]){
                if(hand[0][k] == hand[1][k] && hand[1][k] == hand[2][k]){
                    return true;
                }
            };

            return false;
        };

        bool checkOpenHand(){
            int score{0};

            for(int j{0};j<Vars::NumRows;j++){
                for(int k{0};k<Vars::NumCols;k++){
                    score+=mask[j][k];
                }
            }

            return (score==12)?true:false;
        }

        void turnCard(int j, int k){mask[j][k]=1;}
        void updateCard(int j, int k, int val){hand[j][k]=val;}

        int getScore(){
                int score{0};
                
                for(int j{0};j<Vars::NumRows;j++){
                for(int k{0};k<Vars::NumCols;k++){
                    
                    if(mask[j][k]){  
                    score+=hand[j][k];};
                    
                }
                }

                return score;
        };

        // This should probably be inheritance subclass of Player or smtn - (skill issue)
};


class Player{

     private:
        Hand m_hand{};
        int m_id{};
        int loaded_card{};

    public:
        Player()
        {};
       
        Player(Deck& deck){
            m_hand.setHand(deck);
        };

        void setId(int id){m_id=id;};
        const int getId(){return m_id;};

        void setLC(int x){loaded_card=x;}
        const int getLC(){return loaded_card;}

        void setHand(Deck& deck){m_hand.setHand(deck);};
        
        void show_hand(Deck& deck){m_hand.displayHand(deck);} // can be overloaded as std::ostream& operator
        
        void turnCardP(int j, int k){m_hand.turnCard(j,k);};     
        void updateCardP(int j,int k,int val){m_hand.updateCard(j,k,val);};
        const int getCardP(int j,int k){return m_hand.getCard(j,k);};
        bool checkColumnP(int k){return m_hand.checkColumn(k);}
        bool checkOpenHandP(){return m_hand.checkOpenHand();}

        int visScore(){return m_hand.getScore();};
        int m_total_score;
};  


void checkColumnRule(Player& player, Deck& deck, int column){

     if(player.checkColumnP(column)){

                std::cout << "Congratulations, you collected a column!\n";

                deck.discardCard(player.getCardP(0,column));
                deck.discardCard(player.getCardP(1,column));
                deck.discardCard(player.getCardP(2,column));

                player.updateCardP(0,column,0);
                player.updateCardP(1,column,0);
                player.updateCardP(2,column,0);
                
            }

}


int getNumPlayers(){
    
    std::cout<<" Welcome to Skyjo, a terminal implementation!\n";
    std::cout<<" Enter the number of players (2-8 players): ";
    int numPlayers;
    std::cin>>numPlayers;
    return numPlayers;

};

//Setting up the game, returning index of player first to play

int gameInit(Player* PPointer, int NPlayers, Deck& deck){

    deck.discardCard(deck.draw());


    int c{0};
    int FirstToPlay{0};
    
    while(c<2){
     
    for(int i{0};i<NPlayers;i++){
       
        if(!c){
            (PPointer+i)->setId(i+1);
            (PPointer+i)->setHand(deck);
            } // after this loop the game should be initialized - everyone should have their cards.}
        
        std::cout<< "Player "<< (i+1) << ", choose two cards to reveal \n\n\n";

        std::cout<<"Enter a row: ";        
        int row{};
        std::cin>> row;

        std::cout<<"Enter a column: ";
        int column{};

        std::cin >> column;
        
        (PPointer+i)->turnCardP(row,column);

        std::cout<< "Player "<< (i+1) << ", your hand looks like this:\n";
    
        (PPointer+i)->show_hand(deck);
    
        if(c){

            if((PPointer+i)->visScore()>(PPointer+FirstToPlay)->visScore()){
                FirstToPlay = i;
            }


        };
    };
    c++;
    };

    std::cout << "Player First To Play: " << FirstToPlay+1 << "\n\n\n\n";
    return FirstToPlay;

};
 
void Turn(Player& player, Deck& deck){

    char c{};

    std::cout << "Player " << player.getId() << " , your turn!\n";
    std::cout << "Your hand looks like this: \n\n\n";
    
    player.show_hand(deck);

    std::cout <<" \n\n\n";

    std::cout <<" Enter 'u' to draw from the Face down pile, enter 'd' to pick up the card on the table\n";
    
    std::cin>>c;
    
    if(c=='u'){
       
        player.setLC(deck.draw());
        std::cout << "You have drawn " << player.getLC() << "\n What do you want to do ?\n\n";
        std::cout << "Enter 'k' to keep and 'd' to drop \n";  
        char choice{};
        std::cin>>choice;

        if(choice=='k'){
            std::cout << "You have chosen to keep a " << player.getLC() <<'\n';
            std::cout << "Where do you want to place it ?\n";

            std::cout << "Enter row: ";
            
            int row;
            std::cin >> row;

            std::cout << "Enter column: ";

            int column; 
            std::cin >> column;

            deck.discardCard(player.getCardP(row,column));
            std::cout << "You have dropped a "<< player.getCardP(row,column) << '\n';

            player.updateCardP(row,column,player.getLC());
            player.turnCardP(row,column);
            checkColumnRule(player,deck,column);  
        } 

        else if(choice=='d'){

          std::cout << "You have chosen to discard " << player.getLC() << "\n";
          
          deck.discardCard(player.getLC());
          
          std::cout << "However, you now need to reveal a face down card from your hand\n";
          std::cout << "Enter row: ";

          int row;
          std::cin >> row;
          
          std::cout << "Enter column: ";
          
          int column;
          std::cin >> column;
          
          // In the future, we will check if this card is already face up - input validatio               n will be done at a later point
          player.turnCardP(row,column);
          checkColumnRule(player,deck,column);
        };

    }; 

    if(c=='d'){
            
            int temp_int{deck.peekDeckDown()};
            deck.drawFromPile();

            std::cout << "You have picked up a" << temp_int << '\n';
            std::cout << "Where do you want to place it ? Enter row and column\n";

            std::cout << "Enter row: ";
            int row;
            std::cin >> row;

            std::cout<< "Enter column: ";
            int column;

            std::cin >> column;

            deck.discardCard(player.getCardP(row,column));
            std::cout << " You have dropped a " << player.getCardP(row,column) << '\n';

            player.updateCardP(row,column,temp_int);
            player.turnCardP(row,column);
            checkColumnRule(player,deck,column);
           
    };
	std::cout<< "Player " << player.getId() << " your hand looks like this:\n";
	player.show_hand(deck);
	if(player.checkOpenHandP()){Vars::FinalPlayer=player.getId();};
};

int main(){

     std::cout << "\n\n\n\n\n";
     std::cout << "=========================================================================================================\n";       std::cout << "=========================================================================================================\n";
     std::cout << "=========================================================================================================\n";

    Deck deck{CardVals::init};


    const int NumPlayers{getNumPlayers()};

    // memory addresses for NumPlayers declared below

    Player * PPointer = new Player[NumPlayers];

    // Cards are distributed and each player reveals 2 cards below
    // At the same time, counter is initialized that points to the first player

    int loop{gameInit(PPointer,NumPlayers,deck)};

    // used for properly terminating the game
    int final_counter{1};

    while(true){
   
        
        Turn(*(PPointer+loop),deck);
        
        if(deck.cards_left()==0){
        std::cout << " Time to reshuffle the deck\n ";
        deck.reshuffle();}
        
	(loop<NumPlayers-1)?loop++:loop=0;
        

	if(Vars::FinalPlayer>-1){ // this is a namespaced global variable that indicates someone has opened his hand

            final_counter++;

            };
        if(final_counter==NumPlayers+1){break;}

	
    };

    std::cout << "\n\n\n\n\n";	
    std::cout << " Scores : \n\n\n";

 
    for(int i{0};i<NumPlayers;i++){
	std::cout << "Player " << (PPointer+i)->getId() << " : " << (PPointer+i)->visScore() << '\n'; 
    }

    std::cout << "\n\n\n\n\n";

    std::cout << "Game ended :)\n";
    std::cout << "=========================================================================================================\n";	      std::cout << "=========================================================================================================\n";    	std::cout << "=========================================================================================================\n";
    delete PPointer;
    return 0;
}
