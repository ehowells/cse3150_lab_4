
#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric> // needed for accumulate in compute_score
#include <random>

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    for (const auto &row : board) {
        for (auto val : row) {
            if (val == 0) cout << ".\t";
            else cout << val << "\t";
        }
        cout << "\n";
    }
    cout << endl;
    write_board_csv(board, first);
}

// New functions to remove zeros and merge tiles
vector<int> compress(const vector<int>& row) {
    vector<int> compressed;
    std::copy_if(row.begin(), row.end(), std::back_inserter(compressed),
                 [](int val){ return val != 0; });
    while (compressed.size() < 4) {
        compressed.push_back(0);
    }
    return compressed;
}

vector<int> merge_row(const vector<int>& row) {
    vector<int> compressed = compress(row);

    for (int i = 0; i < 3; i++) {
        if (compressed[i] != 0 && compressed[i] == compressed[i+1]) {
            compressed[i] *= 2;
            compressed[i+1] = 0;
        }
    }

    return compress(compressed);
}

void spawn_tile(vector<vector<int>>& board) {
    vector<int> empty_rows;
    vector<int> empty_cols;

    for (int r=0;r<4;r++)
        for (int c=0;c<4;c++)
            if (board[r][c]==0) { 
                empty_rows.push_back(r);
                empty_cols.push_back(c);
            }

    if (empty_rows.empty()) return;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, empty_rows.size() - 1);

    int idx = dist(gen);
    int r = empty_rows[idx];
    int c = empty_cols[idx];

    std::uniform_int_distribution<> tile_dist(1, 10);
    board[r][c] = (tile_dist(gen) == 1) ? 4 : 2;        
}

bool move_left(vector<vector<int>>& board){
    bool moved = false;
    for(int r = 0; r < 4; r++) {
        vector<int> new_row = merge_row(board[r]);
        if (new_row != board[r]) moved = true;
        board[r] = new_row;
    }
    return moved;
}

bool move_right(vector<vector<int>>& board){
    bool moved = false;
    for (int r = 0; r < 4; r++) {
        vector<int> row = board[r];
        std::reverse(row.begin(), row.end());
        row = merge_row(row);
        std::reverse(row.begin(), row.end());
        if (row != board[r]) moved = true;
        board[r] = row;
    }
    return moved;
}
bool move_up(vector<vector<int>>& board){
    bool moved = false;
    for (int c = 0; c < 4; c++) {
        vector<int> col;
        for (int r = 0; r < 4; r++) col.push_back(board[r][c]);

        vector<int> new_col = merge_row(col);

        for (int r = 0; r < 4; r++) {
            if (board[r][c] != new_col[r]) moved = true;
            board[r][c] = new_col[r];
        }
    }    
    return moved;
}
bool move_down(vector<vector<int>>& board){
    bool moved = false;
    for (int c = 0; c < 4; c++) {
        vector<int> col;
        for (int r = 0; r < 4; r++) col.push_back(board[r][c]);

        std::reverse(col.begin(), col.end());
        col = merge_row(col);
        std::reverse(col.begin(), col.end());

        for (int r = 0; r < 4; r++) {
            if (board[r][c] != col[r]) moved = true;
            board[r][c] = col[r];
        }
    }
    return moved;
}

int compute_score(const vector<vector<int>>& board) {
    int score = 0;
    for(const auto& row : board) {
        score += accumulate(row.begin(), row.end(), 0);
    }
    return score;
}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board, first);
        first=false;
        cout << "Score: " << compute_score(board) << "\n";
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            if(!history.empty()) {
                board = history.top();
                history.pop();
            }
            print_board(board,false);
            continue;
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            history.push(prev);
            spawn_tile(board);
        }
    }
    return 0;
}
