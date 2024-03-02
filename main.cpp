#include<stdio.h>
#include<graphics.h>//easyxͼ�ο�ͷ�ļ�
#include "tools.h"
#define WIN_WIDTH 1000
#define WIN_HEIGHT 600

enum { WAN_DOU, XIANG_RI_KUI,SHI_REN_HUA,ZHI_WU_COUNT };
IMAGE imgBg;//��ʾ����ͼƬ
IMAGE imBar;
IMAGE imCards[8];
IMAGE* imzhiwu[ZHI_WU_COUNT][20];
int curX, curY;//��ǰѡ�е�ֲ����ƶ������е�λ��
int curzhiwu;//0:û��ѡ�У�1:ѡ���˵�һ��ֲ��

struct zhiwu{
	int type;		//0:û��ֲ��
	int frameIndex;	//����֡�����
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
	loadimage(&imgBg,"res/Map/map0.jpg");//���ر���ͼƬ�����ַ����ĳɶ��ֽ��ַ���
	loadimage(&imBar, "res/bar4.png");//ֲ�￨��
	memset(imzhiwu, 0, sizeof(imzhiwu));
	memset(map, 0, sizeof(map));
	//��ʼ��ֲ�￨��
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
	putimage(0, 0, &imgBg);//��ȾͼƬ����ӡ��������
	putimagePNG(250, 0, &imBar);//��ȾͼƬ���ѿ��۴�ӡ��������
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		int x = 315 + i*60;
		int y = 0;
		putimagePNG(x, y, &imCards[i]);
	}
	//��Ⱦֲ��
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