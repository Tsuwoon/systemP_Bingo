#include <iostream>
#include <cstdlib>
#include <time.h>
#include <iomanip>
using namespace std;

class Rnd
{
private:
	int start;
	int end;

public:
	Rnd() // 생성될때 1~25까지 범위 초기화
	{
		start = 1;
		end = 25;
	}
	int getRNumber() // 난수 생성
	{
		int rn = (int)((rand() % (end - start + 1)) + start);
		return rn;
	}
};

class Bingo
{
private:
	int bingoNum[5][5]; // 임의로 생성된 숫자를 저장하는 배열
	int nStatus[5][5]; // bingoNum[5][5]의 숫자가 불렸는지를 체크하는 배열
	int num[25]; // 1배열에 25개 난수를 저장한다.
	int WBingoCount[5]; // 가로줄 빙고 확인
	int HBingoCount[5]; // 세로줄 빙고 확인
	int RBingoCount; // 오른쪽 대각선 확인
	int LBingoCount; // 왼쪽 대각선 확인

public:
	Bingo() // 이 생성자에서는 bingoNum[5][5] 및 그 이외의 변수를 초기화한다.
	{
		Rnd R;
		int i, j;
		int x;

		for (i = 0; i < 25; i++) // 1차 배열에 중복없는 난수를 저장한다.
		{
			x = R.getRNumber();
			num[i] = x;
			for (j = 0; j < i; j++) {
				if (num[j] == x) {
					i--;
					break;
				}
			}
		}
		for (i = 0; i < 5; i++) { // 1차배열을 2차 배열로 만들기
			for (j = 0; j < 5; j++) {
				bingoNum[i][j] = num[(i * 5) + j];
				nStatus[i][j] = bingoNum[i][j];
			}
		}
		// -------------------------------초기화
		for (i = 0; i < 5; i++) { // 배열 초기화
			WBingoCount[i] = 0;
			HBingoCount[i] = 0;
		}
		RBingoCount = 0;
		LBingoCount = 0;

	}

	int checkBingo() // 이 함수는 nStatus[5][5]를 이용하여 빙고를 부를 수 있는지 검사한다.
	   // 빙고이면 return 1, 아니면 return 0
	{
		int i, j;
		int Wcount = 0;
		int Hcount = 0;
		int RCcount = 0;
		int LCoount = 0;
		int bingo = 0;

		for (i = 0; i < 5; i++) { // 가로,세로줄 빙고 체크
			Wcount = 0;
			Hcount = 0;
			for (j = 0; j < 5; j++) {
				if (nStatus[i][j] == 0) // 가로 체크
				{
					Wcount++;
				}
				if (nStatus[j][i] == 0) // 세로 체크
				{
					Hcount++;
				}
				if (Wcount == 5)
				{
					bingo++;
				}
				if (Hcount == 5)
				{
					bingo++;
				}
			}
		}

		for (j = 5, i = 0; j >= 0, i < 5; j--, i++) {
			if (nStatus[j][j] == 0) // 왼쪽 대각선 체크
			{
				LCoount++;
			}
			if (nStatus[i][j] == 0) // 오른쪽 대각선 체크
			{
				RCcount++;
			}
			if (LCoount == 5 || RCcount == 5)
			{
				bingo++;
			}
		}
		if (bingo >= 3)
			return 1; // 빙고
		else
			return 0; // 게임 진행중

	}

