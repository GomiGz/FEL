#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
using namespace std;

typedef struct{
    int value;
    bool visited;
}Square;

typedef struct{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
}Robot;

typedef struct{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
}Path;

static int result;

void preCountWalks(Robot robot,vector<int> *array,Square** field){
    int start_x = robot.start_x;
    int start_y = robot.start_y;
    int end_x = robot.end_x;
    int end_y = robot.end_y;

    if(start_x == end_x && start_y == end_y){
        array->push_back(field[start_y][start_x].value);
        return;
    }
    int val = 0;
    if(start_x == end_x){
        if(start_y > end_y){
            for (int i = start_y; i >= end_y; i--){
                val += field[i][start_x].value;
                array->push_back(val);
            }
        }else{
            for(int i = start_y; i <= end_y; i++){
                val += field[i][start_x].value;
                array->push_back(val);
            }
        }
    }else{
        if(start_x > end_x){
            for(int i = start_x; i >= end_x; i--){
                val += field[start_y][i].value;
                array->push_back(val);
            }
        }else{
            for(int i = start_x; i <= end_x; i++){
                val += field[start_y][i].value;
                array->push_back(val);
            }
        }
    }
}

bool isBetween(int a, int b, int c){
    if(a <= c && c <= b){
        return true;
    }
    if (b <= c && c <= a){
        return true;
    }
    return false;
}
int pathsCross(Robot path1, Path path2){
    if((path1.start_x == path1.end_x && path2.start_x == path2.end_x) || (path1.start_y == path1.end_y && path2.start_y == path2.end_y)){
        if(isBetween(path2.start_x, path2.end_x, path1.start_x) && path1.start_y == path2.start_y){
            return 987654;
        }else if(isBetween(path2.start_x, path2.end_x, path1.end_x ) && path1.start_y == path2.start_y){
            return fmin(abs(path1.start_x - path2.start_x),abs(path1.start_x - path2.end_x));
        }else if (isBetween(path2.start_y, path2.end_y, path1.start_y)&& path1.start_x == path2.start_x){   
            return 987654;
        }else if(isBetween(path2.start_y, path2.end_y, path1.end_y)&& path1.start_x == path2.start_x){
            return fmin(abs(path1.start_y - path2.start_y),abs(path1.start_y - path2.end_y));
        }
    }else{
        if(path1.start_x == path1.end_x){
            if(isBetween(path2.start_x,path2.end_x,path1.start_x) && isBetween(path1.start_y,path1.end_y,path2.start_y)){
                return path1.start_y - path2.start_y;
            }
        }else{
            if(isBetween(path2.start_y,path2.end_y,path1.start_y) && isBetween(path1.start_x,path1.end_x,path2.start_x)){
                return path1.start_x - path2.start_x;
            }
        }
    }
    if(path1.start_x == path1.end_x){
        return abs(path1.start_y - path1.end_y) + 1;
    }else{
        return abs(path1.start_x - path1.end_x) + 1;
    }
}

void updatePath(Path* path,Robot robot,int val){
    if(robot.start_x == robot.end_x){
        if(robot.start_y > robot.end_y){
            path->end_y = robot.start_y - val;
        }else{
            path->end_y = robot.start_y + val;
        }
        path->end_x = robot.end_x;
    }else{
        if(robot.start_x > robot.end_x){
            path->end_x = robot.start_x - val;
        }else{
            path->end_x = robot.start_x + val;
        }
        path->end_y = robot.end_y;
    }
    path->start_x = robot.start_x;
    path->start_y = robot.start_y;
}
void findValue(int num, Robot robots[], bool robotsPlaced[],vector<int>* preWalks,Path robotPaths[],int depth,int max_depth,int currentResult){
    if(depth == max_depth){
        if(currentResult > result){
            result = currentResult;
        }
        return; 
    }else{
        for(int i = 0; i < num;i++){
            if(!robotsPlaced[i]){
                int min = 2648548;
                int a = 0;
                for(int j = 0; j < num; j++){
                    if(robotsPlaced[j] && j != i){
                        a = abs(pathsCross(robots[i],robotPaths[j]));
                        if(min > a){
                            min = a;
                        }
                    }                
                }
                if(depth == 0){
                    if(robots[i].start_x == robots[i].end_x){
                        min = abs(robots[i].end_y - robots[i].start_y)+1;
                    }else{
                        min = abs(robots[i].end_x - robots[i].start_x)+1;
                    }
                }
                if (min == 0 && !(robots[i].start_x == robots[i].end_x && robots[i].start_y == robots[i].end_y )){
                    min = 1;
                }
                updatePath(&robotPaths[i],robots[i],min-1);
                robotsPlaced[i] = true;
                if((robots[i].start_x == robots[i].end_x && robots[i].start_y == robots[i].end_y) || a == 987654 ){
                    findValue(num,robots,robotsPlaced,preWalks,robotPaths,depth+1,max_depth,currentResult);
                }else{
                    findValue(num,robots,robotsPlaced,preWalks,robotPaths,depth+1,max_depth,currentResult + preWalks[i][min-1]);
                }
                robotsPlaced[i] = false;
            }
        }
    }

}

int main (int argc, const char * argv[]) {
    int height,width,num;
    cin >> height >> width;
    Square** field = new Square*[height];
    for(int i = 0; i < height;i++){
        field[i] = new Square[width];
    }
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width;j++){
            int a;
            cin >> a;
            field[i][j].value = a;
            field[i][j].visited = false;
        }
    }
    cin >> num;
    Robot robots[num];
    for (int i = 0; i < num; i++){
        int a,b,c,d;
        cin >> a >> b >> c >> d;
        robots[i].start_y = a;
        robots[i].start_x = b;
        robots[i].end_y = c;
        robots[i].end_x = d;
    }

    vector<int> preWalks[num];
    Path robotPaths[num];
    for(int i = 0; i < num; i++){
        preCountWalks(robots[i],&preWalks[i],field);
    }
    bool robotsPlaced[num];
    for(int i = 0; i < num; i++){
        robotsPlaced[i] = false;
    }
    findValue(num,field,robots,robotsPlaced,preWalks,robotPaths,0,num,0);
    cout << result << "\n";    
}