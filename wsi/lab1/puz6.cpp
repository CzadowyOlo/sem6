#include <algorithm>
#include <ctime>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <numeric>
#include <functional>
#include <array>
#include <unordered_map>
#include <map>
#include <sstream>
#include <fstream>

using namespace std;
static int dupsko = 0;

typedef vector<int> Board;
const int moves[] = {-1, 1, -4, 4};

struct State {
    long long board_hex;
    int g, h, f, empty_pos;
};

long long vectorToHex(const std::vector<int> &vec) {
    long long hexNum = 0;
    for (int num : vec) {
        hexNum = (hexNum << 4) | (num & 0x0F);
    }
    return hexNum;
}

std::vector<int> hexToVector(long long hexNum) {
    std::vector<int> vec(16);
    for (int i = 15; i >= 0; --i) {
        vec[i] = hexNum & 0x0F;
        hexNum >>= 4;
    }
    return vec;
}

bool in_bounds(int pos, int move) {
    int new_pos = pos + move;
    if (new_pos < 0 || new_pos >= 16) {
        return false;
    }

    // Sprawdź, czy ruch jest w tym samym wierszu
    if (abs(move) == 1 && pos / 4 == new_pos / 4) {
        return true;
    }

    // Sprawdź, czy ruch jest w tej samej kolumnie
    if (abs(move) == 4) {
        return true;
    }

    return false;
}

bool is_goal(const Board &board) {
    return is_sorted(board.begin(), board.end() - 1) && board[0] == 1;
}

int manhattan_distance(const Board &board) {
    int distance = 0;
    for (int i = 0; i < 16; ++i) {
        if (board[i] == 0){
            continue;
        }

        int goal_row = (board[i] - 1) / 4;
        int goal_col = (board[i] - 1) % 4;
        distance += abs(goal_row - (i / 4)) + abs(goal_col - (i % 4));
    }
    return distance;
}

array<int, 16> pattern_555 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

int pattern_database_555(const Board &board) {
    int distance = 0;
    for (int i = 0; i < 16; ++i) {
        if (board[i] == 0) continue;
        int idx = find(pattern_555.begin(), pattern_555.end(), board[i]) - pattern_555.begin();
        distance += abs(idx / 4 - i / 4) + abs(idx % 4 - i % 4);
    }
    return distance;
}

int linear_conflict(const Board &board) {
    int conflicts = 0;
    for (int i = 0; i < 16; ++i) {
        if (board[i] == 0) continue;
        int goal_row = (board[i] - 1) / 4;
        int goal_col = (board[i] - 1) % 4;
        if (i / 4 == goal_row) {
            for (int j = i + 1; j < 16; ++j) {
                if (board[j] != 0 && j / 4 == goal_row && board[j] < board[i] && (board[j] - 1) / 4 == goal_row) {
                    conflicts++;
                }
            }
        }
        if (i % 4 == goal_col) {
            for (int j = i + 4; j < 16; j += 4) {
                if (board[j] != 0 && j % 4 == goal_col && board[j] < board[i] && (board[j] - 1) % 4 == goal_col) {
                    conflicts++;
                }
            }
        }
    }
    return manhattan_distance(board) + 2 * conflicts;
}



int corner_tiles(const Board &board) {
    int corner_penalty = 0;
    const int corner_positions[] = {0, 3, 12, 15};
    const int adjacent_positions[][2] = {{1, 4}, {2, 7}, {8, 11}, {13, 14}};
    
    for (int i = 0; i < 4; ++i) {
        int pos = corner_positions[i];
        int correct_value = pos + 1;
        if (board[pos] != 0 && board[pos] != correct_value) {
            corner_penalty += 1;

            // Dodatkowe sprawdzenie dla przyległych kafelków
            for (int j = 0; j < 2; ++j) {
                int adj_pos = adjacent_positions[i][j];
                if (board[adj_pos] != 0 && board[adj_pos] != adj_pos + 1) {
                    corner_penalty++;
                }
            }
        }
    }
    return manhattan_distance(board) + corner_penalty;
}

int relaxed_adjacency(const std::vector<int> &board) {
    int count = 0;

    // Sprawdzanie rzędów
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 3; ++col) {
            int current_tile = board[row * 4 + col];
            int next_tile = board[row * 4 + col + 1];

            if (current_tile != 0 && next_tile != 0 &&
                abs(current_tile - next_tile) != 1) {
                count++;
            }
        }
    }

    // Sprawdzanie kolumn
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 3; ++row) {
            int current_tile = board[row * 4 + col];
            int next_tile = board[(row + 1) * 4 + col];

            if (current_tile != 0 && next_tile != 0 &&
                abs(current_tile - next_tile) != 4) {
                count++;
            }
        }
    }

    return count + manhattan_distance(board);
}


