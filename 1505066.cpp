#include <iostream>
#include <ctime>
using namespace std ;

class MancalaBoard{
private:
    int board[3][6] ;
    int player_storage[3] ;

public:
    MancalaBoard(){
        for(int i=0;i<6;i++){
            board[1][i] = board[2][i] = 4 ;
        }
        player_storage[1] = 0 ;
        player_storage[2] = 0 ;
    }

    int update_board(int player,int bin){
        int current = board[player][bin-1] ;
        board[player][bin-1] = 0 ;

        int opponent = 1 ;
        if(player==1) opponent = 2 ;
        int i = bin ;
        while(i<6 && current>0){
            if(board[player][i]==0 && current==1){
                player_storage[player] += 1 + board[opponent][5-bin+1] ;
                board[opponent][5-bin] = 0 ;
                return opponent ;
            }
            board[player][i]++ ;
            current-- ;
            i++ ;
        }
        if(current==1){
            player_storage[player] += 1 ;
            return player ;
        }
        else if(current>1){
            player_storage[player] += 1 ;
            current-- ;
        }
        int j = 0 ;
        while(current>0){
            board[opponent][5-j]++ ;
            current-- ;
            j++ ;
        }
        return opponent ;
    }
    void print_board(){
        cout<<"Player 1"<<endl ;
        for(int i=0;i<6;i++){
            cout<<board[1][i]<<" " ;
        }
        cout<<"Storage 1 : "<<player_storage[1]<<endl ;
        cout<<endl ;
        cout<<"Player 2"<<endl ;
        for(int i=0;i<6;i++){
            cout<<board[2][i]<<" " ;
        }

        cout<<"Storage 2 : "<<player_storage[2]<<endl ;
        cout<<endl ;
    }

    bool is_game_over(){
        if(player_storage[1] + player_storage[2] == 48) return true ;

        int sum1 = 0 ;
        int sum2 = 0 ;

        for(int i=0;i<6;i++){
            sum1 += board[1][i] ;
            sum2 += board[2][i] ;
        }

        if(!sum1 || !sum2){
            return true ;
        }

        return false ;
    }

    int checkMove(int player,int bin){
        if(board[player][bin-1]){
            return 1 ;
        }
        else{
            return 0 ;
        }
    }

    int check_winner(){
        if(this->player_storage[1] > this->player_storage[2]){
            return 1 ;
        }
        else if(this->player_storage[2] > this->player_storage[1]) return 2 ;
        else return 0 ;
    }

    int heuristic1(int player){
        int opponent = 1 ;
        if(player==1) opponent = 2 ;
        return player_storage[player] - player_storage[opponent] ;
    }

    int heuristic2(int player){
        int opponent = 1 ;
        if(player==1) opponent = 2 ;
        int r1 = rand()%100 ;
        int r2 = rand()%100 ;
        return r1*player_storage[player] - r2*player_storage[opponent] ;
    }

    int heuristic3(int player,int add){
        int opponent = 1 ;
        if(player==1) opponent = 2 ;
        int my_side = 0,opponent_side = 0;
        for(int i=0;i<6;i++){
            my_side += board[player][i] ;
            opponent_side += board[opponent][i] ;
        }
        int r1 = rand()%100 ;
        int r2 = rand()%100 ;
        int r3 = rand()%100 ;
        return r1*(player_storage[player] - player_storage[opponent]) +  r2*(my_side-opponent_side) + r3*add ;
    }

    int heuristic4(int player,int add){
        int opponent = 1 ;
        if(player==1) opponent = 2 ;
        int my_side = 0,opponent_side = 0;
        for(int i=0;i<6;i++){
            my_side += board[player][i] ;
            opponent_side += board[opponent][i] ;
        }
        int r1 = rand()%100 ;
        int r2 = rand()%100 ;
        int r3 = rand()%100 ;
        int r4 = rand()%100 ;
        return r1*(player_storage[player] - player_storage[opponent]) +  r2*(my_side-opponent_side) + r3*add + r4*(player_storage[player] + player_storage[opponent]);
    }

    int heuristic5(int player,int add){
        int opponent = 1 ;
        if(player==1) opponent = 2 ;
        int my_side = 0,opponent_side = 0;
        for(int i=0;i<6;i++){
            my_side += board[player][i] ;
            opponent_side += board[opponent][i] ;
        }
        int r1 = rand()%100 ;
        int r2 = rand()%100 ;
        int r3 = rand()%100 ;
        int r4 = rand()%100 ;
        int r5 = rand()%10 ;
        int r6 = rand()%10 ;
        return r5*(r1*(player_storage[player] - player_storage[opponent]) +  r2*(my_side-opponent_side) )+ r6*(r3*add + r4*(player_storage[player] + player_storage[opponent]));
    }