	void printBingoGrid() // 이 함수는 nStatus[5][5]를 출력한다.
	{
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				cout << setw(3) << nStatus[i][j];
			}
			cout << "\n";
		}
		cout << "\n";
	}

	int callNumber(int number) {

		/* 이 함수는 int number를 입력으로 받아 들인다.
		   그리고, bingoNum[5][5]의 숫자가 호출됐는지 검사하고
		   nStatus[5][5]에 기록한다.
		   int number가 이상이 없으면 return 1
			  이상이 있으면 return 0
				 (1~25 사이의 숫자가 아닌 경우, 문자가 입력된 경우) */

		int i, j;

		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++) {
				if (nStatus[i][j] == 0 && bingoNum[i][j] == number) { // 중복 체크
					cout << "오류!! 중복됩니다" << endl;
					return 0;
				}
				if (nStatus[i][j] == number) { // 문제 없음
					nStatus[i][j] = 0;
					return 1;
				}
			}
		}
		// 위 if에 안걸리는 나머지 경우는 숫자가 아닌 경우 뿐
		cout << "오류!! 1~25 중에 숫자를 넣으세요" << endl;
		return 0;
	}

	int AI() // 컴퓨터의 [5][5]배열을 통해서 가로, 세로, 대각선의
	   // 0개수를 파악하여 0이 가장 많은 줄에 0을 넣게하는 알고리즘.
	{
		int i, j;
		int zerocount[4] = { 0 }; // 가장 큰 가로 세로 외쪽 오른쪽 배열에 넣기
		int Wzerocount[5] = { 0 }; // 가로줄 0 개수 저장
		int Hzerocount[5] = { 0 }; // 가로줄 0 개수 저장
		int Wbigline = 0; // 가장 0이 많은 줄 저장
		int Hbigline = 0; // 가장 0이 많은 줄 저장
		int temp = 0;
		int bignum = -1; // 가로 세로 대각선중 가장 큰 쪽을 저장
		int result;
		int Wbigcount = 0; // 가로줄 중 0이 가장 많은 줄의 0갯수 저장
		int Hbigcount = 0; // 세로줄 중 0이 가장 많은 줄의 0갯수 저장
		int Rbigcount = 0; // 오른쪽 0 개수 저장
		int Lbigcount = 0; // 왼쪽 0 개수 저장
		int centercount = 0; // 중앙에 {11,12,13 21,22,23 31,32,33} 배열 0 숫자 카운트

		if (nStatus[2][2] == 0) {


			for (i = 1; i < 4; i++) // 중앙에 {11,12,13 21,22,23 31,32,33} 배열 0 숫자 카운트
			{
				for (j = 1; j < 4; j++) {
					if (nStatus[i][j] == 0) {
						centercount++;
					}
				}
			}
			if (centercount < 2) {
				for (i = 1; i < 4; i++) // 중앙 {111,12,13 21,22,23 31,32,33} 배열에 숫자를 넣는다
				{
					for (j = 1; j < 4; j++) {
						if (nStatus[i][j] != 0) {
							result = nStatus[i][j];
							nStatus[i][j] = 0;
							return result;
						}
					}
				}
			}

			for (i = 0; i < 5; i++)//가로 세로 0 체크
			{
				for (j = 0; j < 5; j++) {
					if (nStatus[i][j] == 0 && WBingoCount[i] == 0) {
						Wzerocount[i]++;
						if (Wzerocount[i] == 5)
						{
							WBingoCount[i]++; // 만약 가로줄 중에 빙고가 된다면 다시 못들어오게한다.
							Wzerocount[i] = 0; // 그리고 가로줄 라인은 갯수를 0으로 만든다.
						}
					}
				}
				if (nStatus[j][i] == 0 && HBingoCount[i] == 0) {
					Hzerocount[i]++;
					if (Hzerocount[i] == 5) {
						HBingoCount[i]++; // 만약 세로줄 줄에 빙고가 된다면 다시 못들어오게한다.
						Hzerocount[i] = 0; // 그리고 세로줄라인은 갯수를 0으로 만든다.
					}
				}
			}

			Wbigcount = Wzerocount[0]; // 처음 가로줄을 가장큰수로 잡는다.
			Hbigcount = Hzerocount[0]; // 처음 세로줄을 가장큰수로 잡는다.

			for (i = 0; i < 5; i++) // 가장 많이 0이된 줄을 찾는다.
			{
				if (Wbigcount < Wzerocount[i]) {
					Wbigcount = Wzerocount[i];
					Wbigline = i;
				}
				if (Hbigcount < Hzerocount[i]) {
					Hbigcount = Hzerocount[i];
					Hbigline = i;
				}
			}

			zerocount[0] = Wbigcount; // 가로 세로 대각선을 비교하기 위해서
			zerocount[1] = Hbigcount; // 가로 세로를 저장한다.

			/*---------------------------여기서 부터 대각선--------*/

			for (i = 4, j = 0; i >= 0, j < 5; i--, j++) {
				if (nStatus[j][j] == 0 && LBingoCount == 0) // 왼쪽 대각선
				{
					Lbigcount++;
					if (Lbigcount == 5) {
						LBingoCount++;
						Lbigcount = 0;
					}
				}
				if (nStatus[i][j] == 0 && RBingoCount == 0) // 오른쪽 대각선
				{
					Rbigcount++;
					if (Rbigcount == 5)
					{
						RBingoCount++;
						Rbigcount = 0;
					}
				}
			}

			zerocount[2] = Lbigcount; // 왼쪽 대각선의 0개 개수를 저장한다.
			zerocount[3] = Rbigcount; // 오른쪽 대각선의 0개 개수를 저장한다.

			//---------------------------------------------------------------
			//zerocount[0]는 가로줄의 0개수
			//zerocount[1]는 세로줄의 0개수
			//zerocount[2]는 왼쪽 대각선의 0개수
			//zerocount[3]는 오른쪽 대각선의 0개수
			temp = zerocount[0];
			for (i = 0; i < 4; i++) {
				if (zerocount[i] >= temp) {
					temp = zerocount[i]; // 가로 세로 왼쪽 오른쪽 중에 가장 0이 많은곳을 찾는다.
					bignum = i; // 이때 가장 큰곳을 bignum에 저장한다.
				}
			}

			if (bignum == 0) // 가로에 0이 없는 곳에 넣는다.
			{
				for (j = 0; j < 5; j++) {
					if (nStatus[Wbigline][j] != 0) {
						result = nStatus[Wbigline][j];
						nStatus[Wbigline][j] = 0;
						return result;
					}
				}
			}
			else if (bignum == 1) // 세로에 0이 없는 곳에 넣는다.
			{
				for (i = 0; i < 5; i++) {
					if (nStatus[i][Hbigline] != 0) {
						result = nStatus[i][Hbigline];
						nStatus[i][Hbigline] = 0;
						return result;
					}
				}
			}
			else if (bignum == 2) // 왼쪽 대각선에 0이 없는 곳에 넣는다.
			{
				for (i = 0; i < 5; i++) {
					if (nStatus[i][i] != 0) {
						result = nStatus[i][i];
						nStatus[i][i] = 0;
						return result;
					}
				}
			}
			else if (bignum == 3) // 오른쪽 대각선에 0이 없는곳에 넣는다.
			{
				for (i = 0, j = 4; i < 5, j >= 0; i++, j--) {
					if (nStatus[i][j] != 0) {
						result = nStatus[i][j];
						nStatus[i][j] = 0;
						return result;
					}
				}
			}

		}
		else
		{
			result = nStatus[2][2];
			nStatus[2][2] = 0; // 첫번째로 nStatus[2][2]에 0이 없으면 0을 넣는다.
			return result;
		}

	}
};


