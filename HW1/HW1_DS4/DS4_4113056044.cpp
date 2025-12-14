#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

#define SPACE 0
#define WALL 1
#define PERSON 2
#define CORE 3
#define EXIT 4

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct State {
    Position person;
    Position core;
    int steps;
} State;

typedef struct Move {
    int vert;
    int horiz;
} Move;

Move mOve[4] ={{.vert = -1, .horiz = 0}, {.vert = 0, .horiz = 1}, {.vert = 1, .horiz = 0}, {.vert = 0, .horiz = -1}}; 

bool can_person_reach(int **map, const int m, const int n, Position person, Position core, Position target);
int Solve(int **map, const int m, const int n, Position person, Position core, Position exit);
void deleteMap(int **map, int m, int n);
void deleteVisit(bool **map, int m, int n);

int main() {
    ifstream in;
    ofstream out;
    in.open("testcase4.txt");
    out.open("output4.txt");
    if(!in.is_open() || !out.is_open()) {
        cerr << "IO Exception!!!" << endl;
        return -1;
    }

    int numOftestcase = 0;
    in >> numOftestcase;

    for(int testcase = 0;testcase < numOftestcase;++testcase) {
        int m = 0, n = 0;
        in >> m >> n;
        int **map = new int*[m];
        for(int i = 0;i < m;++i) {
            map[i] = new int[n];
        }

        Position person = {.x = -1, .y = -1};
        Position core = {.x = -1, .y = -1};
        Position exit = {.x = -1, .y = -1};

        for(int i = 0;i < m;++i) {
            for(int j = 0;j < n;++j) {
                in >> map[i][j];
                if(map[i][j] == PERSON) {
                    person.x = i;
                    person.y = j;
                }
                if(map[i][j] == CORE) {
                    core.x = i;
                    core.y = j;
                }
                if(map[i][j] == EXIT) {
                    exit.x = i;
                    exit.y = j;
                }
            }
        }

        if(person.x == -1 || core.x == -1 || exit.x == -1) {
            out << -1 << endl;
            continue;
        }

        out << Solve(map, m ,n, person, core, exit) << endl;

        deleteMap(map, m, n);
    }

    return 0;
}

bool can_person_reach(int **map, const int m, const int n, Position person, Position core, Position target) {
    bool **visited = new bool*[m];
    for(int i = 0;i < m;++i) {
        visited[i] = new bool[n]{};
    }

    if(person.x == target.x && person.y == target.y) return true;

    queue<Position> q;
    q.push(person);
    visited[person.x][person.y] = true;

    while (!q.empty()){
        Position current = q.front();
        q.pop();
        for(int i = 0;i < 4;++i) {
            Position nextPos = {.x = current.x + mOve[i].vert, .y = current.y + mOve[i].horiz};
            if(nextPos.x < 0 || nextPos.x >= m || nextPos.y < 0 || nextPos.y >=n) continue;
            if(nextPos.x == core.x && nextPos.y == core.y) continue;
            if(map[nextPos.x][nextPos.y] == WALL) continue;
            if(nextPos.x == target.x && nextPos.y == target.y) {
                deleteVisit(visited, m, n);
                return true;
            }

            if(!visited[nextPos.x][nextPos.y]) {
                visited[nextPos.x][nextPos.y] = true;
                q.push(nextPos);
            }

        }
    }
    deleteVisit(visited, m, n);
    return false;
}

int Solve(int **map, const int m, const int n, Position person, Position core, Position exit) {
    bool **visited = new bool*[m], **visited_core = new bool*[m];
    for(int i = 0;i < m;++i) {
        visited[i] = new bool[n]{};
        visited_core[i] = new bool[n]{};
    }

    State init = {.person = person, .core = core, .steps = 0};
    queue<State> q;
    q.push(init);
    visited[person.x][person.y] = true;
    visited_core[core.x][core.y] = true;

    while(!q.empty()) {
        State current = q.front();
        q.pop();

        if(current.core.x == exit.x && current.core.y == exit.y) {
            deleteVisit(visited, m, n);
            deleteVisit(visited_core, m, n);
            return current.steps;
        }

        for(int i = 0;i < 4;++i) {
            Position nextCore = {.x = current.core.x + mOve[i].vert, .y = current.core.y + mOve[i].horiz};
            Position nextPerson = {.x = current.core.x, .y = current.core.y};
            Position requiredPerson = {.x = current.core.x - mOve[i].vert, .y = current.core.y - mOve[i].horiz};

            if(requiredPerson.x < 0 || requiredPerson.x >= m || requiredPerson.y < 0 || requiredPerson.y >= n) continue;
            if(nextCore.x < 0 || nextCore.x >= m || nextCore.y < 0 || nextCore.y >= n) continue;
            if(map[requiredPerson.x][requiredPerson.y] == WALL || map[nextCore.x][nextCore.y] == WALL) continue;

            if(!visited[nextPerson.x][nextPerson.y] || !visited_core[nextCore.x][nextCore.y]) {
                if(can_person_reach(map, m ,n, current.person, current.core, requiredPerson)) {
                    visited[nextPerson.x][nextPerson.y] = true;
                    visited_core[nextCore.x][nextCore.y] = true;
                    State nextState = {.person = nextPerson, .core = nextCore, .steps = current.steps + 1};
                    q.push(nextState);
                }
            }
        }

    }
    deleteVisit(visited, m, n);
    deleteVisit(visited_core, m, n);

    return -1;
}

void deleteMap(int **map, int m, int n) {
    for(int i = 0;i < m;++i) {
        delete[] map[i];
    }
    delete[] map;
}

void deleteVisit(bool **map, int m, int n) {
    for(int i = 0;i < m;++i) {
        delete[] map[i];
    }
    delete[] map;
}