int gruba_suka(const Board &board){

    int corner_penalty = 0;
    const int corner_positions[] = {0, 3, 12, 15};
    const int adjacent_positions[][2] = {{1, 4}, {2, 7}, {8, 11}, {13, 14}};
    
    for (int i = 0; i < 4; ++i) {
        int pos = corner_positions[i];
        int correct_value = pos + 1;
        if (board[pos] != 0 && board[pos] != correct_value) {
            corner_penalty += 1;

            // // Dodatkowe sprawdzenie dla przyległych kafelków
            // for (int j = 0; j < 2; ++j) {
            //     int adj_pos = adjacent_positions[i][j];
            //     if (board[adj_pos] != 0 && board[adj_pos] != adj_pos + 1) {
            //         corner_penalty++;
            //     }
            // }
        }
    }
    return linear_conflict(board) + corner_penalty;

}


bool is_permutation_solvable(const vector<int> &perm) {
    int inversion_count = 0;
    for (size_t i = 0; i < perm.size(); ++i) {
        if (perm[i] == 0)
            continue;
        for (size_t j = i + 1; j < perm.size(); ++j) {
            if (perm[j] != 0 && perm[i] > perm[j]) {
                inversion_count++;
            }
        }
    }
    int row_of_blank = (find(perm.begin(), perm.end(), 0) - perm.begin()) / 4;
    return (inversion_count + row_of_blank) % 2 == 0;
}

Board generate_board() {
    Board board(16);
    board[15] = 0;
    iota(board.begin(), board.end() - 1, 1);
    do {
        random_shuffle(board.begin(), board.end() - 1);
    } while (!is_permutation_solvable(board));
    return board;
}

