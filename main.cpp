#include<stdio.h>
#include<graphics.h>//easyx图形库头文件
#include "tools.h"
#define WIN_WIDTH 1000
#define WIN_HEIGHT 600

enum { WAN_DOU, XIANG_RI_KUI,SHI_REN_HUA,ZHI_WU_COUNT };
IMAGE imgBg;//表示背景图片
IMAGE imBar;
IMAGE imCards[8];
IMAGE* imzhiwu[ZHI_WU_COUNT][20];
int curX, curY;//当前选中的植物，在移动过程中的位置
int curzhiwu;//0:没有选中，1:选择了第一种植物

struct zhiwu{
	int type;		//0:没有植物
	int frameIndex;	//序列帧的序号
};
struct zhiwu map[6][9];
bool fileExist(const char *name){
	FILE* fp = fopen(name, "r");
	if (fp) {
		fclose(fp);
	}
	return fp != NULL;
}
void gameInit()
{
	loadimage(&imgBg,"res/Map/map0.jpg");//加载背景图片，把字符集改成多字节字符集
	loadimage(&imBar, "res/bar4.png");//植物卡槽
	memset(imzhiwu, 0, sizeof(imzhiwu));
	memset(map, 0, sizeof(map));
	//初始化植物卡牌
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		loadimage(&imCards[i], name);

		for (int j = 0; j < ZHI_WU_COUNT; j++) {
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
			if (fileExist(name)) {
				imzhiwu[i][j] = new IMAGE;
				loadimage(imzhiwu[i][j], name);
			}
			else {
				break;
			}
			}
	}
	curzhiwu = 0;
	initgraph(WIN_WIDTH, WIN_HEIGHT,1);
}
void updateWindow()
{
	BeginBatchDraw();
	putimage(0, 0, &imgBg);//渲染图片，打印到界面上
	putimagePNG(250, 0, &imBar);//渲染图片，把卡槽打印到界面上
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		int x = 315 + i*60;
		int y = 0;
		putimagePNG(x, y, &imCards[i]);
	}
	//渲染植物
	if (curzhiwu > 0){
		IMAGE* img = imzhiwu[curzhiwu - 1][0];
		putimagePNG(curX-img->getwidth()/2, curY-img->getwidth()/2, img);
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {
				int x = 256 + j * 81;
				int y = 0 + i * 100-20;
				int zhiwutype = map[i][j].type - 1;
				int index = map[i][j].frameIndex;
				putimagePNG(x, y, imzhiwu[zhiwutype][index]);
			}
		}
	}

	EndBatchDraw();
}
void userClick() {
	ExMessage msg;
	static int status = 0;
	if (peekmessage(&msg)); {
		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x > 315 && msg.x < 315 + 60 * ZHI_WU_COUNT && msg.y < 96) {
				int index = (msg.x - 315) / 60;
				status = 1;
				curzhiwu = index + 1;
			}
		}
		else if (msg.message == WM_MOUSEMOVE && status ==1) {
			curX = msg.x;
			curY = msg.y;

		}
		else if (msg.message == WM_LBUTTONUP) {
			int row = (msg.y - 0) / 85;
			int col = (msg.x - 256) / 82.5;
			map[row][col].type = curzhiwu;

			if (map[row][col].type == 0) {
				map[row][col].type = curzhiwu;
				map[row][col].frameIndex = 0;
			}
			printf("%d %d\n", row, col);
			curzhiwu = 0;
			status = 0;
		}
	}
}

int main(void)
{
	gameInit();
	while (1) {
		userClick();
		updateWindow();

	}
	system("pause");
	return 0;
}