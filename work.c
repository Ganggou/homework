#include <stdio.h>

struct Table {
  int *table;
  int teamCount;
  int studentCount;
};

struct SearchState {
  int *studentList;   // 记录学生疑似情况的列表
  int *p;             // 疑似排查名单队首
  int *q;             // 疑似排查名单队尾
};

void search(struct Table tableParams, struct SearchState searchState) {
  if (searchState.p == searchState.q) { // 排查队列为空
    return;
  }

  int *table = tableParams.table;
  int studentCount = tableParams.studentCount;
  int teamCount = tableParams.teamCount;
  int *studentList = searchState.studentList;
  int studentNum = *searchState.p; // 当前排查学生

  for(int i = 0; i < teamCount; i++) {
    if(*(table + i * studentCount + studentNum) == 1) {
      for(int j = 0; j < studentCount; j++) {
        if(*(table + i * studentCount + j)== 1 && studentList[j] != 1) {
          studentList[j] = 1; //标记j号学生疑似，避免重复排查
          *(searchState.q++) = j;  //将此学生压入排查队列
        }
      }
    }
  }
  searchState.p++;
  search(tableParams, searchState);
}

int main(void) {
  int studentCount, teamCount, initStudentNum;
  printf("请输入学生总数:");
  scanf("%d", &studentCount);
  printf("请输入团体总数:");
  scanf("%d", &teamCount);

  // 二维数组记录学生参加团体情况
  int table[teamCount][studentCount];
  for(int i = 0; i < teamCount; i++) {
    for (int j = 0; j < studentCount; j++) {
      table[i][j] = 0;
    }
  }
  for(int i = 0; i < teamCount; i++) {
    int j;
    printf("请输入第%d组学生编号:", i+1);
    do {
      scanf("%d", &j);
      if(j == -1) { // 输入-1表明团队没人
        continue;
      }
      table[i][j] = 1;
    } while(getchar() != '\n');
  }
  printf("请输入第一个疑似病例学生编号:");
  scanf("%d", &initStudentNum);

  int queue[studentCount], studentList[studentCount], *p, *q;
  for (int i = 0; i< studentCount; i++) {
    queue[i] = -1;
    studentList[i] = 0;
  }
  
  studentList[initStudentNum] = 1; // 记录学生为疑似，避免重复排查
  queue[0] = initStudentNum; // 第一名疑似学生进入排查队列
  p = queue; // p为队首
  q = p + 1; // q为队尾

  struct Table tableParams;
  tableParams.table = &table[0][0];
  tableParams.teamCount = teamCount;
  tableParams.studentCount = studentCount;
  struct SearchState searchState;
  searchState.studentList = studentList;
  searchState.p = p;
  searchState.q = q;

  search(tableParams, searchState);

  printf("疑似病例学生编号:");
  for (int i = 0; i< studentCount; i++) {
    if(queue[i] == -1) {
      break;
    }
    printf("%d ", queue[i]);
  }
  printf("\n");

  return 0;
}