struct VectorHash {
    size_t operator()(const vector<int> &v) const {
        size_t seed = v.size();
        for (const auto &elem : v) {
            seed ^= elem + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

Board dupav;
Board dupavnext;

bool solve_clear(State &initial_state, function<int(const Board &)> heuristic, int &visited_states, vector<Board> &solution) {
    dupav = hexToVector((initial_state.board_hex));
    initial_state.h = heuristic(dupav);
    initial_state.f = initial_state.g + initial_state.h;

    auto cmp = [](const State &a, const State &b) { return a.f > b.f; };
    priority_queue<State, vector<State>, decltype(cmp)> open_queue(cmp);
    unordered_map<long long, State> state_map;
    unordered_set<long long> closed_set;
    open_queue.push(initial_state);
    state_map[initial_state.board_hex] = initial_state;
    visited_states = 0;
    solution.clear();
    int current_hash;
    int valf;

    while (!open_queue.empty()) {
        State current = open_queue.top();
        open_queue.pop();
        visited_states++;
        current_hash = 0;
        dupav = hexToVector(current.board_hex);

        if (closed_set.count(current.board_hex)) {
            continue;
        }
        closed_set.insert(current.board_hex);

        if (is_goal(dupav)) {
            initial_state = current;
            return true;
        }

        if(initial_state.g > 80){
            return false;
        }

        for (int move : moves) {
            if (in_bounds(current.empty_pos, move)) {
                State next = current;
                next.g++;
                next.empty_pos = current.empty_pos + move;
                dupav = hexToVector(current.board_hex);
                swap(dupav[current.empty_pos], dupav[next.empty_pos]);
                next.board_hex = vectorToHex(dupav);
                next.h = heuristic(dupav);
                next.f = next.g + next.h;

                if (state_map.find(next.board_hex) == state_map.end() || state_map[next.board_hex].f > next.f ) {
                    open_queue.push(next);
                    state_map[next.board_hex] = next;
                } 
            }
        }
    }
    return false;
}

bool solve(State &initial_state, function<int(const Board &)> heuristic, int &visited_states, vector<Board> &solution) {
    dupav = hexToVector((initial_state.board_hex));
    initial_state.h = heuristic(dupav);
    initial_state.f = initial_state.g + initial_state.h;

    auto cmp = [](const State &a, const State &b) { return a.f > b.f; };
    priority_queue<State, vector<State>, decltype(cmp)> open_queue(cmp);
    unordered_map<long long, State> state_map;
    unordered_map<long long, long long> prev_board_map; // Mapa do przechowywania poprzednich stanów
    unordered_set<long long> closed_set;
    open_queue.push(initial_state);
    state_map[initial_state.board_hex] = initial_state;
    visited_states = 0;
    solution.clear();
    int current_hash;
    int valf;

    while (!open_queue.empty()) {
        State current = open_queue.top();
        open_queue.pop();
        visited_states++;
        current_hash = 0;
        dupav = hexToVector(current.board_hex);

        if (closed_set.count(current.board_hex)) {
            continue;
        }
        closed_set.insert(current.board_hex);

        if (is_goal(dupav)) {
            initial_state = current;
            long long boardzik = current.board_hex;
            while (prev_board_map.find(boardzik) != prev_board_map.end()) {
                solution.push_back(hexToVector(boardzik));
                boardzik = prev_board_map[boardzik];
            }
            reverse(solution.begin(), solution.end());
            return true;
        }

        if(initial_state.g > 80){
            return false;
        }

        for (int move : moves) {
            if (in_bounds(current.empty_pos, move)) {
                State next = current;
                next.g++;
                next.empty_pos = current.empty_pos + move;
                dupav = hexToVector(current.board_hex);
                swap(dupav[current.empty_pos], dupav[next.empty_pos]);
                next.board_hex = vectorToHex(dupav);
                next.h = heuristic(dupav);
                next.f = next.g + next.h;

                if (state_map.find(next.board_hex) == state_map.end() || state_map[next.board_hex].f > next.f ) {
                    open_queue.push(next);
                    state_map[next.board_hex] = next;
                    prev_board_map[next.board_hex] = current.board_hex; // Zapisujemy board poprzedniego stanuu

                } 
            }
        }
    }
    return false;
}


std::vector<int> wczytaj_plik(const std::string& nazwa_pliku) {
    std::ifstream plik(nazwa_pliku);
    if (!plik.is_open()) {
        std::cerr << "Kurwa, nie można otworzyć pliku: " << nazwa_pliku << std::endl;
        exit(1);
    }

    std::string linia;
    std::vector<int> liczby;
    while (std::getline(plik, linia, ',')) {
        int liczba;
        std::istringstream iss(linia);
        iss >> liczba;
        liczby.push_back(liczba);
    }

    plik.close();
    return liczby;
}



int main(int argc, char **argv) {
    srand(time(0));
    std::string nazwa_pliku = argv[1];
    int print_path = stoi(argv[2]);
    Board board = wczytaj_plik(nazwa_pliku);
    vector<Board> solution;
    //Board board = generate_board();
    //Board board = {1, 2, 3, 4, 10, 0, 11, 8, 6, 7, 5, 12, 9, 13, 14, 15};
    State initial_state1{vectorToHex(board), 0, 0, 0, (int)(find(board.begin(), board.end(), 0) - board.begin())};
    State initial_state2{vectorToHex(board), 0, 0, 0, (int)(find(board.begin(), board.end(), 0) - board.begin())};
    State initial_state3{vectorToHex(board), 0, 0, 0, (int)(find(board.begin(), board.end(), 0) - board.begin())};
    State initial_state4{vectorToHex(board), 0, 0, 0, (int)(find(board.begin(), board.end(), 0) - board.begin())};

    if(print_path != -1){
        cout<<initial_state1.empty_pos<<endl;
    }


    int visited_states;

    State init = initial_state1;
    bool gowno;

        if(print_path == 1){
            gowno = solve(initial_state2, gruba_suka, visited_states, solution);
        }
        else{
            gowno = solve_clear(initial_state2, gruba_suka, visited_states, solution);
        }

        if (gowno) {
        cout << "Solved using GRUBA HEURA heuristic:" << endl;
        cout << "Number of steps: " << initial_state2.g << endl;
        cout << "Visited states: " << visited_states << endl;
        if(print_path != -1){
        cout << "Solution" << endl;
        Board newboard;
        newboard = hexToVector(init.board_hex);
        cout << "Initial board:" << endl;
            for (int i = 0; i < 16; ++i) {
                cout << newboard[i] << (i % 4 == 3 ? "\n" : " ");
            }
            cout<<endl;
        }

        if(print_path == 1){
            for(const auto &state : solution){
                for (int i = 0; i < 16; ++i) {
                    cout << state[i] << (i % 4 == 3 ? "\n" : " ");
                }        
                cout<<endl;
            }
        }
        else if(print_path != -1){
            cout << "Final board:" << endl;
            Board finalboard = hexToVector(initial_state2.board_hex);
            for (int i = 0; i < 16; ++i) {
                cout << finalboard[i] << (i % 4 == 3 ? "\n" : " ");
            }
        }

    } else {
        cout << "Unsolved using GRUBA HEURA heuristic." << endl;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////
    // if(print_path == 1){
    //     gowno = solve(initial_state3, relaxed_adjacency, visited_states, solution);
    // }
    // else{
    //     gowno = solve_clear(initial_state3, relaxed_adjacency, visited_states, solution);
    // }
    // if (gowno) {
    // cout << "Solved using NILSSON SEQU heuristic:" << endl;
    // cout << "Number of steps: " << initial_state3.g << endl;
    // cout << "Visited states: " << visited_states << endl;
    // if(print_path != -1){
    // cout << "Solution" << endl;
    // Board newboard;
    // newboard = hexToVector(init.board_hex);
    // cout << "Initial board:" << endl;
    //     for (int i = 0; i < 16; ++i) {
    //         cout << newboard[i] << (i % 4 == 3 ? "\n" : " ");
    //     }
    //     cout<<endl;
    // }
    // if(print_path == 1){
    //     for(const auto &state : solution){
    //         for (int i = 0; i < 16; ++i) {
    //             cout << state[i] << (i % 4 == 3 ? "\n" : " ");
    //         }        
    //         cout<<endl;
    //     }
    // }
    // else if(print_path != -1){
    //     cout << "Final board:" << endl;
    //     Board finalboard = hexToVector(initial_state3.board_hex);
    //     for (int i = 0; i < 16; ++i) {
    //         cout << finalboard[i] << (i % 4 == 3 ? "\n" : " ");
    //     }
    // }

    // } else {
    //     cout << "Unsolved using NILSSON SEQU heuristic." << endl;
    // }
////////////////////////////////////////////////////////////////////////////////////////////////
    if(print_path == 1){
        gowno = solve(initial_state4, linear_conflict, visited_states, solution);
    }
    else{
        gowno = solve_clear(initial_state4, linear_conflict, visited_states, solution);
    }
    if (gowno) {
    cout << "Solved using LINEAR CONFLICT heuristic:" << endl;
    cout << "Number of steps: " << initial_state4.g << endl;
    cout << "Visited states: " << visited_states << endl;
    if(print_path != -1){
    cout << "Solution" << endl;
    Board newboard;
    newboard = hexToVector(init.board_hex);
    cout << "Initial board:" << endl;
        for (int i = 0; i < 16; ++i) {
            cout << newboard[i] << (i % 4 == 3 ? "\n" : " ");
        }
        cout<<endl;
    }
    if(print_path == 1){
        for(const auto &state : solution){
            for (int i = 0; i < 16; ++i) {
                cout << state[i] << (i % 4 == 3 ? "\n" : " ");
            }        
            cout<<endl;
        }
    }
    else if(print_path != -1){
        cout << "Final board:" << endl;
        Board finalboard = hexToVector(initial_state4.board_hex);
        for (int i = 0; i < 16; ++i) {
            cout << finalboard[i] << (i % 4 == 3 ? "\n" : " ");
        }
    }

    } else {
        cout << "Unsolved using LINEAR CONFLICT heuristic." << endl;
    }

/////////////////////////////////////////////////////////////////////////////////////////
    if(print_path == 1){
        gowno = solve(initial_state1, manhattan_distance, visited_states, solution);
    }
    else{
        gowno = solve_clear(initial_state1, manhattan_distance, visited_states, solution);
    }
    if (gowno) {
    cout << "Solved using MANHATTAN DISTANCE heuristic:" << endl;
    cout << "Number of steps: " << initial_state1.g << endl;
    cout << "Visited states: " << visited_states << endl;
    if(print_path != -1){
    cout << "Solution" << endl;
    Board newboard;
    newboard = hexToVector(init.board_hex);
    cout << "Initial board:" << endl;
        for (int i = 0; i < 16; ++i) {
            cout << newboard[i] << (i % 4 == 3 ? "\n" : " ");
        }
        cout<<endl;
    }
    if(print_path == 1){
        for(const auto &state : solution){
            for (int i = 0; i < 16; ++i) {
                cout << state[i] << (i % 4 == 3 ? "\n" : " ");
            }        
            cout<<endl;
        }
    }
    else if(print_path != -1){
        cout << "Final board:" << endl;
        Board finalboard = hexToVector(initial_state1.board_hex);
        for (int i = 0; i < 16; ++i) {
            cout << finalboard[i] << (i % 4 == 3 ? "\n" : " ");
        }
    }

    } else {
        cout << "Unsolved using MANHATTAN DISTANCE heuristic." << endl;
    }




    return 0;
}