    int heuristic6(int player,int add){
        int opponent = 1 ;
        if(player==1) opponent = 2 ;
        int my_side = 0,opponent_side = 0;
        for(int i=0;i<6;i++){
            my_side += board[player][i] ;
            opponent_side += board[opponent][i] ;
        }
        int r1 = rand()%100 ;
        int r2 = rand()%100 ;
        int r3 = rand()%100 ;
        int r4 = rand()%100 ;
        return r1*(player_storage[player] - player_storage[opponent]) +  r2*(r3*add + r4*(player_storage[player] + player_storage[opponent]));
    }
};

class MancalaPlayer{
private:
    int player ;
    int opponent ;
    int turn ;
    int depth ;
    int bonus ;
    int current_add ;
public:
    MancalaPlayer(int player,int depth){
        this->player = player ;
        if(player==1) opponent = 2 ;
        else opponent = 1 ;
        this->depth = depth ;
        this->bonus = 0 ;
        this->current_add = 0 ;
    }

    int next_move(MancalaBoard b){
        return MiniMax(b,true,this->depth,-1e9,+1e9) ;
    }

    int MiniMax(MancalaBoard b,bool max_,int depth,int alpha,int beta){
        if(depth==0){
                int r = rand()%4+ 1 ;
                if(r==1){
                    return b.heuristic1(this->player) ;
                }
                if(r==2){
                    return b.heuristic2(this->player) ;
                }
                if(r==3){
                    return b.heuristic3(this->player,this->current_add) ;
                }
                if(r==4){
                    return b.heuristic4(this->player,this->current_add) ;
                }
                 if(r==5){
                    return b.heuristic5(this->player,this->current_add) ;
                }
                if(r==6){
                    return b.heuristic6(this->player,this->current_add) ;
                }
        }
        if(max_){
            int max_value = -1e9 ;
            int move = -1 ;
            int current = -1 ;
            for(int i=1;i<=6;i++){
                MancalaBoard tem ;
                tem = b ;
                if(tem.checkMove(player,i)){
                    bonus = tem.update_board(player,i) ;
                    if(bonus==player){
                        current_add++ ;
                        current = MiniMax(tem,true,depth-1,alpha,beta) ;
                        current_add-- ;
                    }
                    else{
                        current = MiniMax(tem,false,depth-1,alpha,beta) ;
                    }

                    if(current>max_value){
                        max_value = current ;
                        move = i ;
                    }
                    alpha = max_value>alpha?max_value:alpha ;
                    if(beta<=alpha) break ;
                }
            }

            return move;
        }
        else{
            int min_value = 1e9 ;
            int current = -1 ;
            for(int i=1;i<=6;i++){
                MancalaBoard tem ;
                tem = b ;
                if(tem.checkMove(opponent,i)){
                    bonus = tem.update_board(opponent,i) ;
                    if(bonus==opponent){
                        current = MiniMax(tem,false,depth-1,alpha,beta) ;
                    }
                    else{
                        current = MiniMax(tem,true,depth-1,alpha,beta) ;
                    }

                    if(current<min_value){
                        min_value = current ;
                    }
                    beta = min_value<beta?min_value:beta ;
                    if(beta<=alpha) break ;
                }
            }

            return min_value ;
        }
    }
};


int play(){
    MancalaBoard b ;
    MancalaPlayer p1(1,10) ;
    MancalaPlayer p2(2,5) ;
    int turn = 1 ;
    int move_index = -1 ;
    int i=0 ;
    //b.print_board() ;
    srand(time(NULL)) ;
    while(!b.is_game_over()){
        cout<<"Player "<<turn<<"'s Move"<<endl ;

        if(turn == 1){
            move_index = p1.next_move(b) ;
        }
        else{
            move_index = p2.next_move(b) ;
        }
        cout<<"Move index: "<<move_index<<endl ;
        turn = b.update_board(turn,move_index) ;
        b.print_board() ;
        cout<<endl ;
        i++ ;
    }

    return b.check_winner() ;
}

int main(){
    int p1 = 0 ;
    int p2 = 0 ;
    for(int i=0;i<100;i++){
        int c = play() ;
        if(c==1){
            p1++ ;
        }
        else p2++ ;
    }

    cout<<"Player 1:: "<<p1<<endl ;
    cout<<"Player 2:: "<<p2<<endl ;

    return 0 ;
}
