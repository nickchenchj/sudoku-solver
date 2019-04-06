#include "sudoku.h"
using namespace std;

// set
Sudoku::Sudoku() {
    for(int i = 0; i < sudokuSize; i++)
        map[i] = 0;
}

void Sudoku::setMap(const int set_map[]) {
    for(int i = 0; i < sudokuSize; i++)
        map[i] = set_map[i];
}

void Sudoku::setMap2d(const int set_map[]) {

    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++) {
            map2d[i][j] = set_map[i*9 + j];
            total_cand[i][j] = 9;
            for(int k = 0; k < 9; k++)
                cand[i][j][k] = 1;
        }
}

// show map
void Sudoku::showMap() {
    for(int i = 0; i < sudokuSize; i++) {
        if(i % 9 == 8)
            cout << map[i] << endl;
        else
            cout << map[i] << " ";
    }
}

void Sudoku::showMap2d() {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            cout << map2d[i][j];
            if(j < 8) cout << " ";
        }
        cout << endl;
    }
}

// transform
void Sudoku::swapNum(int x, int y) {
    for(int i = 0; i < sudokuSize; i++) {
        if(map[i] == x)
            map[i] = y;
        else if(map[i] == y)
            map[i] = x;
    }
}

void Sudoku::swapRow(int x, int y) {
    int temp[sudokuSize/3], init_xpos, init_ypos;

    // prevent large number
    x %= 3;
    y %= 3;

    // find position in map
    init_xpos = x*sudokuSize/3;
    init_ypos = y*sudokuSize/3;

    // store in temporary map
    for(int i = 0; i < sudokuSize/3; i++)
        temp[i] = map[i + init_xpos];

    // store y in x
    for(int i = 0; i < sudokuSize/3; i++)
        map[i + init_xpos] = map[i + init_ypos];

    // store temp in y
    for(int i = 0; i < sudokuSize/3; i++)
        map[i + init_ypos] = temp[i];
}

void Sudoku::swapCol(int x, int y) {
    int temp[sudokuSize/3], init_xpos, init_ypos;

    // prevent large number
    x %= 3;
    y %= 3;

    // find position in map
    init_xpos = x*3;
    init_ypos = y*3;

    // store column x in temp
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 3; j++)
            temp[j + i*3] = map[init_xpos + j + i*9];
    }

    // store y in x
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 3; j++)
            map[init_xpos + j + i*9] = map[init_ypos + j + i*9];
    }

    // store temp in y
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 3; j++)
            map[init_ypos + j + i*9] = temp[j + i*3];
    }
}

void Sudoku::rotate(int x) {
    int temp[9][9];

    // prevent large number
    x %= 4;

    // store 1D map into 2D temp
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            map2d[i][j] = map[i*9 + j];

    // rotate
    switch(x) {
    case 0: // rotate 0 degree(do nothing)
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                temp[i][j] = map2d[i][j];
        break;

    case 1: // rotate 90 degree
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                temp[i][j] = map2d[8-j][i];
        break;

    case 2: // rotate 180 degree
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                temp[i][j] = map2d[8-i][8-j];
        break;

    case 3: // rotate 270 degree
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                temp[i][j] = map2d[j][8-i];
        break;

    default:
        break;
    }

    for(int i = 0; i < 9; i++)
        for(int j = 0 ; j < 9; j++)
            map[i*9 + j] = temp[i][j];
}

void Sudoku::flip(int x) {
    int temp[9][9];

    // prevent large number
    x %= 2;

    // store map into 2D map & temporary map
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            map2d[i][j] = map[i*9 + j];

    switch(x) {
    case 0: // up-down flip
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                temp[i][j] = map2d[8-i][j];
        break;

    case 1: // left-right flip
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                temp[i][j] = map2d[i][8-j];
        break;

    default:
        cout << "ERROR: Wrong Command\n"
             << "0: up-down flip; 1: left-right flip"
             << endl;
        exit(1);
        break;
    }

    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            map[i*9 + j] = temp[i][j];
}

// solve
bool Sudoku::checkUnity(const int (&checkMap)[9]) {
    int count[10];

    // initialize
    for(int i = 0; i < 10; i++)
        count[i] = 0;

    // count
    for(int i = 0; i < 9; i++)
        count[checkMap[i]]++;

    // check
    for(int i = 1; i < 10; i++) {
        if(count[i] == 2)
            return false;
    }

    return true;
}

bool Sudoku::isCorrect() {
    int checkMap[9];
    bool check_result;

    // check row
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            checkMap[j] = map2d[i][j];
        }

        check_result = checkUnity(checkMap);
        if(check_result == false)
            return false;
    }

    // check column
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            checkMap[j] = map2d[j][i];
        }

        check_result = checkUnity(checkMap);
        if(check_result == false)
            return false;
    }

    // check cell
    int count;
    for(int i = 0; i < 7; i += 3) {
        for(int j = 0; j < 7; j += 3) {
            count = 0;
            for(int k = 0; k < 3; k++) {
                for(int w = 0; w < 3; w++)
                    checkMap[count++] = map2d[i+k][j+w];
            }

            check_result = checkUnity(checkMap);
            if(check_result == false)
                return false;
        }
    }

    return true;
}

