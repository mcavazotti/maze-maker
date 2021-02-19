#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

class coordinate {
 public:
  coordinate() : coord{0, 0} {};
  coordinate(int x, int y, int px, int py) : coord{x, y}, parent{px, py} {};
  int coord[2];
  int parent[2];

  int x() const { return coord[0]; }
  int y() const { return coord[1]; }
  int parentX() const { return parent[0]; }
  int parentY() const { return parent[1]; }
};

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    std::cerr << "Missing arguments.\nUsage: " << argv[0] << " <w> <h>\nOutput is a PPM file\n";
    exit(1);
  }
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);

  int *maze = (int *)calloc(width * height, sizeof(int));
  if (maze == NULL) {
    std::cerr << "Error allocating memory\n";
    exit(1);
  }

  std::vector<coordinate> nodeQueue;

  nodeQueue.push_back(coordinate(1, 1, 0, 0));
  srand(time(NULL));
  while (!nodeQueue.empty()) {
    int index = rand() % nodeQueue.size();
    auto item = nodeQueue[index];
    int x = item.x();
    int y = item.y();


    if (maze[y * width + x] != 0xffffff) {
      maze[y * width + x] = 0xffffff;

      if (item.parentX() != 0 && item.parentY() != 0) {
        int deltaX = 0;
        int deltaY = 0;
        if (item.parentX() != x) deltaX = item.parentX() > x ? 1 : -1;
        if (item.parentY() != y) deltaY = item.parentY() > y ? 1 : -1;
        maze[(y + deltaY) * width + (x + deltaX)] = 0xffffff;
      }

      if (y - 2 > 0 && maze[(y - 2) * width + x] == 0) {
        nodeQueue.push_back(coordinate(x, y - 2, x, y));
      }
      if (y + 2 < height - 1 && maze[(y + 2) * width + x] == 0) {
        nodeQueue.push_back(coordinate(x, y + 2, x, y));
      }
      if (x - 2 > 0 && maze[y * width + x - 2] == 0) {
        nodeQueue.push_back(coordinate(x - 2, y, x, y));
      }
      if (x + 2 < width - 1 && maze[y * width + x + 2] == 0) {
        nodeQueue.push_back(coordinate(x + 2, y, x, y));
      }
    }
    nodeQueue.erase(nodeQueue.begin() + index);

  }

  // entry point
  maze[1] = 0x00ff00;
  // exit point
  maze[width * (height - ((height - 1) % 2)) - 2 - ((width - 1) % 2)] =
      0xff0000;

  std::cout << "P3\n";
  std::cout << width << " " << height << "\n";
  std::cout << "255\n";

  for (int i = 0; i < height * width; i++) {
    int r = (maze[i] >> 16) & 0xff;
    int g = (maze[i] >> 8) & 0xff;
    int b = maze[i] & 0xff;
    std::cout << r << " " << g << " " << b << "\n";
  }

  free(maze);
  return 0;
}