//테마 선택 함수
int Select_Thema() {
	int thema = 0;
	while (1) {
		cout << "테마를 선택하세요 (1. 음식 2. 동물 3. 숫자 4. 나라이름) : ";
		cin >> thema;
		if (thema >= 1 && thema <= 4)
			break;
		else
			cout << "1에서 4의 숫자를 입력해주세요." << endl;

	}
	switch (thema) {
	case 1:
		//r_food(); //<읽는 것은 이 안에서 해도 되고 따로 함수로 빼도 될 듯
		break;
	case 2:
		//r_animal();
		break;
	case 3:
		//r_number();
		break;
	case 4:
		//r_country();
		break;
	}
	
	return thema;
}
//실제 게임 진행 함수
void vscomputer() {
	srand((unsigned)time(NULL));

	Bingo user;
	Bingo com;

	int x;
	int num;
	int a;
	int y;

	while (1) {

		fflush(stdin); // 버퍼를 비워준다.
		user.printBingoGrid();
		com.printBingoGrid();

		cout << "숫자를 선택하세요 : ";
		x = scanf("%d", &num); // scanf는 지형 형식자 이외에 모두 입력 거부되어 0또는 1이 들어간다.

		cout << "---------------------------------" << endl;
		if (x == 1) // x가 1일때에는 정수를 입력했으므로 다시 정수를 입력시킨다.
		{
			a = num;
		}
		if (x == 0) // x가 문자나 0일 때는 초과된 26을 넣는다.
		{
			a = 26;
		}
		if (user.callNumber(a) == 0) {
			continue;
		}

		//---------------------------------컴퓨터--------------------------------
		com.callNumber(a);
		if (user.checkBingo() == 1) // 사용자가 먼저 빙고를 부르면 승리!!를 체크
		{
			cout << "-------------------------------" << endl;
			cout << "빙고!! 승리하셨습니다!" << endl;
			user.printBingoGrid();
			cout << "-------------------------------" << endl;
			break;
		}
		if (com.checkBingo() == 1)
		{
			cout << "-------------------------------" << endl;
			cout << "빙고!! 패배하셨습니다!" << endl;
			com.printBingoGrid();
			cout << "-------------------------------" << endl;
			break;
		}

		y = com.AI(); // 컴퓨터 실행;
		cout << "컴퓨터가 부른 수는? : " << y << endl;
		user.callNumber(y);

		if (com.checkBingo() == 1) // 컴퓨터가 먼저 빙고를 부르면 승리!!를 체크
		{
			cout << "-------------------------------" << endl;
			cout << "빙고!! 승리하셨습니다!" << endl;
			com.printBingoGrid();
			cout << "-------------------------------" << endl;
			break;
		}
		if (user.checkBingo() == 1)
		{
			cout << "-------------------------------" << endl;
			cout << "빙고!! 패배하셨습니다!" << endl;
			user.printBingoGrid();
			cout << "-------------------------------" << endl;
			break;
		}


	}
}

//1번 메뉴(게임시작) 미완성
void num1() {
	int thema=Select_Thema();
	
	//Make_Map();//<<현재 Bingo에 내장되어있음
	vscomputer();

}

//단어 추가(write) 미완성
void Add_word() {

}
//2번 메뉴(보기추가)
void num2(){
	int thema = Select_Thema();
	Add_word();
}

//메뉴 선택
void Game_Start() {
	cout << "메뉴를 선택하세요. " << endl << endl;
	cout << "1. 게임 시작" << endl;
	cout << "2. 보기 추가 " << endl;
	cout << "3. 게임 종료 " << endl;
	cout << endl;
	int menu = 0;
	int i = 0;
	while (menu < 1 || menu >3) {
		if (i > 0) {
			cout << "1~3을 입력해주세요." << endl;
		}
		cin >> menu;
		i++;
		cin.clear();
		while (cin.get() != '\n');
	}
	cout << endl;
	switch (menu) {
	case 1:
		//cout << 1<< endl; fork
		num1();
		break;
	case 2:
		//cout << 2 << endl; fork
		num2();
		break;
	case 3:
		//cout << 3 << endl; fork
		exit(0);
		break;
	}

}

int main()
{
	Game_Start();
}