int Sudoku::solve() {
    bool unchanged, result;

    // first check
    if(isCorrect() == false)
        return 0;

    // count element
    total_element = 0;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(map2d[i][j]) {
                total_element++;

                // update cand & total num of cand
                total_cand[i][j] = 1;
                for(int k = 0; k < 9; k++)
                    cand[i][j][k] = 0;
                cand[i][j][map2d[i][j]-1] = 1;
            }
        }
    }

    // multiple solutions
    if(total_element < 17)
        return 2;

    /*solve*/
    // delete candidates

    //test
    //int count = 0;

    do {
        unchanged = killCand();
        // check solution
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 9; j++) {
                // zero candidate: zero solution
                if(total_cand[i][j] == 0) {
                    return 0;
                }

                // only one candidate in a space
                if(map2d[i][j] == 0 && total_cand[i][j] == 1) {
                    update(i, j);

                    if(total_element == sudokuSize)
                        return 1;
                }
            }
        }

        /*
        // test
        cout << "count = " << count++ << endl;
        showMap2d();
        cout << endl;
        */

    } while(unchanged == false);

    // store map2d
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            tempMap[i][j] = map2d[i][j];

    result = recursive_solve(0, 0);

    if(result == false) return 0;
    else {
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                map2d[i][j] = tempMap[i][j];

        return 1;
        //recursive_solve_reverse(map2d, 0, 0);
    }
}

// delete candidates in each space
bool Sudoku::killCand() {
    bool unchanged = true;

    // delete candidates in cells & update unchanged
    if(delCellCand() == false)
        unchanged = false;

    // delete candidates in columns & update unchanged
    if(delColCand() == false)
        unchanged = false;

    // delete candidates in rows & update unchanged
    if(delRowCand() == false)
        unchanged = false;

    return unchanged;
}

bool Sudoku::delCand(int i, int j, int (&checkMap)[9]) {
    bool unchanged = true;

    for(int k = 0; k < 9; k++) {
        if(checkMap[k] != 0 && cand[i][j][checkMap[k]-1] == 1) {
            unchanged = false;
            cand[i][j][checkMap[k]-1] = 0;
            total_cand[i][j]--;
        }
    }
    return unchanged;
}

bool Sudoku::delRowCand() {
    int checkMap[9];
    int unchanged = true;

    // delete candidates of spaces in rows
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(map2d[i][j] == 0) {
                for(int k = 0; k < 9; k++)
                    checkMap[k] = map2d[i][k];

                // delete candidates & update unchanged
                if(delCand(i, j, checkMap) == false)
                    unchanged = false;
            }
        }
    }
    return unchanged;
}

bool Sudoku::delColCand() {
    int checkMap[9];
    int unchanged = true;

    // delete candidates of spaces in columns
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(map2d[i][j] == 0) {
                for(int k = 0; k < 9; k++)
                    checkMap[k] = map2d[k][j];

                // delete candidates & update unchanged
                if(delCand(i, j, checkMap) == false)
                    unchanged = false;
            }
        }
    }
    return unchanged;
}

bool Sudoku::delCellCand() {
    int checkMap[9];
    int unchanged = true;
    int count, xi, yi;

    // delete candidates of spaces in cells
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(map2d[i][j] == 0) {
                // store a cell in checkMap
                count = 0;
                xi = i/3*3;
                yi = j/3*3;
                for(int x = xi; x < xi + 3; x++) {
                    for(int y = yi; y < yi + 3; y++) {
                        checkMap[count++] = map2d[x][y];
                    }
                }

                // delete candidates & update unchanged
                if(delCand(i, j, checkMap) == false)
                    unchanged = false;
            }
        }
    }
    return unchanged;
}

void Sudoku::update(int i, int j) {
    for(int k = 0; k < 9; k++) {
        if(cand[i][j][k]) {
            map2d[i][j] = k + 1;
            total_element++;
            break;
        }
    }
}

bool Sudoku::recursive_solve(int i, int j) {
    // out of range
    if(i == 9) return true;

    // find next space
    while(total_cand[i][j] == 1) {
        // end of a row
        if(j == 8) {
            i++;
            j = 0;
            if(i == 9) return true;
        } else
            j++;
    }

    for(int k = 0; k < 9; k++) {
        if(cand[i][j][k]) {
            // test
            // cout << "map2d[" << i << "][" << j << "] = " << k+1 << endl;

            // put in candidates
            map2d[i][j] = k + 1;

            // test
            //showMap2d();
            //cout << endl;

            if(isCorrect()) {
                // test
                //cout << "CORRECT" << endl;

                // done: store results in tempMap
                if(checkDone()) {
                    // test
                    //cout << "DONE" << endl;

                    for(int x = 0; x < 9; x++)
                        for(int y = 0; y < 9; y++)
                            tempMap[x][y] = map2d[x][y];
                    return true;
                } else if(j == 8) {
                    if(recursive_solve(i+1, 0))
                        return true;
                } else if(recursive_solve(i, j+1))
                    return true;
            }
        }
    }
    map2d[i][j] = 0;

    // test
    // cout << "return false" << endl;

    return false;
}

bool Sudoku::checkDone() {
    int count = 0;

    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(map2d[i][j])
                count++;

    if(count == sudokuSize)
        return true;
    else
        return false;
}


