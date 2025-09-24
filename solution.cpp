#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
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

void spawn_tile(vector<vector<int>>& board) {
    vector<int> rows;
    vector<int> cols;

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (board[r][c] == 0) {
                rows.push_back(r);
                cols.push_back(c);
            }
        }
    }

    if (rows.empty()) return;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> idx_dist(0, rows.size() - 1);
    int idx = idx_dist(gen);

    int r = rows[idx];
    int c = cols[idx];

    std::uniform_int_distribution<int> val_dist(1, 10);
    int roll = val_dist(gen);
    board[r][c] = (roll == 1 ? 4 : 2);
}


std::vector<int> compress_row(const std::vector<int>& row) {
    std::vector<int> compressed;
    std::copy_if(row.begin(), row.end(), std::back_inserter(compressed), [](int x){return x != 0;});
    while (compressed.size() < 4) {
        compressed.push_back(0);
    }
    return compressed;
}

std::vector<int> merge_row(std::vector<int> row) {
    for (int i = 0; i < 3; ++i) {
        if (row[i] != 0 && row[i] == row[i+1]) {
            row[i] *= 2;
            row[i+1] = 0;
            ++i;
        }
    }
    return compress_row(row);
}
bool move_left(vector<vector<int>>& board){
    bool moved = false;
    for(int r = 0; r < 4; r++) {
        vector<int> new_row = merge_row(board[r]);
        if (new_row != board[r]) {
            moved = true;
        }
        board[r] = new_row;
    }
    return moved;
}

bool move_right(vector<vector<int>>& board){bool moved = false;
    for (int r = 0; r < 4; r++) {
        vector<int> row = board[r];
        std::reverse(row.begin(), row.end());
        row = merge_row(row);
        std::reverse(row.begin(), row.end());
        if (row != board[r]) {
            moved = true;
        }
        board[r] = row;
    }
    return moved;
}
bool move_up(vector<vector<int>>& board){bool moved = false;
    for (int c = 0; c < 4; c++) {
        vector<int> col;
        for (int r = 0; r < 4; r++) {
            col.push_back(board[r][c]);
        }

        vector<int> new_col = merge_row(col);

        for (int r = 0; r < 4; r++) {
            if (board[r][c] != new_col[r]) {
                moved = true;
            }
            board[r][c] = new_col[r];
        }
    }    
    return moved;
}
bool move_down(vector<vector<int>>& board){
    bool moved = false;
    for (int c = 0; c < 4; c++) {
        vector<int> col;
        for (int r = 0; r < 4; r++) {
            col.push_back(board[r][c]);
        }

        std::reverse(col.begin(), col.end());
        col = merge_row(col);
        std::reverse(col.begin(), col.end());

        for (int r = 0; r < 4; r++) {
            if (board[r][c] != col[r]) {
                moved = true;
            }
            board[r][c] = col[r];
        }
    }
    return moved;
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
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            if(!history.empty()) {
                board = history.top();
                history.pop();
            }
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
