#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> map;
int mapSize, mapLastIndex;

class Location {
public:
	int raw = -1;
	int col = -1;
	Location() {};
	Location(int r, int c) {
		this->raw = r;
		this->col = c;
	}

	Location operator+(const Location& loc) {
		return Location(this->raw + loc.raw, this->col + loc.col);
	}

};

Location upLoc = Location(-1, 0);
Location leftLoc = Location(0, -1);
Location rightLoc = Location(0, 1);
Location downLoc = Location(1, 0);

class Node {
	Location loc;
	int depth;

public:
	Node() {
		loc = Location(-1, -1);
		depth = 0;
	};
	Node(Location loc) {
		this->loc = loc;
		depth = 0;
	}
	Node(Location loc, int d) {
		this->loc = loc;
		this->depth = d;
	}

	Location getLocation() {
		return this->loc;
	}

	int getDepth() {
		return this->depth;
	}

	bool operator<(const Node n) const {
		if (this->depth == n.depth) {
			if (this->loc.raw == n.loc.raw) return this->loc.col > n.loc.col;
			return this->loc.raw > n.loc.raw;
		}

		return this->depth > n.depth;

		return false;
	}
};

class Shark {
	int size;
	int saturation;
	int time;
	Location loc;
public:
	Shark() {
		size = 2;
		saturation = 0;
		time = 0;
	}

	void getPrey() {
		saturation++;
		if (saturation == size) {
			size++;
			saturation = 0;
		}
	}

	const int getSize() {
		return this->size;
	}

	void addTime(int t) {
		this->time += t;
	}

	const int getTime() {
		return this->time;
	}

	void setLocation(Location l) {
		this->loc = l;
	}

	void swapLocation(Location l) {
		map[this->loc.raw][this->loc.col] = 0;
		this->loc = l;
		map[this->loc.raw][this->loc.col] = 9;
	}

	Location getLocation() {
		return this->loc;
	}
};

class BFS {
	Shark* s;
	int lastTime = 0;
public:
	BFS(Shark* s) {
		this->s = s;
		mapLastIndex = mapSize - 1;
		int fishSize;

		map = vector<vector<int>>(mapSize, vector<int>(mapSize, 0));

		for (int i = 0; i < mapSize; i++) {
			for (int j = 0; j < mapSize; j++) {
				cin >> fishSize;

				if (fishSize == 9) {
					s->setLocation(Location(i, j));
				}

				map[i][j] = fishSize;
			}
		}
	}

	bool isFind() {
		vector<vector<bool>> visited(mapSize, vector<bool>(mapSize, false));
		priority_queue<Node> que;
		int sharkSize = s->getSize();
		que.push(Node(s->getLocation()));

		do {
			Node currNode = que.top();
			Location currLoc = currNode.getLocation();
			int depth = currNode.getDepth();
			que.pop();

			if (visited[currLoc.raw][currLoc.col]) continue;
			else visited[currLoc.raw][currLoc.col] = true;
			//find
			if (isPrey(sharkSize, currLoc)) {
				s->getPrey();
				s->swapLocation(currLoc);
				s->addTime(depth);
				return true;
			}


			//not find
			if (isPass(currLoc + upLoc, sharkSize, visited)) {
				Location tempLocation = currLoc + upLoc;
				que.push(Node(tempLocation, depth + 1));
			}
			if (isPass(currLoc + leftLoc, sharkSize, visited)) {
				Location tempLocation = currLoc + leftLoc;
				que.push(Node(tempLocation, depth + 1));
			}
			if (isPass(currLoc + rightLoc, sharkSize, visited)) {
				Location tempLocation = currLoc + rightLoc;
				que.push(Node(tempLocation, depth + 1));
			}
			if (isPass(currLoc + downLoc, sharkSize, visited)) {
				Location tempLocation = currLoc + downLoc;
				que.push(Node(tempLocation, depth + 1));
			}


		} while (!que.empty());

		return false;
	}

	bool isPass(Location loc, int currSharkSize, vector<vector<bool>>& visitedMat) {
		int raw = loc.raw;
		int col = loc.col;

		if (raw > mapLastIndex || raw < 0) {
			return false;
		}
		if (col > mapLastIndex || col < 0) {
			return false;
		}

		if (currSharkSize < map[raw][col]) {
			return false;
		}

		if (visitedMat[raw][col]) {
			return false;
		}

		return true;
	}

	bool isPrey(int size, Location loc) {
		if (size > map[loc.raw][loc.col] && map[loc.raw][loc.col] != 0) {
			return true;
		}
		else {
			return false;
		}
	}
};




int main() {
	cin >> mapSize;
	mapLastIndex = mapSize - 1;

	Shark* shark = new Shark();
	BFS B(shark);
	while (B.isFind()) {
	}

	cout << shark->getTime();
}